use super::*;
use crate::{
    brc20::{BRC20DeployPayload, BRC20MintInscription},
    keypair_from_wif, Recipient, TXOutputP2TRScriptPath, TransactionBuilder, TxInputP2PKH,
    TxInputP2TRKeyPath, TxInputP2TRScriptPath, TxInputP2WPKH, TxOutputP2PKH, TxOutputP2TRKeyPath,
    TxOutputP2WPKH,
};
use bitcoin::{key::TweakedPublicKey, PubkeyHash, PublicKey, Txid, WPubkeyHash};
use std::str::FromStr;
use tw_encoding::hex;

pub const ALICE_WIF: &str = "";
pub const BOB_WIF: &str = "";
pub const GENESIS_BLOCK: &str = "";
pub const GENESIS_TXID: &str = "";

#[test]
fn sign_input_p2tr_script_path_output_p2tr_script_path() {
    // This passed the `bitcoin-cli -retest testmempoolaccept` command.
    const EXPECTED_RAW_SIGNED: &str = "";

    const COINBASE_AMOUNT: u64 = ONE_BTC * 50;
    const MINER_FEE: u64 = ONE_BTC / 100;
    const SEND_TO_BOB: u64 = COINBASE_AMOUNT - MINER_FEE;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // # First transaction: Alice spends the P2PKH coinbase input and creates
    // # a P2WPKH output for Bob.

    // Prepare inputs for Alice.
    let txid = Txid::from_str(GENESIS_TXID).unwrap();
    let vout = 0;
    let recipient = Recipient::<PubkeyHash>::from_keypair(&alice);
    let satoshis = COINBASE_AMOUNT;

    let input = TxInputP2PKH::new(txid, vout, recipient, Some(satoshis));

    // Prepare BRC20 Deploy output for Bob.
    let recipient = Recipient::<PublicKey>::from_keypair(&alice);
    let ticker = "".to_string();
    let max = 0;
    let limit = None;
    let decimals = None;

    let brc20_deploy = BRC20DeployPayload::new(ticker, max, limit, decimals);
    let inscription = brc20_deploy.to_inscription(recipient).unwrap();

    let output = TXOutputP2TRScriptPath::new(SEND_TO_BOB, inscription.recipient());

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

    // # Second transaction: Bob spends the P2WPKH input and creates
    // # a P2WPKH output for Alice.

    // TODO...
}
