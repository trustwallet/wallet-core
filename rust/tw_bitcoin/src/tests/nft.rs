use crate::nft::OrdinalNftInscription;
use crate::{
    keypair_from_wif, TXOutputP2TRScriptPath, TransactionBuilder, TxInputP2TRScriptPath,
    TxInputP2WPKH, TxOutputP2WPKH,
};
use bitcoin::Txid;
use std::str::FromStr;
use tw_encoding::hex;

pub const ALICE_WIF: &str = "L4of5AJ6aKmvChg7gQ7m2RzHFgpWe5Uirmuey1fXJ1FtfmXj59LW";
pub const FULL_SATOSHIS: u64 = 32_400;
pub const INSCRIBE_SATOSHIS: u64 = FULL_SATOSHIS - MINER_FEE;
pub const DUST_SATOSHIS: u64 = 546;

pub const MINER_FEE: u64 = 1_300;

pub const COMMIT_TXID: &str = "579590c3227253ad423b1e7e3c5b073b8a280d307c68aecd779df2600daa2f99";
pub const COMMIT_RAW_TX: &str = "02000000000101992faa0d60f29d77cdae687c300d288a3b075b3c7e1e3b42ad537222c39095570000000000ffffffff017c790000000000002251202ac69a7e9dba801e9fcba826055917b84ca6fba4d51a29e47d478de603eedab602473044022054212984443ed4c66fc103d825bfd2da7baf2ab65d286e3c629b36b98cd7debd022050214cfe5d3b12a17aaaf1a196bfeb2f0ad15ffb320c4717eb7614162453e4fe0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000";

pub const REVEAL_TXID: &str = "f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117";
pub const REVEAL_RAW_TX: &str = super::data::NFT_INSCRIPTION_RAW_HEX;

#[test]
fn inscribe_nft() {
    let alice = keypair_from_wif(ALICE_WIF).unwrap();

    let payload = hex::decode(super::data::NFT_INSCRIPTION_IMAGE_DATA).unwrap();
    let nft_inscription = OrdinalNftInscription::new(b"image/png", &payload, alice.into()).unwrap();

    let txid = Txid::from_str(COMMIT_TXID).unwrap();

    // Commit NFT.
    let input = TxInputP2WPKH::builder()
        .txid(txid)
        .vout(0)
        .recipient(alice.try_into().unwrap())
        .satoshis(FULL_SATOSHIS)
        .build()
        .unwrap();

    let output = TXOutputP2TRScriptPath::builder()
        .recipient(nft_inscription.inscription().recipient().clone())
        .satoshis(INSCRIBE_SATOSHIS)
        .build()
        .unwrap();

    let transaction = TransactionBuilder::new()
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(&transaction, false);
    assert_eq!(hex, COMMIT_RAW_TX);

    // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117

    let txid = Txid::from_str(REVEAL_TXID).unwrap();

    // Reveal NFT.
    let input = TxInputP2TRScriptPath::builder()
        .txid(txid)
        .vout(0)
        .recipient(nft_inscription.inscription().recipient().clone())
        .satoshis(INSCRIBE_SATOSHIS)
        .script(nft_inscription.inscription().taproot_program().to_owned())
        .spend_info(nft_inscription.inscription().spend_info().clone())
        .build()
        .unwrap();

    let output = TxOutputP2WPKH::builder()
        .recipient(alice.try_into().unwrap())
        .satoshis(DUST_SATOSHIS)
        .build()
        .unwrap();

    let transaction = TransactionBuilder::new()
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(&transaction, false);
    assert_eq!(hex[..164], REVEAL_RAW_TX[..164]);
    // We ignore the 64-byte Schnorr signature, since it uses random data for
    // signing on each construction and is therefore not reproducible.
    assert_ne!(hex[164..292], REVEAL_RAW_TX[164..292]);
    assert_eq!(hex[292..], REVEAL_RAW_TX[292..]);

    // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/173f8350b722243d44cc8db5584de76b432eb6d0888d9e66e662db51584f44ac
}
