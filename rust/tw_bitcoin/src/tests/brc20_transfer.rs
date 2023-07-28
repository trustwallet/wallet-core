use crate::{
    brc20::{BRC20TransferInscription, Ticker},
    keypair_from_wif, TXOutputP2TRScriptPath, TransactionBuilder, TxInputP2TRScriptPath,
    TxInputP2WPKH, TxOutputP2WPKH,
};
use bitcoin::Txid;
use std::str::FromStr;
use tw_encoding::hex;

// Those private keys were used for Bitcoin mainnet tests and have a transaction
// history. BTC holdings have been emptied.
pub const ALICE_WIF: &str = "L4of5AJ6aKmvChg7gQ7m2RzHFgpWe5Uirmuey1fXJ1FtfmXj59LW";
pub const BOB_WIF: &str = "L59WHi2hj1HnMAYaFyMqR4Z36HrUDTZQCixzTHachAxbUU9VUCjp";

pub const FULL_SATOSHIS: u64 = 26_400;
pub const MINER_FEE: u64 = 3_000;

pub const BRC20_TICKER: &str = "oadf";
pub const BRC20_AMOUNT: u64 = 20;
pub const BRC20_INSCRIBE_SATOSHIS: u64 = 7_000;
pub const BRC20_DUST_SATOSHIS: u64 = 546;

pub const FOR_FEE_SATOSHIS: u64 = FULL_SATOSHIS - BRC20_INSCRIBE_SATOSHIS - MINER_FEE;

// Used for the committing the Inscription.
// https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
pub const COMMIT_TXID: &str = "8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008";
pub const COMMIT_TX_RAW: &str = "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000";

// Used for revealing the Inscription.
// https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca
pub const REVEAL_TXID: &str = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
pub const REVEAL_TX_RAW: &str = "02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d0340de6fd13e43700f59876d305e5a4a5c41ad7ada10bc5a4e4bdd779eb0060c0a78ebae9c33daf77bb3725172edb5bd12e26f00c08f9263e480d53b93818138ad0b5b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000";

// Used for transfering the Inscription ("BRC20 transfer").
// https://www.blockchain.com/explorer/transactions/btc/3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7
pub use skip::*;
// We skip formatting for the `skip` module, re-exporting everything.
#[rustfmt::skip]
mod skip {
pub const TRANSFER_TXID_INSCRIPTION: &str = "7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca";
pub const TRANSFER_TXID_FOR_FEES: &str = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
pub const TRANSFER_RAW: &str = "02000000000102ca3edda74a46877efa5364ab85947e148508713910ada23e147ea28926dc46700000000000ffffffffb11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790100000000ffffffff022202000000000000160014e891850afc55b64aa8247b2076f8894ebdf889015834000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d024830450221008798393eb0b7390217591a8c33abe18dd2f7ea7009766e0d833edeaec63f2ec302200cf876ff52e68dbaf108a3f6da250713a9b04949a8f1dcd1fb867b24052236950121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb0248304502210096bbb9d1f0596d69875646689e46f29485e8ceccacde9d0025db87fd96d3066902206d6de2dd69d965d28df3441b94c76e812384ab9297e69afe3480ee4031e1b2060121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000";
}

#[test]
fn brc20_transfer() {
    let ticker = Ticker::new(BRC20_TICKER.to_string()).unwrap();

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    let txid = Txid::from_str(COMMIT_TXID).unwrap();

    // # Make "available" tokens "transferable".
    // Based on Bitcoin transaction:
    // https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1

    // Commit transfer.
    let input = TxInputP2WPKH::builder()
        .txid(txid)
        .vout(1)
        .recipient(alice.try_into().unwrap())
        .satoshis(FULL_SATOSHIS)
        .build()
        .unwrap();

    let transfer = BRC20TransferInscription::new(alice.into(), ticker, BRC20_AMOUNT).unwrap();

    let output = TXOutputP2TRScriptPath::builder()
        .recipient(transfer.inscription().recipient().clone())
        .satoshis(BRC20_INSCRIBE_SATOSHIS)
        .build()
        .unwrap();

    let output_change = TxOutputP2WPKH::builder()
        .recipient(alice.try_into().unwrap())
        .satoshis(FOR_FEE_SATOSHIS)
        .build()
        .unwrap();

    let transaction = TransactionBuilder::new()
        .add_input(input.into())
        .add_output(output.into())
        .add_output(output_change.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    // Encode the signed transaction.
    let hex = hex::encode(&transaction, false);
    assert_eq!(hex, COMMIT_TX_RAW);

    // # Reveal transfer.
    // Based on Bitcoin transaction:
    // https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca

    let txid =
        Txid::from_str("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1").unwrap();

    let input = TxInputP2TRScriptPath::builder()
        .txid(txid)
        .vout(0)
        .recipient(transfer.inscription().recipient().clone())
        .satoshis(BRC20_INSCRIBE_SATOSHIS)
        .script(transfer.inscription().taproot_program().to_owned())
        .spend_info(transfer.inscription().spend_info().clone())
        .build()
        .unwrap();

    let output = TxOutputP2WPKH::builder()
        .recipient(alice.try_into().unwrap())
        .satoshis(BRC20_DUST_SATOSHIS)
        .build()
        .unwrap();

    let transaction = TransactionBuilder::new()
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    // Encode the signed transaction.
    let hex = hex::encode(&transaction, false);

    assert_eq!(hex[..164], REVEAL_TX_RAW[..164]);
    // We ignore the 64-byte Schnorr signature, since it uses random data for
    // signing on each construction and is therefore not reproducible.
    assert_ne!(hex[164..292], REVEAL_TX_RAW[164..292]);
    assert_eq!(hex[292..], REVEAL_TX_RAW[292..]);

    // # Actually transfer the "transferable" tokens.
    // Based on Bitcoin transaction:
    // https://www.blockchain.com/explorer/transactions/btc/3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7

    // We use a normal P2WPKH output for this.
    let input_for_brc20_transfer = TxInputP2WPKH::builder()
        .txid(Txid::from_str(TRANSFER_TXID_INSCRIPTION).unwrap())
        .vout(0)
        .recipient(alice.try_into().unwrap())
        .satoshis(BRC20_DUST_SATOSHIS)
        .build()
        .unwrap();

    let input_for_fee = TxInputP2WPKH::builder()
        .txid(Txid::from_str(TRANSFER_TXID_FOR_FEES).unwrap())
        .vout(1)
        .recipient(alice.try_into().unwrap())
        .satoshis(FOR_FEE_SATOSHIS)
        .build()
        .unwrap();

    // We transfer the tokens to Bob.
    let output_brc20_transfer = TxOutputP2WPKH::builder()
        .recipient(bob.try_into().unwrap())
        .satoshis(BRC20_DUST_SATOSHIS)
        .build()
        .unwrap();

    let output_change = TxOutputP2WPKH::builder()
        .recipient(alice.try_into().unwrap())
        .satoshis(FOR_FEE_SATOSHIS - MINER_FEE)
        .build()
        .unwrap();

    // We carefully add the BRC20 transfer in the first position for both input and output.
    let transaction = TransactionBuilder::new()
        .add_input(input_for_brc20_transfer.into())
        .add_input(input_for_fee.into())
        .add_output(output_brc20_transfer.into())
        .add_output(output_change.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    // Encode the signed transaction.
    let hex = hex::encode(&transaction, false);
    assert_eq!(hex, TRANSFER_RAW);
}
