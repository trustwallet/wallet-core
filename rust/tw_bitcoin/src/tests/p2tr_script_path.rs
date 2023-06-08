use super::*;
use crate::{
    brc20::{self, BRC20DeployInscription, BRC20DeployPayload, BRC20MintInscription},
    keypair_from_wif, Recipient, TXOutputP2TRScriptPath, TaprootScript, TransactionBuilder,
    TxInputP2PKH, TxInputP2TRKeyPath, TxInputP2TRScriptPath, TxInputP2WPKH, TxOutput,
    TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH,
};
use bitcoin::{key::TweakedPublicKey, PubkeyHash, PublicKey, Txid, WPubkeyHash};
use std::{i128::MIN, str::FromStr};
use tw_encoding::hex;

#[test]
fn mainnet_brc20_deploy() {
    const EXPECTED_RAW_DEPLOY: &str = "020000000121a9074b8d5c9f0c57528bbe503e96a06bb5530253afcde740357a0aac11a5c4000000006b48304502210094f38d41dd28f1916d06b49747276cf418ba002efa4ff151a4167ed070a7e90b022020c6edc13f4a9e1222ee9e09b99ca205139ccf5439a65abb34d62ce694bafdc4012103c934b8fa66a55b0a1517e8b443140c844b43682d75a4260a040cd8d301d11019ffffffff022602000000000000225120971bffe440f17f394e728dfd499d0d35fbab4fd17d0ca307eb4bc6909eaed6a6825f0000000000001976a9147542c8ab655580ef2a3d1d93153030c73467f8ba88ac00000000";

    let ALICE_WIF: &str = env!("ALICE_WIF");
    let ORDINALS_SATOSHIS: u64 = env!("ORDINALS_SATOSHIS").parse().unwrap();
    let MINER_FEE: u64 = env!("MINER_FEE").parse().unwrap();

    let ORIGIN_TXID: &str = env!("ORIGIN_TXID");
    let DEPLOY_INPUT_AMOUNT: u64 = env!("DEPLOY_INPUT_AMOUNT").parse().unwrap();
    let DEPLOY_RETURN_AMOUNT: u64 = DEPLOY_INPUT_AMOUNT - ORDINALS_SATOSHIS - MINER_FEE;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();

    // ## FIRST TRANSACTION: Deploy the BRC20 token.

    // Claim input.
    let txid = Txid::from_str(ORIGIN_TXID).unwrap();
    let vout = 0;
    let recipient = Recipient::<PubkeyHash>::from_keypair(&alice);
    let satoshis = DEPLOY_INPUT_AMOUNT;

    let input = TxInputP2PKH::new(txid, vout, recipient, Some(satoshis));

    // Prepare BRC20 Deploy output for Alice.
    let recipient = Recipient::<PublicKey>::from_keypair(&alice);
    let ticker = "qqqq".to_string();
    let max = 1000;
    let limit = None;
    let decimals = None;

    let brc20_deploy =
        BRC20DeployInscription::new(recipient.clone(), ticker.clone(), max, limit, decimals)
            .unwrap();
    //dbg!(&brc20_deploy);

    let output = TXOutputP2TRScriptPath::new(ORDINALS_SATOSHIS, brc20_deploy.0.recipient());

    // Return address (back to Alice)
    let satoshis = DEPLOY_RETURN_AMOUNT;

    let return_output = TxOutputP2PKH::new(satoshis, recipient.clone());

    // Alice signs the transaction.
    let signed_transaction = TransactionBuilder::new(bitcoin::Network::Regtest)
        .miner_fee(MINER_FEE)
        .add_input(input.into())
        .add_output(output.into())
        .add_output(return_output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(signed_transaction, false);
    assert_eq!(&hex, EXPECTED_RAW_DEPLOY);
    println!("{hex}");

    // ## SECOND TRANSACTION: Deploy the BRC20 token.
    let input_deploy;
    let input_fee_payer;
    let output_mint;
    let output_return;
    {
        let DEPLOY_TXID: &str = env!("DEPLOY_TXID");
        //let MINT_INPUT_AMOUNT: u64 = env!("MINT_INPUT_AMOUNT").parse().unwrap();
        let DEPLOY_RETURN_AMOUNT: u64 = env!("DEPLOY_RETURN_AMOUNT").parse().unwrap();
        let MINT_RETURN_AMOUNT: u64 = DEPLOY_RETURN_AMOUNT - MINER_FEE;
        dbg!(DEPLOY_RETURN_AMOUNT, MINER_FEE, MINT_RETURN_AMOUNT);

        // Claim BRC20 DEPLOY input.
        let txid = Txid::from_str(DEPLOY_TXID).unwrap();
        let vout = 0;
        let satoshis = ORDINALS_SATOSHIS;

        // Input of BRC20 DEPLOY
        input_deploy = TxInputP2TRScriptPath::new(
            txid,
            vout,
            brc20_deploy.0.recipient().clone(),
            satoshis,
            brc20_deploy.0.envelope.script,
            brc20_deploy.0.envelope.spend_info,
        );

        // Input for paying fees.
        let vout = 1;
        let satoshis = DEPLOY_RETURN_AMOUNT;
        input_fee_payer = TxInputP2PKH::new(txid, vout, recipient.clone(), Some(satoshis));

        // Prepare BRC20 MINT output.

        // Mint 50 tokens of "qqqq".
        let amount = 50;

        let brc20_mint =
            BRC20MintInscription::new(recipient.clone(), ticker.clone(), amount).unwrap();

        output_mint = TXOutputP2TRScriptPath::new(ORDINALS_SATOSHIS, brc20_mint.0.recipient());

        // Return address (back to Alice)
        let satoshis = MINT_RETURN_AMOUNT;
        output_return = TxOutputP2PKH::new(satoshis, recipient.clone());
    };

    // Alice signs the transaction.
    let signed_transaction = TransactionBuilder::new(bitcoin::Network::Regtest)
        .miner_fee(MINER_FEE)
        .add_input(input_deploy.into())
        .add_input(input_fee_payer.into())
        .add_output(output_mint.into())
        .add_output(output_return.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(signed_transaction, false);
    //assert_eq!(&hex, EXPECTED_RAW_SIGNED);
    println!("{hex}");
}
