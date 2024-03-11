use crate::{Error, Result};
use tw_keypair::ecdsa::signature::Signature;
use tw_keypair::schnorr;
use tw_keypair::tw::PublicKey;
use tw_proto::BitcoinV2::Proto::mod_Input::mod_InputBuilder::OneOfvariant;
use tw_proto::BitcoinV2::Proto::{self, mod_Output};
use tw_proto::Utxo::Proto as UtxoProto;
use tw_utxo::address::standard_bitcoin::{StandardBitcoinAddress, StandardBitcoinPrefix};
use tw_utxo::script::{Script, Witness};
use tw_utxo::sighash_computer::SighashComputer;
use tw_utxo::sighash_computer::{SpendingData, UtxoToSign};
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::standard_transaction::builder::{
    OutputBuilder, SpendingScriptBuilder, TransactionBuilder, UtxoBuilder,
};
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::transaction_fee::TransactionFee;
use tw_utxo::utxo_selector::SelectionBuilder;

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
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        .p2wpkh(pubkey.clone())
                        .unwrap();

                    let sig = Signature::from_bytes(&vec![1; 65]).unwrap();
                    let claim = SpendingScriptBuilder::new().p2wpkh(sig, pubkey).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::p2tr_key_path(payload) => {
                    // TODO: Rename field to `pubkey`?
                    let pubkey = pubkey_from_raw(&payload.public_key).unwrap();

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        .p2tr_key_path(pubkey)
                        .unwrap();

                    let sig = schnorr::Signature::from_bytes(&vec![1; 64]).unwrap();
                    let claim = SpendingScriptBuilder::new().p2tr_key_path(sig).unwrap();

                    Ok((utxo, arg, claim))
                },
                OneOfvariant::p2tr_script_path(_) => todo!(),
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
