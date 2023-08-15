use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::Result;
use bitcoin::absolute::{Height, LockTime, Time};
use bitcoin::address::{NetworkChecked, Payload};
use bitcoin::consensus::encode::Encodable;
use bitcoin::key::{TapTweak, TweakedKeyPair};
use bitcoin::taproot::{ControlBlock, LeafVersion, TapLeafHash, TapNodeHash};
use bitcoin::{
    OutPoint, PubkeyHash, ScriptBuf, ScriptHash, Sequence, Transaction, TxIn, TxOut, Txid,
    WPubkeyHash, Witness,
};
use secp256k1::hashes::Hash;
use secp256k1::{KeyPair, Message, Secp256k1, XOnlyPublicKey};
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
use tw_utxo::compiler::StandardBitcoinContext;

// Convenience aliases.
pub type ProtoOutputRecipient<'a> = Proto::mod_Output::OneOfto_recipient<'a>;
pub type ProtoOutputBuilder<'a> = Proto::mod_Output::mod_Builder::OneOfvariant<'a>;
pub type ProtoPubkeyOrHash<'a> = Proto::mod_ToPublicKeyOrHash::OneOfto_address<'a>;
pub type ProtoInputRecipient<'a> = Proto::mod_Input::OneOfto_recipient<'a>;
pub type ProtoInputBuilder<'a> = Proto::mod_Input::mod_Builder::OneOfvariant<'a>;

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
        let txouts = process_recipients(&proto.outputs);

        let total_spent: u64 = txouts.iter().map(|output| output.value).sum();

        let mut utxo_inputs = vec![];
        for input in proto.inputs {
            let mut leaf_hash = None;

            let (signing_method, script_pubkey) = match &input.to_recipient {
                ProtoInputRecipient::builder(builder) => match &builder.variant {
                    ProtoInputBuilder::p2sh(_) => todo!(),
                    ProtoInputBuilder::p2pkh(pubkey) => {
                        let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                        (
                            UtxoProto::SigningMethod::Legacy,
                            ScriptBuf::new_p2pkh(&pubkey.pubkey_hash()),
                        )
                    },
                    ProtoInputBuilder::p2wsh(_) => todo!(),
                    ProtoInputBuilder::p2wpkh(pubkey) => {
                        let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                        (
                            UtxoProto::SigningMethod::Segwit,
                            ScriptBuf::new_v0_p2wpkh(&pubkey.wpubkey_hash().unwrap()),
                        )
                    },
                    ProtoInputBuilder::p2tr_key_path(pubkey) => {
                        let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();
                        let xonly = XOnlyPublicKey::from(pubkey.inner);

                        (
                            UtxoProto::SigningMethod::Taproot,
                            ScriptBuf::new_v1_p2tr(&secp256k1::Secp256k1::new(), xonly, None),
                        )
                    },
                    ProtoInputBuilder::p2tr_script_path(complex) => {
                        let script_buf = ScriptBuf::from_bytes(complex.payload.to_vec());
                        leaf_hash = Some(TapLeafHash::from_script(
                            script_buf.as_script(),
                            bitcoin::taproot::LeafVersion::TapScript,
                        ));

                        (UtxoProto::SigningMethod::Taproot, script_buf)
                    },
                    ProtoInputBuilder::brc20_inscribe(brc20) => {
                        let pubkey =
                            bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref()).unwrap();
                        let ticker = Ticker::new(brc20.ticker.to_string()).unwrap();

                        let brc20 = BRC20TransferInscription::new(
                            pubkey.into(),
                            ticker,
                            brc20.transfer_amount,
                        )
                        .unwrap();
                        leaf_hash = Some(TapLeafHash::from_script(
                            brc20.inscription().taproot_program(),
                            bitcoin::taproot::LeafVersion::TapScript,
                        ));

                        (
                            UtxoProto::SigningMethod::Taproot,
                            ScriptBuf::from(brc20.inscription().taproot_program()),
                        )
                    },
                    ProtoInputBuilder::None => todo!(),
                },
                ProtoInputRecipient::custom(custom) => {
                    todo!()
                },
                ProtoInputRecipient::None => todo!(),
            };

            utxo_inputs.push(UtxoProto::TxIn {
                txid: input.txid.to_vec().into(),
                vout: input.vout,
                amount: input.amount,
                script_pubkey: script_pubkey.to_vec().into(),
                signing_method,
                sighash_type: input.sighash_type,
                leaf_hash: leaf_hash
                    .map(|hash| hash.to_vec().into())
                    .unwrap_or_default(),
                one_prevout: input.one_prevout,
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
            //todo!()
        }

        let utxo_signing = UtxoProto::SigningInput {
            version: proto.version,
            lock_time: convert_locktime(&proto.lock_time),
            inputs: utxo_inputs.clone(),
            outputs: txouts
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
            txouts,
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
        for (index, input) in proto.inputs.iter().enumerate() {
            let sig_slice = &signatures[index];

            let (script_sig, witness) = match &input.to_recipient {
                ProtoInputRecipient::builder(variant) => match &variant.variant {
                    ProtoInputBuilder::None => panic!(),
                    ProtoInputBuilder::p2sh(_) => todo!(),
                    ProtoInputBuilder::p2pkh(pubkey) => {
                        let sig = bitcoin::ecdsa::Signature::from_slice(sig_slice).unwrap();
                        let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                        (
                            ScriptBuf::builder()
                                .push_slice(sig.serialize())
                                .push_key(&pubkey)
                                .into_script(),
                            Witness::new(),
                        )
                    },
                    ProtoInputBuilder::p2wsh(_) => todo!(),
                    ProtoInputBuilder::p2wpkh(pubkey) => {
                        let sig = bitcoin::ecdsa::Signature::from_slice(sig_slice).unwrap();
                        let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                        (ScriptBuf::new(), {
                            let mut w = Witness::new();
                            w.push(sig.serialize());
                            w.push(pubkey.to_bytes());
                            w
                        })
                    },
                    ProtoInputBuilder::p2tr_key_path(_) => {
                        let sig = bitcoin::taproot::Signature::from_slice(sig_slice).unwrap();

                        dbg!(&sig);

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
                    ProtoInputBuilder::brc20_inscribe(brc20) => {
                        let pubkey =
                            bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref()).unwrap();
                        let ticker = Ticker::new(brc20.ticker.to_string()).unwrap();
                        let control_block =
                            ControlBlock::decode(brc20.control_block.as_ref()).unwrap();

                        let brc20 = BRC20TransferInscription::new(
                            pubkey.into(),
                            ticker,
                            brc20.transfer_amount,
                        )
                        .unwrap();

                        let sig = bitcoin::taproot::Signature::from_slice(sig_slice).unwrap();

                        (ScriptBuf::new(), {
                            let mut w = Witness::new();
                            w.push(sig.to_vec());
                            w.push(brc20.inscription().taproot_program());
                            w.push(control_block.serialize());
                            w
                        })
                    },
                },
                ProtoInputRecipient::custom(custom) => (
                    ScriptBuf::from_bytes(custom.script_sig.to_vec()),
                    Witness::from_slice(&custom.witness_items),
                ),
                ProtoInputRecipient::None => {
                    todo!()
                },
            };

            utxo_input_claims.push(UtxoProto::TxInClaim {
                txid: input.txid.clone(),
                vout: input.vout,
                // TODO
                sequence: u32::MAX,
                script_sig: script_sig.to_vec().into(),
                witness_items: witness
                    .to_vec()
                    .into_iter()
                    .map(Cow::Owned)
                    .collect::<Vec<Cow<_>>>(),
            });
        }

        // Process all the outputs.
        let utxo_outputs = process_recipients(&proto.outputs);

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

fn process_recipients<'a>(
    outputs: &Vec<Proto::Output<'a>>,
) -> Vec<Proto::mod_PreSigningOutput::TxOut<'static>> {
    let mut utxo_outputs = vec![];

    let secp = secp256k1::Secp256k1::new();

    for output in outputs {
        let (script_pubkey, control_block) = match &output.to_recipient {
            // Script spending condition was passed on directly.
            ProtoOutputRecipient::script_pubkey(script) => {
                (ScriptBuf::from_bytes(script.to_vec()), None)
            },
            // Process builder methods. We construct the Script spending
            // conditions by using the specified parameters.
            ProtoOutputRecipient::builder(builder) => match &builder.variant {
                ProtoOutputBuilder::p2sh(_) => {
                    todo!()
                },
                ProtoOutputBuilder::p2pkh(pubkey_or_hash) => {
                    let pubkey_hash = pubkey_hash_from_proto(pubkey_or_hash).unwrap();
                    (ScriptBuf::new_p2pkh(&pubkey_hash), None)
                },
                ProtoOutputBuilder::p2wsh(_) => {
                    todo!()
                },
                ProtoOutputBuilder::p2wpkh(pubkey_or_hash) => {
                    let wpubkey_hash = witness_pubkey_hash_from_proto(pubkey_or_hash).unwrap();
                    (ScriptBuf::new_v0_p2wpkh(&wpubkey_hash), None)
                },
                ProtoOutputBuilder::p2tr_key_path(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();
                    let xonly = XOnlyPublicKey::from(pubkey.inner);
                    (ScriptBuf::new_v1_p2tr(&secp, xonly, None), None)
                },
                ProtoOutputBuilder::p2tr_script_path(complex) => {
                    let node_hash = TapNodeHash::from_slice(complex.node_hash.as_ref()).unwrap();

                    let pubkey =
                        bitcoin::PublicKey::from_slice(complex.public_key.as_ref()).unwrap();
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    (ScriptBuf::new_v1_p2tr(&secp, xonly, Some(node_hash)), None)
                },
                ProtoOutputBuilder::brc20_inscribe(brc20) => {
                    let pubkey =
                        bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref()).unwrap();
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    let ticker = Ticker::new(brc20.ticker.to_string()).unwrap();
                    let brc20 =
                        BRC20TransferInscription::new(pubkey.into(), ticker, brc20.transfer_amount)
                            .unwrap();

                    // Explicit check
                    let control_block = brc20
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            brc20.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .unwrap();
                    let merkle_root = brc20.inscription().spend_info().merkle_root().unwrap();
                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, Some(merkle_root)),
                        Some(control_block.serialize()),
                    )
                },
                ProtoOutputBuilder::None => todo!(),
            },
            // We derive the spending condition for the address.
            ProtoOutputRecipient::from_address(_) => todo!(),
            ProtoOutputRecipient::None => todo!(),
        };

        utxo_outputs.push(Proto::mod_PreSigningOutput::TxOut {
            value: output.amount,
            script_pubkey: script_pubkey.to_vec().into(),
            control_block: control_block.map(|cb| cb.into()).unwrap_or_default(),
        });
    }

    utxo_outputs
}
