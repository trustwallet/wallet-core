// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod walletcore;
pub mod wc_proto;

use crate::walletcore::*;
use crate::wc_proto::Ethereum::transaction;

fn test_address(coin: i32, coin_name: &str, wallet: &HDWallet) {
    println!("==> coin {}:", coin_name);
    let address = hd_wallet_get_address_for_coin(wallet, coin);
    println!("==>   address: {}", address.to_string());

    let priv_key = hd_wallet_get_key_for_coin(wallet, coin);
    let priv_key_data = private_key_data(&priv_key);
    println!("==>   privkey: {}", priv_key_data.to_hex());

    let pub_key = private_key_get_public_key_secp256k1(&priv_key, true);
    let pub_key_data = public_key_data(&pub_key);
    println!("==>   pubkey:  {}", pub_key_data.to_hex());
}

fn main() {
	println!("==> Calling wallet-core from Rust");

    let mnemonic = "confirm bleak useless tail chalk destroy horn step bulb genuine attract split";
    println!("==> mnemonic is valid: {}", mnemonic_is_valid(&TWString::from_str(mnemonic)));

    let mnemonic_tw = TWString::from_str(mnemonic);
    if !mnemonic_is_valid(&mnemonic_tw) {
        println!("Mnemonic is invalid! {}", mnemonic);
        return;
    }
    println!("==> wallet created, mnemonic {}", mnemonic);
    let wallet = hd_wallet_create_with_mnemonic(&mnemonic_tw, &TWString::from_str(""));

    test_address(0, "bitcoin", &wallet);

    test_address(60, "ethereum", &wallet);

    // Ethereum transaction
    let _transfer = transaction::Transfer::new();
    // TODO
}
