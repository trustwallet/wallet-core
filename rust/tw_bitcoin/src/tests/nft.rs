use crate::nft::{ImageType, NftInscription};
use crate::{
    brc20::{BRC20TransferInscription, Ticker},
    keypair_from_wif, TXOutputP2TRScriptPath, TransactionBuilder, TxInputP2TRScriptPath,
    TxInputP2WPKH, TxOutputP2WPKH,
};
use bitcoin::Txid;
use tw_encoding::hex;
use std::str::FromStr;
use std::fs::File;
use std::io::prelude::*;

pub const ALICE_WIF: &str = "L4of5AJ6aKmvChg7gQ7m2RzHFgpWe5Uirmuey1fXJ1FtfmXj59LW";
pub const FULL_SATOSHIS: u64 = 32_400;
pub const INSCRIBE_SATOSHIS: u64 = FULL_SATOSHIS - MINER_FEE;
pub const DUST_SATOSHIS: u64 = 546;

pub const MINER_FEE: u64 = 1_300;

pub const COMMIT_TXID: &str = "579590c3227253ad423b1e7e3c5b073b8a280d307c68aecd779df2600daa2f99";
pub const REVEAL_TXID: &str = "f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117";

#[test]
fn inscribe_nft() {
	let alice = keypair_from_wif(ALICE_WIF).unwrap();

	let mut file = File::open("./src/tests/data/tw_logo.png").unwrap();
	let mut content = vec![];
	file.read_to_end(&mut content).unwrap();

	let nft_inscription = NftInscription::new_image(&content, ImageType::ImagePng, alice.into()).unwrap();

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
	println!("{hex}");

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

	println!(">>>> ");

    let hex = hex::encode(&transaction, false);
	println!("{hex}");
}
