use crate::Result;
use bitcoin::absolute::{Height, LockTime, Time};
use bitcoin::address::{NetworkChecked, Payload};
use bitcoin::consensus::encode::Encodable;
use bitcoin::taproot::ControlBlock;
use bitcoin::{
    OutPoint, PubkeyHash, ScriptBuf, ScriptHash, Sequence, Transaction, TxIn, TxOut, Txid,
    WPubkeyHash, Witness,
};
use secp256k1::hashes::Hash;
use std::borrow::Cow;
use std::fmt::Display;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

// Convenience aliases.
type ProtoOutputRecipient<'a> = Proto::mod_Output::OneOfto_recipient<'a>;
type ProtoBuilderType<'a> = Proto::mod_Builder::OneOftype_pb<'a>;
type ProtoPubkeyOrHash<'a> = Proto::mod_ToPublicKeyOrHash::OneOfto_address<'a>;
type ProtoInputVariant<'a> = Proto::mod_Input::OneOfvariant<'a>;
type ProtoInputBuilder<'a> = Proto::mod_Input::mod_InputVariant::OneOfvariant<'a>;

pub type PlaceHolderProto<'a> = tw_proto::Bitcoin::Proto::SigningInput<'a>;

pub struct PlaceHolder;

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
    val: Proto::mod_SigningInput::OneOflock_time,
) -> UtxoProto::mod_SigningInput::OneOflock_time {
    match val {
        Proto::mod_SigningInput::OneOflock_time::blocks(blocks) => {
            UtxoProto::mod_SigningInput::OneOflock_time::blocks(blocks)
        },
        Proto::mod_SigningInput::OneOflock_time::seconds(seconds) => {
            UtxoProto::mod_SigningInput::OneOflock_time::seconds(seconds)
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
    fn sign(&self, _coin: &dyn CoinContext, _input: Self::SigningInput<'_>) -> Self::SigningOutput {
        todo!()
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        let mut total_spent: u64 = 0;

        let mut utxo_outputs = vec![];
        for output in &proto.outputs {
            let amount = output.amount as u64;
            total_spent += amount;

            let script_pubkey = match &output.to_recipient {
                ProtoOutputRecipient::script_pubkey(script) => {
                    ScriptBuf::from_bytes(script.to_vec())
                },
                ProtoOutputRecipient::builder(builder) => match &builder.type_pb {
                    ProtoBuilderType::p2sh(hash) => {
                        todo!()
                    },
                    ProtoBuilderType::p2pkh(pubkey_or_hash) => {
                        let pubkey_hash = pubkey_hash_from_proto(pubkey_or_hash).unwrap();
                        ScriptBuf::new_p2pkh(&pubkey_hash)
                    },
                    ProtoBuilderType::p2wsh(_) => {
                        todo!()
                    },
                    ProtoBuilderType::p2wpkh(pubkey_or_hash) => {
                        let wpubkey_hash = witness_pubkey_hash_from_proto(pubkey_or_hash).unwrap();
                        ScriptBuf::new_v0_p2wpkh(&wpubkey_hash)
                    },
                    ProtoBuilderType::p2tr_key_path(pubkey) => {
                        todo!()
                    },
                    ProtoBuilderType::p2tr_script_path(pubkey) => {
                        todo!()
                    },
                    ProtoBuilderType::None => todo!(),
                },
                ProtoOutputRecipient::from_address(address) => todo!(),
                ProtoOutputRecipient::None => todo!(),
            };

            utxo_outputs.push(UtxoProto::TxOut {
                value: amount,
                script_pubkey: script_pubkey.to_vec().into(),
            });
        }

        let mut utxo_inputs = vec![];
        for input in &proto.inputs {
            let sighash_method = match &input.variant {
                ProtoInputVariant::builder(builder) => match &builder.variant {
                    ProtoInputBuilder::p2sh(_) => todo!(),
                    ProtoInputBuilder::p2pkh(pubkey_or_hash) => {
                        let pubkey_hash = pubkey_hash_from_proto(pubkey_or_hash).unwrap();

                        UtxoProto::mod_TxIn::OneOfsighash_method::legacy(
                            UtxoProto::mod_TxIn::Legacy {
                                script_pubkey: ScriptBuf::new_p2pkh(&pubkey_hash).to_vec().into(),
                            },
                        )
                    },
                    ProtoInputBuilder::p2wsh(_) => todo!(),
                    ProtoInputBuilder::p2wpkh(_) => todo!(),
                    ProtoInputBuilder::p2tr_key_path(_) => todo!(),
                    ProtoInputBuilder::p2tr_script_path(_) => {
                        todo!()
                    },
                    ProtoInputBuilder::None => todo!(),
                },
                ProtoInputVariant::custom(custom) => {
                    todo!()
                },
                ProtoInputVariant::None => todo!(),
            };

            utxo_inputs.push(UtxoProto::TxIn {
                txid: input.txid.clone(),
                vout: input.vout,
                amount: input.amount,
                sighash_method,
                // TODO
                sighash: UtxoProto::SighashType::All,
            });
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
            todo!()
        }

        let utxo_signing = UtxoProto::SigningInput {
            version: proto.version,
            lock_time: convert_locktime(proto.lock_time),
            inputs: utxo_inputs,
            outputs: utxo_outputs,
        };

        let utxo_presigning = tw_utxo::compiler::Compiler::preimage_hashes(&utxo_signing);

        Proto::PreSigningOutput {
            error: 0,
            sighashes: utxo_presigning.sighashes,
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
        for (index, input) in proto.inputs.iter().enumerate() {
            let sig_slice = &signatures[index];

            let (script_sig, witness) = match &input.variant {
                ProtoInputVariant::builder(variant) => match &variant.variant {
                    ProtoInputBuilder::p2pkh(pubkey_or_hash) => {
                        let sig = bitcoin::ecdsa::Signature::from_slice(sig_slice).unwrap();
                        let pubkey_hash = pubkey_hash_from_proto(pubkey_or_hash).unwrap();

                        (
                            ScriptBuf::builder()
                                .push_slice(sig.serialize())
                                .push_slice(pubkey_hash)
                                .into_script(),
                            Witness::new(),
                        )
                    },
                    ProtoInputBuilder::p2wpkh(pubkey_or_hash) => {
                        let sig = bitcoin::ecdsa::Signature::from_slice(sig_slice).unwrap();
                        let wpubkey_hash = witness_pubkey_hash_from_proto(pubkey_or_hash).unwrap();

                        (ScriptBuf::new(), {
                            let mut w = Witness::new();
                            w.push(sig.serialize());
                            w.push(wpubkey_hash.as_byte_array());
                            w
                        })
                    },
                    ProtoInputBuilder::p2tr_key_path(_) => {
                        let sig = bitcoin::taproot::Signature::from_slice(sig_slice).unwrap();

                        (ScriptBuf::new(), {
                            let mut w = Witness::new();
                            w.push(sig.to_vec());
                            w
                        })
                    },
                    ProtoInputBuilder::p2tr_script_path(taproot) => {
                        let sig = bitcoin::taproot::Signature::from_slice(sig_slice).unwrap();
                        let control_block =
                            ControlBlock::decode(taproot.control_block.as_ref()).unwrap();

                        (ScriptBuf::new(), {
                            let mut w = Witness::new();
                            w.push(sig.to_vec());
                            w.push(taproot.payload.as_ref());
                            w.push(control_block.serialize());
                            w
                        })
                    },
                    _ => panic!(),
                },
                ProtoInputVariant::custom(custom) => (
                    ScriptBuf::from_bytes(custom.script_sig.to_vec()),
                    Witness::from_slice(&custom.witness_items),
                ),
                ProtoInputVariant::None => {
                    todo!()
                },
            };

            utxo_input_claims.push(UtxoProto::TxInClaim {
                txid: input.txid.clone(),
                vout: input.vout,
                // TODO
                sequence: 0,
                script_sig: script_sig.to_vec().into(),
                witness_items: witness
                    .to_vec()
                    .into_iter()
                    .map(Cow::Owned)
                    .collect::<Vec<Cow<_>>>(),
            });
        }

        let mut native_txouts: Vec<TxOut> = vec![];
        for output in proto.outputs {
            let script_pubkey = match output.to_recipient {
                ProtoOutputRecipient::script_pubkey(script) => {
                    ScriptBuf::from_bytes(script.to_vec())
                },
                _ => todo!(),
            };

            native_txouts.push(TxOut {
                value: output.amount as u64,
                script_pubkey,
            });
        }

        let native_lock_time = match proto.lock_time {
            Proto::mod_SigningInput::OneOflock_time::blocks(blocks) => {
                LockTime::Blocks(Height::from_consensus(blocks).unwrap())
            },
            Proto::mod_SigningInput::OneOflock_time::seconds(blocks) => {
                LockTime::Seconds(Time::from_consensus(blocks).unwrap())
            },
            Proto::mod_SigningInput::OneOflock_time::None => {
                LockTime::Blocks(Height::from_consensus(0).unwrap())
            },
        };

        // Use native type from `bitcoin` crate for encoding.
        let native_tx = Transaction {
            version: 2,
            lock_time: native_lock_time,
            input: vec![],
            output: native_txouts,
        };

        // Encode the transaction.
        let mut encoded = vec![];
        native_tx.consensus_encode(&mut encoded).unwrap();

        // Prepare `Proto::TransactionInput` protobufs for signing output.
        let mut proto_inputs = vec![];
        for input in &native_tx.input {
            proto_inputs.push(Proto::TransactionInput {
                txid: input.previous_output.txid.to_vec().into(),
                vout: input.previous_output.vout,
                sequence: input.sequence.to_consensus_u32(),
                script_sig: input.script_sig.to_vec().into(),
                witness_items: input
                    .witness
                    .to_vec()
                    .into_iter()
                    .map(Cow::Owned)
                    .collect::<Vec<Cow<_>>>(),
            });
        }

        // Prepare `Proto::TransactionOutput` protobufs for output.
        let mut proto_outputs = vec![];
        for output in &native_tx.output {
            proto_outputs.push(Proto::TransactionOutput {
                recipient: Cow::default(),
                script_pubkey: output.script_pubkey.to_vec().into(),
                amount: output.value,
                control_block: None,
            });
        }

        // Prepare `Proto::Transaction` protobuf for output.
        let transaction = Proto::Transaction {
            version: proto.version,
            lock_time: native_tx.lock_time.to_consensus_u32(),
            inputs: proto_inputs,
            outputs: proto_outputs,
        };

        // Return the full protobuf output.
        Proto::SigningOutput {
            transaction: Some(transaction),
            encoded: encoded.into(),
            transaction_id: native_tx.txid().to_vec().into(),
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

fn pubkey_hash_from_proto(pubkey_or_hash: &Proto::ToPublicKeyOrHash) -> Result<PubkeyHash> {
    let pubkey_hash = match &pubkey_or_hash.to_address {
        ProtoPubkeyOrHash::hash(hash) => PubkeyHash::from_slice(hash.as_ref()).unwrap(),
        ProtoPubkeyOrHash::pubkey(pubkey) => bitcoin::PublicKey::from_slice(pubkey.as_ref())
            .unwrap()
            .pubkey_hash(),
        ProtoPubkeyOrHash::None => return Err(crate::Error::Todo),
    };

    Ok(pubkey_hash)
}

fn witness_pubkey_hash_from_proto(
    pubkey_or_hash: &Proto::ToPublicKeyOrHash,
) -> Result<WPubkeyHash> {
    let wpubkey_hash = match &pubkey_or_hash.to_address {
        ProtoPubkeyOrHash::hash(hash) => WPubkeyHash::from_slice(hash.as_ref()).unwrap(),
        ProtoPubkeyOrHash::pubkey(pubkey) => bitcoin::PublicKey::from_slice(pubkey.as_ref())
            .unwrap()
            .wpubkey_hash()
            .unwrap(),
        ProtoPubkeyOrHash::None => todo!(),
    };

    Ok(wpubkey_hash)
}
