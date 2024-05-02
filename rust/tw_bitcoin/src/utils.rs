use std::str::FromStr;

use crate::{Error, Result};
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_hash::{H160, H256};
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::ecdsa::signature::Signature;
use tw_keypair::schnorr;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::tw::PublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto::mod_Input::mod_InputBuilder::OneOfvariant as OneOfInputVariant;
use tw_proto::BitcoinV2::Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant as OneOfOutputVariant;
use tw_proto::BitcoinV2::Proto::mod_ToPublicKeyOrHash::OneOfto_address;
use tw_proto::BitcoinV2::Proto::{self, mod_Output};
use tw_proto::Utxo::Proto as UtxoProto;
use tw_utxo::address::standard_bitcoin::StandardBitcoinAddress;
use tw_utxo::script::{Script, Witness};
use tw_utxo::sighash_computer::{SpendingData, UtxoToSign};
use tw_utxo::transaction::standard_transaction::builder::{
    OutputBuilder, SpendingScriptBuilder, UtxoBuilder,
};
use tw_utxo::transaction::standard_transaction::{TransactionInput, TransactionOutput};

// TODO: Adjust error type
fn pubkey_from_raw(pubkey: &[u8]) -> Result<PublicKey> {
    PublicKey::new(pubkey.to_vec(), tw_keypair::tw::PublicKeyType::Secp256k1)
        .map_err(|_| Error::from(Proto::Error::Error_internal))
}

fn schnorr_pubkey_from_raw(pubkey: &[u8]) -> Result<schnorr::PublicKey> {
    schnorr::PublicKey::try_from(pubkey).map_err(|_| Error::from(Proto::Error::Error_internal))
}

pub fn proto_input_to_native(
    input: &Proto::Input,
    sig: Option<SignatureBytes>,
) -> Result<(TransactionInput, UtxoToSign, SpendingData)> {
    match &input.to_recipient {
        Proto::mod_Input::OneOfto_recipient::builder(b) => {
            match &b.variant {
                OneOfInputVariant::p2sh(redeem_script) => {
                    let redeem_script = Script::from(redeem_script.to_vec());

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2sh(redeem_script.clone())
                        .unwrap();

                    // Note that the signature is ignored here, since it's
                    // expected that the provided redeem script is constructed
                    // in such a way that it can claim the UTXO. The caller must
                    // either use None or a dummy value.

                    let claim = SpendingScriptBuilder::new().p2sh(redeem_script);

                    Ok((utxo, arg, claim))
                },
                OneOfInputVariant::p2pkh(pubkey) => {
                    let pubkey = pubkey_from_raw(pubkey).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2pkh(pubkey.clone())
                        .unwrap();

                    // Use the provided signature, or a dummy value if None.
                    let sig = if let Some(sig_bytes) = sig {
                        Signature::from_bytes(&sig_bytes).unwrap()
                    } else {
                        Signature::from_bytes(&[1; 65]).unwrap()
                    };

                    let claim = SpendingScriptBuilder::new().p2pkh(sig, pubkey).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfInputVariant::p2wsh(_) => todo!(),
                OneOfInputVariant::p2wpkh(pubkey) => {
                    let pubkey = pubkey_from_raw(pubkey).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2wpkh(pubkey.clone())
                        .unwrap();

                    // Use the provided signature, or a dummy value if None.
                    let sig = if let Some(sig_bytes) = sig {
                        Signature::from_bytes(&sig_bytes).unwrap()
                    } else {
                        Signature::from_bytes(&[1; 65]).unwrap()
                    };

                    let claim = SpendingScriptBuilder::new().p2wpkh(sig, pubkey).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfInputVariant::p2tr_key_path(ctx) => {
                    // TODO: Rename field to `pubkey`?
                    let pubkey = schnorr_pubkey_from_raw(&ctx.public_key).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2tr_key_path(&pubkey)
                        .unwrap();

                    // Use the provided signature, or a dummy value if None.
                    let sig = if let Some(sig_bytes) = sig {
                        schnorr::Signature::from_bytes(&sig_bytes).unwrap()
                    } else {
                        schnorr::Signature::from_bytes(&[1; 64]).unwrap()
                    };

                    let claim = SpendingScriptBuilder::new().p2tr_key_path(sig);

                    Ok((utxo, arg, claim))
                },
                OneOfInputVariant::p2tr_script_path(ctx) => {
                    let payload = Script::from(ctx.payload.to_vec());
                    let control_block = ctx.control_block.to_vec();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2tr_script_path(payload.clone())
                        .unwrap();

                    // Use the provided signature, or a dummy value if None.
                    let sig = if let Some(sig_bytes) = sig {
                        schnorr::Signature::from_bytes(&sig_bytes).unwrap()
                    } else {
                        schnorr::Signature::from_bytes(&[1; 64]).unwrap()
                    };

                    let claim =
                        SpendingScriptBuilder::new().p2tr_script_path(sig, payload, control_block);

                    Ok((utxo, arg, claim))
                },
                OneOfInputVariant::brc20_inscribe(payload) => {
                    // TODO: Rename field to `pubkey`?
                    let pubkey = schnorr_pubkey_from_raw(&payload.inscribe_to).unwrap();
                    let ticker = payload.ticker.to_string();
                    let value = payload.transfer_amount.to_string();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        .brc20_transfer(&pubkey, ticker.clone(), value.clone())
                        .unwrap();

                    // Use the provided signature, or a dummy value if None.
                    let sig = if let Some(sig_bytes) = sig {
                        schnorr::Signature::from_bytes(&sig_bytes).unwrap()
                    } else {
                        schnorr::Signature::from_bytes(&[1; 64]).unwrap()
                    };

                    let claim = SpendingScriptBuilder::new()
                        .brc20_transfer(sig, &pubkey, ticker, value)
                        .unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfInputVariant::ordinal_inscribe(_) => todo!(),
                OneOfInputVariant::None => todo!(),
            }
        },
        Proto::mod_Input::OneOfto_recipient::custom_script(payload) => {
            let script_pubkey = Script::from(payload.script_pubkey.to_vec());
            let script_sig = Script::from(payload.script_sig.to_vec());
            let mut witness = Witness::new();
            for item in &payload.witness_items {
                witness.push_item(Script::from(item.to_vec()));
            }

            let (utxo, arg) = UtxoBuilder::new()
                .prev_txid(input.txid.as_ref().try_into().unwrap())
                .prev_index(input.vout)
                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                // TODO: Signing method:
                .custom_script_pubkey(script_pubkey, tw_utxo::signing_mode::SigningMethod::Legacy)
                .unwrap();

            // Signature is ignored here, caller must provide a valid
            // script_sig/witness.

            let claim =
                SpendingScriptBuilder::custom_script_sig_witness(Some(script_sig), Some(witness));

            Ok((utxo, arg, claim))
        },
        Proto::mod_Input::OneOfto_recipient::None => todo!(),
    }
}

pub fn proto_sighash_to_sig(
    private_key: &[u8],
    ctx: &UtxoProto::Sighash,
    no_aux_rand: bool,
) -> Result<Vec<u8>> {
    match ctx.signing_method {
        UtxoProto::SigningMethod::Legacy | UtxoProto::SigningMethod::Segwit => {
            let privkey = PrivateKey::try_from(private_key).unwrap();
            let sighash: H256 = ctx.sighash.as_ref().try_into().unwrap();
            let sig = privkey.sign(sighash).unwrap().to_vec();
            Ok(sig)
        },
        UtxoProto::SigningMethod::TaprootAll | UtxoProto::SigningMethod::TaprootOnePrevout => {
            let privkey = schnorr::PrivateKey::try_from(private_key).unwrap();
            let privkey = if no_aux_rand {
                privkey.no_aux_rand()
            } else {
                privkey
            };

            let sighash: H256 = ctx.sighash.as_ref().try_into().unwrap();
            let sig = privkey.sign(sighash).unwrap().to_vec();
            Ok(sig)
        },
    }
}

pub fn proto_output_to_native(
    output: &Proto::Output,
) -> Result<(
    TransactionOutput,
    Proto::mod_PreSigningOutput::TxOut<'static>,
)> {
    match &output.to_recipient {
        Proto::mod_Output::OneOfto_recipient::builder(b) => match &b.variant {
            mod_Output::mod_OutputBuilder::OneOfvariant::p2sh(payload) => match &payload.variant {
                OneOfOutputVariant::hash(hash) => {
                    let redeem_hash: H160 = hash.as_ref().try_into().unwrap();

                    let out = OutputBuilder::new(output.value as i64).p2sh_from_hash(&redeem_hash);

                    let tx_out = Proto::mod_PreSigningOutput::TxOut {
                        value: output.value,
                        script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                        ..Default::default()
                    };

                    Ok((out, tx_out))
                },
                OneOfOutputVariant::redeem_script(redeem_script) => {
                    let out = OutputBuilder::new(output.value as i64).p2sh(redeem_script.as_ref());

                    let tx_out = Proto::mod_PreSigningOutput::TxOut {
                        value: output.value,
                        script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                        ..Default::default()
                    };

                    Ok((out, tx_out))
                },
                OneOfOutputVariant::None => todo!(),
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2pkh(payload) => {
                match &payload.to_address {
                    OneOfto_address::pubkey(pubkey) => {
                        let pubkey = pubkey_from_raw(pubkey).unwrap();

                        let out = OutputBuilder::new(output.value as i64).p2pkh(&pubkey);

                        let tx_out = Proto::mod_PreSigningOutput::TxOut {
                            value: output.value,
                            script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                            ..Default::default()
                        };

                        Ok((out, tx_out))
                    },
                    OneOfto_address::hash(hash) => {
                        let pubkey_hash = hash.as_ref().try_into().unwrap();

                        let out =
                            OutputBuilder::new(output.value as i64).p2pkh_from_hash(&pubkey_hash);

                        let tx_out = Proto::mod_PreSigningOutput::TxOut {
                            value: output.value,
                            script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                            ..Default::default()
                        };

                        Ok((out, tx_out))
                    },
                    OneOfto_address::None => todo!(),
                }
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2wsh(_) => todo!(),
            mod_Output::mod_OutputBuilder::OneOfvariant::p2wpkh(payload) => {
                match &payload.to_address {
                    OneOfto_address::pubkey(pubkey) => {
                        let pubkey = pubkey_from_raw(pubkey).unwrap();

                        let out = OutputBuilder::new(output.value as i64).p2wpkh(&pubkey);

                        let tx_out = Proto::mod_PreSigningOutput::TxOut {
                            value: output.value,
                            script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                            ..Default::default()
                        };

                        Ok((out, tx_out))
                    },
                    OneOfto_address::hash(hash) => {
                        let pubkey_hash = hash.as_ref().try_into().unwrap();

                        let out =
                            OutputBuilder::new(output.value as i64).p2wpkh_from_hash(&pubkey_hash);

                        let tx_out = Proto::mod_PreSigningOutput::TxOut {
                            value: output.value,
                            script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                            ..Default::default()
                        };

                        Ok((out, tx_out))
                    },
                    OneOfto_address::None => todo!(),
                }
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_key_path(pubkey) => {
                let pubkey = pubkey_from_raw(pubkey).unwrap();

                let out = OutputBuilder::new(output.value as i64).p2tr_key_path(&pubkey);

                let tx_out = Proto::mod_PreSigningOutput::TxOut {
                    value: output.value,
                    script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                    ..Default::default()
                };

                Ok((out, tx_out))
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_script_path(payload) => {
                let pubkey = pubkey_from_raw(&payload.internal_key).unwrap();
                let merkle_root: H256 = payload.merkle_root.as_ref().try_into().unwrap();

                let out =
                    OutputBuilder::new(output.value as i64).p2tr_script_path(pubkey, merkle_root);

                let tx_out = Proto::mod_PreSigningOutput::TxOut {
                    value: output.value,
                    script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                    ..Default::default()
                };

                Ok((out, tx_out))
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_dangerous_assume_tweaked(_) => {
                todo!()
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::brc20_inscribe(payload) => {
                let pubkey = pubkey_from_raw(&payload.inscribe_to).unwrap();
                let ticker = payload.ticker.to_string();
                let value = payload.transfer_amount.to_string();

                let out = OutputBuilder::new(output.value as i64)
                    .brc20_transfer(pubkey, ticker, value)
                    .unwrap();

                let tx_out = Proto::mod_PreSigningOutput::TxOut {
                    value: output.value,
                    script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                    ..Default::default()
                };

                Ok((out, tx_out))
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::ordinal_inscribe(_) => todo!(),
            mod_Output::mod_OutputBuilder::OneOfvariant::None => todo!(),
        },
        Proto::mod_Output::OneOfto_recipient::custom_script_pubkey(script_pubkey) => {
            let out = OutputBuilder::new(output.value as i64)
                .custom_script_pubkey(script_pubkey.to_vec().into());

            let tx_out = Proto::mod_PreSigningOutput::TxOut {
                value: output.value,
                script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                ..Default::default()
            };

            Ok((out, tx_out))
        },
        Proto::mod_Output::OneOfto_recipient::from_address(addr) => {
            address_to_native(addr, output.value)
        },
        Proto::mod_Output::OneOfto_recipient::None => todo!(),
    }
}

fn address_to_native(
    addr: &str,
    value: u64,
) -> Result<(
    TransactionOutput,
    Proto::mod_PreSigningOutput::TxOut<'static>,
)> {
    let addr = StandardBitcoinAddress::from_str(addr).unwrap();
    match addr {
        StandardBitcoinAddress::Legacy(addr) => {
            let payload = addr.bytes();
            debug_assert_eq!(payload.len(), 21);

            match payload[0] {
                // P2PKH
                0 => {
                    let pubkey_hash: H160 = payload[1..].try_into().unwrap();

                    let out = OutputBuilder::new(value as i64).p2pkh_from_hash(&pubkey_hash);

                    let tx_out = Proto::mod_PreSigningOutput::TxOut {
                        value,
                        script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                        ..Default::default()
                    };

                    Ok((out, tx_out))
                },
                // P2SH
                5 => {
                    let pubkey_hash: H160 = payload[1..].try_into().unwrap();

                    let out = OutputBuilder::new(value as i64).p2sh_from_hash(&pubkey_hash);

                    let tx_out = Proto::mod_PreSigningOutput::TxOut {
                        value,
                        script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                        ..Default::default()
                    };

                    Ok((out, tx_out))
                },
                _ => todo!(),
            }
        },
        StandardBitcoinAddress::Segwit(addr) => {
            let prog = addr.witness_program();

            match prog.len() {
                // P2WPKH
                20 => {
                    let pubkey_hash: H160 = prog.try_into().unwrap();

                    let out = OutputBuilder::new(value as i64).p2wpkh_from_hash(&pubkey_hash);

                    let tx_out = Proto::mod_PreSigningOutput::TxOut {
                        value,
                        script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                        ..Default::default()
                    };

                    Ok((out, tx_out))
                },
                // P2WSH
                32 => {
                    let redeem_hash: H256 = prog.try_into().unwrap();

                    let out = OutputBuilder::new(value as i64).p2wsh_from_hash(&redeem_hash);

                    let tx_out = Proto::mod_PreSigningOutput::TxOut {
                        value,
                        script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                        ..Default::default()
                    };

                    Ok((out, tx_out))
                },
                // Invalid
                _ => todo!(),
            }
        },
        StandardBitcoinAddress::Taproot(addr) => {
            let tweaked_pubkey: H256 = addr.witness_program().try_into().unwrap();
            debug_assert_eq!(tweaked_pubkey.len(), 32);

            let out =
                OutputBuilder::new(value as i64).p2tr_dangerous_assume_tweaked(&tweaked_pubkey);

            let tx_out = Proto::mod_PreSigningOutput::TxOut {
                value,
                script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                ..Default::default()
            };

            Ok((out, tx_out))
        },
    }
}
