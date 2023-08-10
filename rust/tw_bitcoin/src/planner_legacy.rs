use crate::TransactionBuilder;
use crate::entry::{BitcoinEntry, PlaceHolderProto};
use bitcoin::absolute::{Height, LockTime, Time};
use bitcoin::address::{Payload, WitnessVersion};
use bitcoin::key::TweakedPublicKey;
use bitcoin::{
    Address, OutPoint, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, VarInt, WPubkeyHash,
    Witness,
};
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
    fn compile_legacy<'a>(
        &self,
        coin: &dyn CoinContext,
        mut proto: BitcoinProto::SigningInput::<'a>,
    ) -> PlaceHolderProto<'static> {
        let proto_amount = proto.amount as u64;

        // If the TransactionPlan is missing, it will be computed.
        let plan = if let Some(plan) = proto.plan {
            plan
        } else {
            LegacyPlanBuilder.plan(coin, proto.clone())
        };

        let mut outputs: Vec<TxOut> = vec![];

        // Primary send output (to target destination)
        let send_output = convert_address_to_script_pubkey(&proto.to_address);
        outputs.push(TxOut {
            value: plan.amount as u64,
            script_pubkey: send_output,
        });

        // Change output (to oneself).
        let change_output = convert_address_to_script_pubkey(&proto.change_address);
        outputs.push(TxOut {
            value: plan.amount as u64,
            script_pubkey: change_output,
        });

        // Create inputs for the `bitcoin` crate.
        let _txin = plan.utxos
            .iter()
            .map(|input| {
                let out_point = input.out_point.as_ref().unwrap();
                TxIn {
                    previous_output: OutPoint {
                        txid: Txid::from_slice(out_point.hash.as_ref()).unwrap(),
                        vout: out_point.index,
                    },
                    script_sig: ScriptBuf::from_bytes(input.script.to_vec()),
                    // TODO: Note that UnspendTransaction has two "sequence" fields (for
                    // some reason)... not sure how to handle this yet.
                    sequence: Sequence::from_consensus(out_point.sequence),
                    witness: Witness::from_slice(&[input.spendingScript.as_ref()]),
                }
            })
            .collect();

        // Create transaction from `bitcoin` crate, let it calculate the weight.
        // After that it's no longer needed.
        let tx = TransactionBuilder::new()
            .version(2)
            .lock_time_native({
                // TODO: Double check this.
                if proto.lock_time < 500_000_000 {
                    LockTime::Blocks(Height::from_consensus(proto.lock_time).unwrap())
                } else {
                    LockTime::Seconds(Time::from_consensus(proto.lock_time).unwrap())
                }
            })

        todo!()
    }

    #[inline]
    fn plan_builder_legacy(&self) -> Option<LegacyPlanBuilder> {
        Some(LegacyPlanBuilder)
    }
}

// TODO: Should implement PlanBuilder.
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
        let change_amount = total_selected_amount - proto_amount;

        let mut outputs: Vec<TxOut> = vec![];

        // Primary send output (to target destination)
        let send_output = convert_address_to_script_pubkey(&proto.to_address);
        outputs.push(TxOut {
            value: proto_amount,
            script_pubkey: send_output,
        });

        // Change output (to oneself).
        let change_output = convert_address_to_script_pubkey(&proto.change_address);
        outputs.push(TxOut {
            value: change_amount,
            script_pubkey: change_output,
        });

        // Create inputs for the `bitcoin` crate.
        let _txin = selected
            .iter()
            .map(|input| {
                let out_point = input.out_point.as_ref().unwrap();
                TxIn {
                    previous_output: OutPoint {
                        txid: Txid::from_slice(out_point.hash.as_ref()).unwrap(),
                        vout: out_point.index,
                    },
                    script_sig: ScriptBuf::from_bytes(input.script.to_vec()),
                    // TODO: Note that UnspendTransaction has two "sequence" fields (for
                    // some reason)... not sure how to handle this yet.
                    sequence: Sequence::from_consensus(out_point.sequence),
                    witness: Witness::from_slice(&[input.spendingScript.as_ref()]),
                }
            })
            .collect();

        // Create transaction from `bitcoin` crate, let it calculate the weight.
        // After that it's no longer needed.
        let tx = Transaction {
            version: 2,
            lock_time: {
                // TODO: Double check this.
                if proto.lock_time < 500_000_000 {
                    LockTime::Blocks(Height::from_consensus(proto.lock_time).unwrap())
                } else {
                    LockTime::Seconds(Time::from_consensus(proto.lock_time).unwrap())
                }
            },
            input: _txin,
            output: outputs,
        };

        // Calculate fee.
        let _weight = tx.weight().to_wu();
        let fee = (_weight + 3) / 4 * proto.byte_fee as u64;

        // The generated TransactionPlan
        BitcoinProto::TransactionPlan {
            amount: proto.amount,
            available_amount: total_selected_amount as i64,
            fee: fee as i64,
            change: change_amount as i64,
            utxos: selected,
            // Empty, used for Zcash
            branch_id: Cow::default(),
            error: tw_proto::Common::Proto::SigningError::OK,
            // Empty, used for other chain(s).
            output_op_return: Cow::default(),
            // Empty, used for other chain(s).
            preblockhash: Cow::default(),
            // Empty, used for other chain(s).
            preblockheight: 0,
        }
    }
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
