use super::*;
use crate::{keypair_from_wif, TxInputP2PKH, TxOutputP2PKH};
use crate::ffi::{tw_transaction_builder_create, tw_transaction_add_p2pkh_input, tw_transaction_add_p2pkh_output, tw_transaction_sign, TWTxOutputP2PKH, TWTxInputP2PKH};
use bitcoin::Txid;
use std::str::FromStr;

pub const ALICE_WIF: &str = "cQUNzeMnF9xPPLqZhH7hMVYGwSuu3b78zznuc5UrxgXnYQBq6Bx1";
pub const BOB_WIF: &str = "cTk5wSci88FPka7JwHpNEA82dUMjAysdDbCiuYB2fegfgGESAZVn";
// Mined block, value 50 BTC, vout: 0
pub const TXID: &str = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";

#[test]
fn build_ffi_p2pkh() {
    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    const FULL_AMOUNT: u64 = ONE_BTC * 50;
    const MINER_FEE: u64 = ONE_BTC / 100;
    const SEND_AMOUNT: u64 = FULL_AMOUNT - MINER_FEE;

    let input = TxInputP2PKH::builder()
        .txid(Txid::from_str(TXID).unwrap())
        .vout(0)
        .recipient(alice)
        .satoshis(FULL_AMOUNT)
        .build()
        .unwrap();

	let input= Box::new(TWTxInputP2PKH(input));

    // Prepare outputs for Bob.
    let output = TxOutputP2PKH::builder()
        .satoshis(SEND_AMOUNT)
        .recipient(bob)
        .build()
        .unwrap();

	let output = Box::new(TWTxOutputP2PKH(output));

	let privkey = alice.secret_bytes();

	unsafe {
		let builder = tw_transaction_builder_create();
		let builder = tw_transaction_add_p2pkh_input(builder, Box::into_raw(input));
		let builder = tw_transaction_add_p2pkh_output(builder, Box::into_raw(output));
		let sig = tw_transaction_sign(builder, privkey.as_ptr(), privkey.len()).unwrap();

		let sig_bytes = sig.into_vec();
		dbg!(sig_bytes);
	}
}
