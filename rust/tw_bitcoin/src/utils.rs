use crate::{Error, Result};
use tw_hash::H256;
use tw_keypair::ecdsa::signature::Signature;
use tw_keypair::schnorr;
use tw_keypair::tw::PublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto::mod_Input::mod_InputBuilder::OneOfvariant;
use tw_proto::BitcoinV2::Proto::mod_ToPublicKeyOrHash::OneOfto_address;
use tw_proto::BitcoinV2::Proto::{self, mod_Output};
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

pub fn proto_input_to_native(
    input: &Proto::Input,
) -> Result<(TransactionInput, UtxoToSign, SpendingData)> {
    match &input.to_recipient {
        Proto::mod_Input::OneOfto_recipient::builder(b) => {
            match &b.variant {
                OneOfvariant::p2sh(_) => todo!(),
                OneOfvariant::p2pkh(pubkey) => {
                    let pubkey = pubkey_from_raw(&pubkey).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        .p2pkh(pubkey.clone())
                        .unwrap();

                    let sig = Signature::from_bytes(&vec![1; 65]).unwrap();
                    let claim = SpendingScriptBuilder::new().p2pkh(sig, pubkey).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::p2wsh(_) => todo!(),
                OneOfvariant::p2wpkh(pubkey) => {
                    let pubkey = pubkey_from_raw(&pubkey).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2wpkh(pubkey.clone())
                        .unwrap();

                    let sig = Signature::from_bytes(&vec![1; 65]).unwrap();
                    let claim = SpendingScriptBuilder::new().p2wpkh(sig, pubkey).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::p2tr_key_path(ctx) => {
                    // TODO: Rename field to `pubkey`?
                    let pubkey = pubkey_from_raw(&ctx.public_key).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2tr_key_path(pubkey)
                        .unwrap();

                    let sig = schnorr::Signature::from_bytes(&vec![1; 64]).unwrap();
                    let claim = SpendingScriptBuilder::new().p2tr_key_path(sig).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::p2tr_script_path(ctx) => {
                    // TODO:
                    let one_prevout = ctx.one_prevout;
                    let payload = Script::from(ctx.payload.to_vec());
                    let control_block = ctx.control_block.to_vec();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64)
                        .p2tr_script_path(payload.clone())
                        .unwrap();

                    let sig = schnorr::Signature::from_bytes(&vec![1; 64]).unwrap();
                    let claim = SpendingScriptBuilder::new()
                        .p2tr_script_path(sig, payload, control_block)
                        .unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::brc20_inscribe(payload) => {
                    // TODO: Rename field to `pubkey`?
                    let pubkey = pubkey_from_raw(&payload.inscribe_to).unwrap();
                    let ticker = payload.ticker.to_string();
                    let value = payload.transfer_amount.to_string();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        .brc20_transfer(pubkey.clone(), ticker.clone(), value.clone())
                        .unwrap();

                    let sig = schnorr::Signature::from_bytes(&vec![1; 64]).unwrap();
                    let claim = SpendingScriptBuilder::new()
                        .brc20_transfer(sig, pubkey, ticker, value)
                        .unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::ordinal_inscribe(_) => todo!(),
                OneOfvariant::None => todo!(),
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

            let claim =
                SpendingScriptBuilder::custom_script_sig_witness(Some(script_sig), Some(witness));

            Ok((utxo, arg, claim))
        },
        Proto::mod_Input::OneOfto_recipient::None => todo!(),
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
            mod_Output::mod_OutputBuilder::OneOfvariant::p2sh(_) => todo!(),
            mod_Output::mod_OutputBuilder::OneOfvariant::p2pkh(payload) => {
                match &payload.to_address {
                    OneOfto_address::pubkey(pubkey) => {
                        let pubkey = pubkey_from_raw(&pubkey).unwrap();

                        let out = OutputBuilder::new()
                            .amount(output.value as i64)
                            .p2pkh(pubkey)
                            .unwrap();

                        let tx_out = Proto::mod_PreSigningOutput::TxOut {
                            value: output.value,
                            script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                            ..Default::default()
                        };

                        Ok((out, tx_out))
                    },
                    OneOfto_address::hash(_) => todo!(),
                    OneOfto_address::None => todo!(),
                }
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2wsh(_) => todo!(),
            mod_Output::mod_OutputBuilder::OneOfvariant::p2wpkh(payload) => {
                match &payload.to_address {
                    OneOfto_address::pubkey(pubkey) => {
                        let pubkey = pubkey_from_raw(&pubkey).unwrap();

                        let out = OutputBuilder::new()
                            .amount(output.value as i64)
                            .p2wpkh(pubkey)
                            .unwrap();

                        let tx_out = Proto::mod_PreSigningOutput::TxOut {
                            value: output.value,
                            script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                            ..Default::default()
                        };

                        Ok((out, tx_out))
                    },
                    OneOfto_address::hash(_) => todo!(),
                    OneOfto_address::None => todo!(),
                }
            },
            mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_key_path(pubkey) => {
                let pubkey = pubkey_from_raw(&pubkey).unwrap();

                let out = OutputBuilder::new()
                    .amount(output.value as i64)
                    .p2tr_key_path(pubkey)
                    .unwrap();

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

                let out = OutputBuilder::new()
                    .amount(output.value as i64)
                    .p2tr_script_path(pubkey, merkle_root)
                    .unwrap();

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

                let out = OutputBuilder::new()
                    .amount(output.value as i64)
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
            let out = OutputBuilder::new()
                .amount(output.value as i64)
                .custom_script_pubkey(script_pubkey.to_vec().into())
                .unwrap();

            let tx_out = Proto::mod_PreSigningOutput::TxOut {
                value: output.value,
                script_pubkey: out.script_pubkey.as_data().to_vec().into(),
                ..Default::default()
            };

            Ok((out, tx_out))
        },
        Proto::mod_Output::OneOfto_recipient::from_address(addr) => {
            todo!()
        },
        Proto::mod_Output::OneOfto_recipient::None => todo!(),
    }
}
