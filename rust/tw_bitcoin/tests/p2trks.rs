use std::str::FromStr;

use bitcoin::{Network, PubkeyHash, PublicKey, Txid};
use tw_bitcoin::{
    keypair_from_wif, Recipient, TransactionBuilder, TxInputP2PKH, TxOutputP2TKeyPath,
};
use tw_encoding::hex;

mod common;
use common::*;

#[test]
fn sign_input_p2pkh_output_p2tr_key_path() {
    // This passed the `bitcoin-cli -retest testmempoolaccept` command.
    const EXPECTED_RAW_SIGNED: &str = "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a47304402206cd6484a6f9199d0f8ae1696bff915f70ddee994349e3aa474e048dc8ad4604002206a7a562d28dfd663cb30b54039a791af65acdcfac38c3e212518256fa3e21d3b0121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000002251202842b4039775688d0c12c6a97a72fa618f1eda45241c665437ff53709442e31300000000";

    const FULL_AMOUNT: u64 = ONE_BTC * 50;
    const MINER_FEE: u64 = ONE_BTC / 100;
    const SEND_AMOUNT: u64 = FULL_AMOUNT - MINER_FEE;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Prepare inputs for Alice.
    let txid = Txid::from_str(GENESIS_TXID).unwrap();
    let vout = 0;
    // TODO: this can be done nicer
    let recipient = Recipient::<PublicKey>::from_keypair(&alice, bitcoin::Network::Regtest);
    let satoshis = FULL_AMOUNT;

    let input = TxInputP2PKH::new(txid, vout, recipient, Some(satoshis));

    // Prepare outputs for Bob
    let recipient = Recipient::<PublicKey>::from_keypair(&bob, bitcoin::Network::Regtest);
    let satoshis = SEND_AMOUNT;

    let output = TxOutputP2TKeyPath::new(satoshis, &recipient);

    let signed_transaction = TransactionBuilder::new(Network::Regtest)
        // TODO: Set return address, decrease miner fee.
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
