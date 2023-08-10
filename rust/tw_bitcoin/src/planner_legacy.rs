use crate::entry::{BitcoinEntry, PlaceHolderProto};
use bitcoin::address::{Payload, WitnessVersion};
use bitcoin::key::TweakedPublicKey;
use bitcoin::{Address, ScriptBuf, VarInt, WPubkeyHash};
use secp256k1::hashes::Hash;
use secp256k1::XOnlyPublicKey;
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_proto::Bitcoin::Proto::{self as BitcoinProto, UnspentTransaction};

// Legacy implementations used for `Bitcoin.proto` backwards-compatibility.
impl BitcoinEntry {
    #[inline]
    fn compile_legacy(
        &self,
        _coin: &dyn CoinContext,
        _input: PlaceHolderProto<'static>,
        _signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> PlaceHolderProto<'static> {
        todo!()
    }

    #[inline]
    fn plan_builder_legacy(&self) -> Option<LegacyPlanBuilder> {
        None
    }
}

pub struct LegacyPlanBuilder;

impl LegacyPlanBuilder {
    fn plan<'a>(
        &self,
        _coin: &dyn CoinContext,
        proto: BitcoinProto::SigningInput<'a>,
    ) -> BitcoinProto::TransactionPlan<'a> {
        let proto_amount = proto.amount as u64;

        // Sort inputs by amount, ascending.
        let mut inputs = proto.utxo.to_vec();
        inputs.sort_by(|a, b| a.amount.partial_cmp(&b.amount).unwrap());

        // Select a set of inputs sufficent enough to cover `proto_amount`.
        let mut total_selected_amount = 0;
        let mut remaining = proto_amount;
        let selected: Vec<UnspentTransaction<'_>> = inputs
            .into_iter()
            .take_while(|input| {
                let input_amount = input.amount as u64;
                total_selected_amount += input_amount;
                remaining = remaining.saturating_sub(input_amount);

                remaining != 0
            })
            .collect();

        if remaining != 0 {
            // Return error
            todo!()
        }

        // Calculate the change amount that should be returned.
        let change = total_selected_amount - proto_amount;

        let mut outputs = vec![];

        // Primary send output (to target destination)
        let send_output = convert_address_to_script_pubkey(&proto.to_address);
        outputs.push(send_output);

        // Change output (to oneself).
        let change_output = convert_address_to_script_pubkey(&proto.change_address);
        outputs.push(change_output);

        // Calculate fee.
        let _weight = calculate_weight(&selected, &outputs);
        let fee = (_weight + 3) / 4 * proto.byte_fee as u64;

        // The generated TransactionPlan
        BitcoinProto::TransactionPlan {
            amount: proto.amount,
            available_amount: total_selected_amount as i64,
            fee: fee as i64,
            change: change as i64,
            utxos: selected,
            // Used for Zcash
            branch_id: Cow::default(),
            error: tw_proto::Common::Proto::SigningError::OK,
            // Used for other chain(s).
            output_op_return: Cow::default(),
            // Used for other chain(s).
            preblockhash: Cow::default(),
            // Used for other chain(s).
            preblockheight: 0,
        }
    }
}

fn calculate_weight<'a>(selected: &[UnspentTransaction<'a>], outputs: &[ScriptBuf]) -> u64 {
    // Non-witness data gets multiplied by the scale factor.
    const SCALE_FACTOR: usize = 4;

    // Base weight.
    let mut weight = SCALE_FACTOR
        * (
            // version
            4
            // lock time
            + 4
            // inputs/outputs VarInts
            + VarInt(selected.len() as u64).len()
            + VarInt(outputs.len() as u64).len()
        );

    // Witness item count.
    let mut witness_count = 0;

    // Count weight of all inputs.
    for input in selected {
        weight += SCALE_FACTOR
            * (
                // Outpoint
                32 + 4
				// Sequence
				+ 4
				+ VarInt(input.script.as_ref().len() as u64).len()
				+ input.script.len()
            );

        // Witness data weight is reduced (no scale factor).
        if !input.spendingScript.is_empty() {
            witness_count += 1;
            weight += input.spendingScript.len();
        }
    }

    // Count weight of all outputs.
    for output in outputs {
        weight +=
			// value
			8
			+ VarInt(output.len() as u64).len()
			+ output.len();
    }

    if witness_count != 0 {
        weight += selected.len() - witness_count + 2
    };

    weight as u64
}

fn convert_address_to_script_pubkey(address: &str) -> ScriptBuf {
    // Address type from `bitcoin` crate.
    let address = address
        .parse::<Address<_>>()
        .unwrap()
        .require_network(bitcoin::Network::Bitcoin)
        .unwrap();

    let script_pubkey = match address.address_type().unwrap() {
        bitcoin::AddressType::P2pkh => {
            if let Payload::PubkeyHash(ref hash) = address.payload {
                ScriptBuf::new_p2pkh(hash)
            } else {
                panic!()
            }
        },
        bitcoin::AddressType::P2sh => {
            if let Payload::ScriptHash(ref hash) = address.payload {
                ScriptBuf::new_p2sh(hash)
            } else {
                panic!()
            }
        },
        bitcoin::AddressType::P2wpkh => {
            if let Payload::WitnessProgram(program) = address.payload {
                // TODO
                assert_eq!(program.version(), WitnessVersion::V0);
                let slice = program.program().as_bytes();
                let wpubkey_hash = WPubkeyHash::from_slice(slice).unwrap();
                ScriptBuf::new_v0_p2wpkh(&wpubkey_hash)
            } else {
                panic!()
            }
        },
        bitcoin::AddressType::P2wsh => todo!(),
        bitcoin::AddressType::P2tr => {
            if let Payload::WitnessProgram(program) = address.payload {
                // TODO
                assert_eq!(program.version(), WitnessVersion::V1);
                let slice = program.program().as_bytes();
                let xonly = XOnlyPublicKey::from_slice(slice).unwrap();
                // TODO: another way to do this?
                let output_key = TweakedPublicKey::dangerous_assume_tweaked(xonly);
                ScriptBuf::new_v1_p2tr_tweaked(output_key)
            } else {
                panic!()
            }
        },
        _ => todo!(),
    };

    script_pubkey
}
