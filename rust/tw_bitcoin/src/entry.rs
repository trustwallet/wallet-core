use bitcoin::address::{NetworkChecked, Payload};
use bitcoin::key::{TapTweak, TweakedKeyPair};
use secp256k1::hashes::Hash;
use secp256k1::{KeyPair, Message, Secp256k1};
use std::borrow::Cow;
use std::fmt::Display;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey, PublicKeyType};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

// Convenience aliases.
pub use aliases::*;
pub mod aliases {
    use super::Proto;

    pub type ProtoOutputRecipient<'a> = Proto::mod_Output::OneOfto_recipient<'a>;
    pub type ProtoOutputBuilder<'a> = Proto::mod_Output::mod_Builder::OneOfvariant<'a>;
    pub type ProtoPubkeyOrHash<'a> = Proto::mod_ToPublicKeyOrHash::OneOfto_address<'a>;
    pub type ProtoInputRecipient<'a> = Proto::mod_Input::OneOfto_recipient<'a>;
    pub type ProtoInputBuilder<'a> = Proto::mod_Input::mod_Builder::OneOfvariant<'a>;
}

pub type PlaceHolderProto<'a> = tw_proto::Bitcoin::Proto::SigningInput<'a>;

pub struct PlaceHolder;

impl CoinContext for PlaceHolder {
    fn public_key_type(&self) -> PublicKeyType {
        todo!()
    }
}

pub struct NoJsonSigner;

impl JsonSigner for NoJsonSigner {
    fn sign_json(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        _input_json: &str,
        _key: &PrivateKey,
    ) -> tw_coin_entry::error::SigningResult<String> {
        todo!()
    }
}

pub struct BitcoinEntry;

pub struct Address(bitcoin::address::Address<NetworkChecked>);

impl Display for Address {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl CoinAddress for Address {
    fn data(&self) -> tw_memory::Data {
        match &self.0.payload {
            Payload::PubkeyHash(hash) => hash.to_byte_array().into(),
            Payload::ScriptHash(hash) => hash.to_byte_array().into(),
            Payload::WitnessProgram(wp) => wp.program().as_bytes().into(),
            _ => todo!(), // Payload is non-exhaustive
        }
    }
}

// Todo: type should be unified.
fn convert_locktime(
    val: &Proto::mod_SigningInput::OneOflock_time,
) -> UtxoProto::mod_SigningInput::OneOflock_time {
    match val {
        Proto::mod_SigningInput::OneOflock_time::blocks(blocks) => {
            UtxoProto::mod_SigningInput::OneOflock_time::blocks(*blocks)
        },
        Proto::mod_SigningInput::OneOflock_time::seconds(seconds) => {
            UtxoProto::mod_SigningInput::OneOflock_time::seconds(*seconds)
        },
        Proto::mod_SigningInput::OneOflock_time::None => {
            UtxoProto::mod_SigningInput::OneOflock_time::None
        },
    }
}

impl CoinEntry for BitcoinEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        _address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        todo!()
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        _public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        todo!()
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, proto: Self::SigningInput<'_>) -> Self::SigningOutput {
        // TODO: Can we avoid cloning here?
        let pre_signed = self.preimage_hashes(_coin, proto.clone());
        // TODO: Check error

        dbg!(&pre_signed);

        let secp = Secp256k1::new();
        let keypair = KeyPair::from_seckey_slice(&secp, proto.private_key.as_ref()).unwrap();

        let mut signatures: Vec<SignatureBytes> = vec![];

        for (entry, utxo_in) in pre_signed
            .sighashes
            .iter()
            .zip(pre_signed.utxo_inputs.iter())
        {
            let sighash = Message::from_slice(entry.sighash.as_ref()).unwrap();

            match entry.signing_method {
                UtxoProto::SigningMethod::Legacy | UtxoProto::SigningMethod::Segwit => {
                    let sig = bitcoin::ecdsa::Signature {
                        sig: keypair.secret_key().sign_ecdsa(sighash),
                        // TODO
                        hash_ty: bitcoin::sighash::EcdsaSighashType::All,
                    };

                    signatures.push(sig.serialize().to_vec());
                },
                UtxoProto::SigningMethod::Taproot => {
                    // Any empty leaf hash implies P2TR key-path (balance transfer)
                    if utxo_in.leaf_hash.is_empty() {
                        // Tweak keypair for P2TR key-path (ie. zeroed Merkle root).
                        let tapped: TweakedKeyPair = keypair.tap_tweak(&secp, None);
                        let tweaked = KeyPair::from(tapped);

                        dbg!(&sighash);

                        // Construct the Schnorr signature.
                        #[cfg(not(test))]
                        let schnorr = secp.sign_schnorr(&sighash, &tweaked);
                        #[cfg(test)]
                        // For tests, we disable the included randomness in order to create
                        // reproducible signatures. Randomness should ALWAYS be used in
                        // production.
                        let schnorr = secp.sign_schnorr_no_aux_rand(&sighash, &tweaked);

                        let sig = bitcoin::taproot::Signature {
                            sig: schnorr,
                            // TODO.
                            hash_ty: bitcoin::sighash::TapSighashType::Default,
                        };

                        dbg!(&sig);

                        signatures.push(sig.to_vec());
                    }
                    // If it has a leaf hash, then it's a P2TR script-path (complex transaction)
                    else {
                        // We do not tweak the key here since we're passing on
                        // the "control block" when claiming, hence this signing
                        // process is simpler that P2TR key-path.
                        let sig = bitcoin::taproot::Signature {
                            sig: keypair.sign_schnorr(sighash),
                            // TODO.
                            hash_ty: bitcoin::sighash::TapSighashType::Default,
                        };

                        signatures.push(sig.to_vec());
                    }
                },
            }
        }

        self.compile(_coin, proto, signatures, vec![])
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        let mut utxo_outputs = vec![];
        for output in proto.outputs {
            let utxo = crate::modules::OutputBuilder::utxo_from_proto(&output).unwrap();

            utxo_outputs.push(utxo);
        }

        let total_spent: u64 = utxo_outputs.iter().map(|output| output.value).sum();

        let mut utxo_inputs = vec![];
        for input in proto.inputs {
            let txin = crate::modules::InputBuilder::utxo_from_proto(&input).unwrap();
            utxo_inputs.push(txin);
        }

        let mut remaining = total_spent;
        // TODO: This logic can be combined with the processor above.
        match proto.input_selector {
            Proto::SelectorType::AutomaticAscending => {
                utxo_inputs.sort_by(|a, b| a.amount.partial_cmp(&b.amount).unwrap());

                let mut total_input_amount = 0;
                utxo_inputs = utxo_inputs
                    .into_iter()
                    .take_while(|input| {
                        total_input_amount += input.amount;
                        remaining = remaining.saturating_sub(input.amount);

                        remaining != 0
                    })
                    .collect();
            },
            // Do nothing.
            Proto::SelectorType::UseAll => {},
        }

        if remaining != 0 {
            // Error, insufficient funds.
            //todo!()
        }

        let utxo_signing = UtxoProto::SigningInput {
            version: proto.version,
            lock_time: convert_locktime(&proto.lock_time),
            inputs: utxo_inputs.clone(),
            outputs: utxo_outputs
                .iter()
                .map(|out| UtxoProto::TxOut {
                    value: out.value,
                    script_pubkey: Cow::Borrowed(&out.script_pubkey),
                })
                .collect(),
        };

        let utxo_presigning = tw_utxo::compiler::Compiler::preimage_hashes(utxo_signing);

        Proto::PreSigningOutput {
            error: 0,
            sighashes: utxo_presigning.sighashes,
            utxo_inputs,
            utxo_outputs,
        }
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        if proto.inputs.len() != signatures.len() {
            // Error
            todo!()
        }

        let mut utxo_input_claims: Vec<UtxoProto::TxInClaim> = vec![];

        // Generate claims for all the inputs.
        for (input, signature) in proto.inputs.iter().zip(signatures.into_iter()) {
            let utxo_claim =
                crate::modules::InputBuilder::utxo_claim_from_proto(input, signature).unwrap();
            utxo_input_claims.push(utxo_claim);
        }

        // Process all the outputs.
        let mut utxo_outputs = vec![];
        for output in proto.outputs {
            let utxo = crate::modules::OutputBuilder::utxo_from_proto(&output).unwrap();

            utxo_outputs.push(utxo);
        }

        let utxo_preserializtion = UtxoProto::PreSerialization {
            version: proto.version,
            // TODO:
            lock_time: UtxoProto::mod_PreSerialization::OneOflock_time::blocks(0),
            inputs: utxo_input_claims.clone(),
            outputs: utxo_outputs
                .iter()
                .map(|out| UtxoProto::TxOut {
                    value: out.value,
                    script_pubkey: Cow::Borrowed(&out.script_pubkey),
                })
                .collect(),
        };

        let utxo_serialized = tw_utxo::compiler::Compiler::compile(utxo_preserializtion);

        // Prepare `Proto::TransactionInput` protobufs for signing output.
        let mut proto_inputs = vec![];
        for input in utxo_input_claims {
            proto_inputs.push(Proto::TransactionInput {
                txid: Cow::Owned(input.txid.to_vec()),
                vout: input.vout,
                sequence: input.sequence,
                script_sig: Cow::Owned(input.script_sig.into_owned()),
                witness_items: input
                    .witness_items
                    .into_iter()
                    .map(|item| Cow::Owned(item.into_owned()))
                    .collect(),
            });
        }

        // Prepare `Proto::TransactionOutput` protobufs for output.
        let mut proto_outputs = vec![];
        for output in utxo_outputs {
            proto_outputs.push(Proto::TransactionOutput {
                recipient: Cow::default(),
                script_pubkey: output.script_pubkey,
                amount: output.value,
                control_block: output.control_block,
            });
        }

        // Prepare `Proto::Transaction` protobuf for output.
        let transaction = Proto::Transaction {
            version: proto.version,
            // TODO
            lock_time: 0,
            inputs: proto_inputs,
            outputs: proto_outputs,
        };

        // Return the full protobuf output.
        Proto::SigningOutput {
            transaction: Some(transaction),
            encoded: utxo_serialized.encoded,
            // TODO: Should be returned by `tw_utxo`.
            transaction_id: Cow::default(),
            error: 0,
            fee: 0,
        }
    }

    #[inline]
    fn json_signer(&self) -> Option<Self::JsonSigner> {
        None
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        None
    }
}
