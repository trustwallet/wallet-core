use super::*;
use crate::{
    brc20::{self, BRC20DeployInscription, BRC20DeployPayload, BRC20MintInscription},
    keypair_from_wif, Recipient, TXOutputP2TRScriptPath, TaprootScript, TransactionBuilder,
    TxInputP2PKH, TxInputP2TRKeyPath, TxInputP2TRScriptPath, TxInputP2WPKH, TxOutput,
    TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH,
};
use bitcoin::{key::TweakedPublicKey, PubkeyHash, PublicKey, Txid, WPubkeyHash};
use std::str::FromStr;
use tw_encoding::hex;

#[test]
fn mainnet_brc20_deploy() {
    const EXPECTED_RAW_DEPLOY: &str = "020000000121a9074b8d5c9f0c57528bbe503e96a06bb5530253afcde740357a0aac11a5c4000000006a473044022038fe1288c5b3c1fc4e276c80eb130ac81507a1d16a93de61f1065f6fc2878bde02203f513414f6a480e1b4c7a95c762725b9ebc9a83355344e4194f2f8c06b09a31c012103c934b8fa66a55b0a1517e8b443140c844b43682d75a4260a040cd8d301d11019ffffffff022602000000000000225120971bffe440f17f394e728dfd499d0d35fbab4fd17d0ca307eb4bc6909eaed6a60a730000000000001976a9147542c8ab655580ef2a3d1d93153030c73467f8ba88ac00000000";

    let ALICE_WIF: &str = env!("DEPLOY_ALICE_WIF");
    let SEND_AMOUNT: u64 = env!("DEPLOY_SEND_AMOUNT").parse().unwrap();
    let MINER_FEE: u64 = env!("DEPLOY_MINER_FEE").parse().unwrap();

    let DEPLOY_INPUT_TXID: &str = env!("DEPLOY_INPUT_TXID");
    let DEPLOY_INPUT_AMOUNT: u64 = env!("DEPLOY_INPUT_AMOUNT").parse().unwrap();
    let DEPLOY_RETURN_AMOUNT: u64 = DEPLOY_INPUT_AMOUNT - SEND_AMOUNT - MINER_FEE;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();

    // ## FIRST TRANSACTION: Deploy the BRC20 token.

    // Claim input.
    let txid = Txid::from_str(DEPLOY_INPUT_TXID).unwrap();
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

    let output = TXOutputP2TRScriptPath::new(SEND_AMOUNT, brc20_deploy.0.recipient());

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
    let outputs = {
        let MINT_INPUT_TXID: &str = env!("MINT_INPUT_TXID");
        let MINT_INPUT_AMOUNT: u64 = env!("MINT_INPUT_AMOUNT").parse().unwrap();
        let MINT_RETURN_AMOUNT: u64 = MINT_INPUT_AMOUNT - SEND_AMOUNT - MINER_FEE;

        // Claim BRC20 DEPLOY input.
        let txid = Txid::from_str(MINT_INPUT_TXID).unwrap();
        let vout = 0;
        let satoshis = SEND_AMOUNT;

        let input = TxInputP2TRScriptPath::new(
            txid,
            vout,
            brc20_deploy.0.recipient().clone(),
            satoshis,
            brc20_deploy.0.envelope.script,
            brc20_deploy.0.envelope.spend_info,
        );

        // Prepare BRC20 MINT output.

        // Mint 50 tokens of "qqqq".
        let amount = 50;

        let brc20_mint =
            BRC20MintInscription::new(recipient.clone(), ticker.clone(), amount).unwrap();
        //dbg!(&brc20_deploy);

        let output_mint = TXOutputP2TRScriptPath::new(SEND_AMOUNT, brc20_mint.0.recipient());

        // Return address (back to Alice)
        let satoshis = MINT_RETURN_AMOUNT;
        let output_return = TxOutputP2PKH::new(satoshis, recipient.clone());

        [TxOutput::from(output_mint), TxOutput::from(output_return)]
    };

    // Alice signs the transaction.
    /*
    let signed_transaction = TransactionBuilder::new(bitcoin::Network::Regtest)
        .miner_fee(MINER_FEE)
        //.add_input(input.into())
        .add_output(output.into())
        .add_output(return_output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(signed_transaction, false);
    //assert_eq!(&hex, EXPECTED_RAW_SIGNED);
    println!("{hex}");
    */
}
