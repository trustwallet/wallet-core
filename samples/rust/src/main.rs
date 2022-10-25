// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod walletcore_iface;
pub mod walletcore_extra;
pub mod wc_proto;

use crate::walletcore_iface::*;
use crate::walletcore_extra::*;
use crate::wc_proto::Ethereum;
use protobuf::Message;
use hex::ToHex;

// returns private key
fn get_wallet_info(coin: u32, coin_name: &str, wallet: &HDWallet) -> TWData {
    println!("coin {}:", coin_name);
    let address = hd_wallet_get_address_for_coin(wallet, coin);
    println!("  address: {}", address.to_string());

    let priv_key = hd_wallet_get_key_for_coin(wallet, coin);
    let priv_key_data = private_key_data(&priv_key);
    println!("  privkey: {}", priv_key_data.to_hex());

    let pub_key = private_key_get_public_key_secp256k1(&priv_key, true);
    let pub_key_data = public_key_data(&pub_key);
    println!("  pubkey:  {}", pub_key_data.to_hex());

    return priv_key_data;
}

fn main() {
    println!("=== Calling wallet-core from Rust");

    let mnemonic = "confirm bleak useless tail chalk destroy horn step bulb genuine attract split";
    println!("mnemonic is valid: {}", mnemonic_is_valid(&TWString::from_str(mnemonic)));

    let mnemonic_tw = TWString::from_str(mnemonic);
    if !mnemonic_is_valid(&mnemonic_tw) {
        println!("Mnemonic is invalid! {}", mnemonic);
        return;
    }
    println!("wallet created, mnemonic {}", mnemonic);
    let wallet = hd_wallet_create_with_mnemonic(&mnemonic_tw, &TWString::from_str(""));

    get_wallet_info(0, "bitcoin", &wallet);

    let eth_pk = get_wallet_info(60, "ethereum", &wallet);

    // Ethereum transaction
    println!("Signing Ethereum transaction");
    let mut signing_input = Ethereum::SigningInput::new();
    signing_input.chain_id = vec!(4);
    signing_input.nonce = vec!(0);
    signing_input.tx_mode = ::protobuf::EnumOrUnknown::new(Ethereum::TransactionMode::Legacy);
    signing_input.gas_price = hex::decode("00174876E800").unwrap(); // 100000000000 100 gwei
    signing_input.gas_limit = hex::decode("005208").unwrap(); // 21000
    signing_input.to_address = "0xE9B511C0753649E5F3E78Ed8AdBEE92d0d2Db384".to_string();
    signing_input.private_key = eth_pk.to_vec();
    let mut transfer = Ethereum::transaction::Transfer::new();
    transfer.amount = hex::decode("002386F26FC10000").unwrap(); // 10000000000000000 0.01
    let mut transaction = Ethereum::Transaction::new();
    transaction.transaction_oneof = Some(Ethereum::transaction::Transaction_oneof::Transfer(transfer));
    signing_input.transaction = ::protobuf::MessageField::some(transaction);

    let input_ser = signing_input.write_to_bytes().unwrap();
    let input_ser_data = TWData::from_vec(&input_ser);

    let output_ser_data = any_signer_sign(&input_ser_data, 60);

    let outputp: Ethereum::SigningOutput = protobuf::Message::parse_from_bytes(&output_ser_data.to_vec()).unwrap();
    let x: Vec<u8> = outputp.encoded;
    println!("  signed tx: {}", x.encode_hex::<String>());
}
