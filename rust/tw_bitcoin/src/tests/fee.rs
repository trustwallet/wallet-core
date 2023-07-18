use crate::fee::calculate_fee;
use bitcoin::{consensus::Decodable, Transaction};

// 10 satoshis per virtual byte.
const SAT_VB: u64 = 12;

fn decode_tx(raw: &str) -> Transaction {
    let hex = tw_encoding::hex::decode(raw).unwrap();
    Transaction::consensus_decode(&mut hex.as_slice()).unwrap()
}

#[test]
fn p2pkh_fee() {
    let tx = decode_tx(super::p2pkh::TX_RAW);

    let (weight, fee) = calculate_fee(&tx, SAT_VB);
    assert_eq!(weight.to_vbytes_ceil(), 191);
    assert_eq!(fee, 191 * SAT_VB);
}

#[test]
fn p2wpkh_fee() {
    let tx = decode_tx(super::p2wpkh::TX_RAW);

    let (weight, fee) = calculate_fee(&tx, SAT_VB);
    assert_eq!(weight.to_vbytes_ceil(), 189);
    assert_eq!(fee, 189 * SAT_VB);
}

#[test]
fn brc20_commit_fee() {
    // Metadata can be observed live on:
    // https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
    //
    // Fee/VB	19.608 sat/vByte
    // Size		235 Bytes
    // Weight	610

    // 19 satoshis per vbyte.
    const SAT_19_VB: u64 = 19;

    let tx = decode_tx(super::brc20_transfer::COMMIT_TX_RAW);

    let (weight, fee) = calculate_fee(&tx, SAT_19_VB);
    assert_eq!(weight.to_vbytes_ceil(), 153); // 153 = ceil(610/4)
    assert_eq!(fee, 153 * SAT_19_VB);
}

#[test]
fn brc20_reveal_fee() {
    // Metadata can be observed live on:
    // https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca
    //
    // Fee/VB	49.267 sat/vByte
    // Size		276 Bytes
    // Weight	522

    // 49 satoshis per vbyte (slightly overpaid here...)
    const SAT_49_VB: u64 = 49;

    let tx = decode_tx(super::brc20_transfer::REVEAL_RAW);

    let (weight, fee) = calculate_fee(&tx, SAT_49_VB);
    assert_eq!(weight.to_vbytes_ceil(), 131); // 131 = ceil(522/4)
    assert_eq!(fee, 131 * SAT_49_VB);
}
