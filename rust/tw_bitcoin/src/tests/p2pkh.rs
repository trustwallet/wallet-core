use super::*;
use crate::{keypair_from_wif, TransactionBuilder, TxInputP2PKH, TxOutputP2PKH};
use bitcoin::Txid;
use std::str::FromStr;
use tw_encoding::hex;

// Those private keys were used in a Bitcoin regtest environment.
pub const ALICE_WIF: &str = "cQUNzeMnF9xPPLqZhH7hMVYGwSuu3b78zznuc5UrxgXnYQBq6Bx1";
pub const BOB_WIF: &str = "cTk5wSci88FPka7JwHpNEA82dUMjAysdDbCiuYB2fegfgGESAZVn";
pub const TXID: &str = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";

pub const FULL_SATOSHIS: u64 = ONE_BTC * 50;
pub const MINER_FEE: u64 = ONE_BTC / 100;
pub const SEND_SATOSHIS: u64 = FULL_SATOSHIS - MINER_FEE;

// This passed the `bitcoin-cli -retest testmempoolaccept` command.
pub const TX_RAW: &str = "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000";

#[test]
fn sign_input_p2pkh_output_p2pkh() {
    // This passed the `bitcoin-cli -retest testmempoolaccept` command.

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Prepare inputs for Alice.
    let input = TxInputP2PKH::builder()
        .txid(Txid::from_str(TXID).unwrap())
        .vout(0)
        .recipient(alice)
        .satoshis(FULL_SATOSHIS)
        .build()
        .unwrap();

    // Prepare outputs for Bob.
    let output = TxOutputP2PKH::builder()
        .satoshis(SEND_SATOSHIS)
        .recipient(bob)
        .build()
        .unwrap();

    // Alice signs the transaction.
    let signed_transaction = TransactionBuilder::new()
        .miner_fee(MINER_FEE)
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(&signed_transaction, false);
    assert_eq!(&hex, TX_RAW);
}
