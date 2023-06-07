use super::*;
use crate::{keypair_from_wif, Recipient, TransactionBuilder, TxInputP2PKH, TxOutputP2PKH};
use bitcoin::{PubkeyHash, PublicKey, Txid};
use std::str::FromStr;
use tw_encoding::hex;

pub const ALICE_WIF: &str = "cQUNzeMnF9xPPLqZhH7hMVYGwSuu3b78zznuc5UrxgXnYQBq6Bx1";
pub const BOB_WIF: &str = "cTk5wSci88FPka7JwHpNEA82dUMjAysdDbCiuYB2fegfgGESAZVn";
pub const GENESIS_BLOCK: &str = "0b3b41749c70214385724a4064d87fb445f5aef57ff047cd4ac5b167ced513c7";
// Mined block, value 50 BTC, vout: 0
pub const GENESIS_TXID: &str = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";

#[test]
fn sign_input_p2pkh_output_p2pkh() {
    // This passed the `bitcoin-cli -retest testmempoolaccept` command.
    const EXPECTED_RAW_SIGNED: &str = "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000";

    const FULL_AMOUNT: u64 = ONE_BTC * 50;
    const MINER_FEE: u64 = ONE_BTC / 100;
    const SEND_AMOUNT: u64 = FULL_AMOUNT - MINER_FEE;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Prepare inputs for Alice.
    let txid = Txid::from_str(GENESIS_TXID).unwrap();
    let vout = 0;
    let recipient = Recipient::<PublicKey>::from_keypair(&alice);
    let satoshis = FULL_AMOUNT;

    let input = TxInputP2PKH::new(txid, vout, recipient, Some(satoshis));

    // Prepare outputs for Bob.
    let recipient = Recipient::<PublicKey>::from_keypair(&bob);
    let satoshis = SEND_AMOUNT;

    let output = TxOutputP2PKH::new(satoshis, recipient);

    // Alice signs the transaction.
    let signed_transaction = TransactionBuilder::new(bitcoin::Network::Regtest)
        .miner_fee(MINER_FEE)
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(signed_transaction, false);
    assert_eq!(&hex, EXPECTED_RAW_SIGNED);
}
