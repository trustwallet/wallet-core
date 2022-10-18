pub mod walletcore;
pub mod proto;

use crate::walletcore::*;
use crate::proto::Ethereum::transaction;

fn test_address(coin: i32, coin_name: &str, wallet: &HDWallet) {
    let address = hd_wallet_get_address_for_coin(wallet, coin);
    println!("==> addr for {}: {}", coin_name, address.to_string());
}

fn main() {
	println!("==> Calling wallet-core from Rust");

    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
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
