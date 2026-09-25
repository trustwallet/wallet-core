// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::input::out_point;
use crate::chains::common::bitcoin::psbt_plan::BitcoinPsbtPlanHelper;
use crate::chains::common::bitcoin::{btc_info, input, output, transaction_psbt, RecipientType};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ecdsa;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_bitcoin_plan_psbt_thorchain_swap_witness() {
    let private_key = ecdsa::secp256k1::PrivateKey::try_from(
        "f00ffbe44c5c2838c13d2778854ac66b75e04eb6054f0241989e223223ad5e55",
    )
    .unwrap();

    let psbt = "70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d00000000";
    let input = Proto::SigningInput {
        public_keys: vec![private_key.public().compressed().to_vec().into()],
        chain_info: btc_info(),
        transaction: transaction_psbt(psbt),
        ..Proto::SigningInput::default()
    };

    let utxo_0 = Proto::Input {
        out_point: out_point(
            "c6986020440e94beeeaab40fc8fb4314135cc6fe90107c0619f6bc5fdb107014",
            0,
        ),
        value: 66_406,
        sighash_type: 1,
        sequence: input::sequence(u32::MAX),
        claiming_script: input::receiver_address("bc1qkyu3n8k8jmekl3pwvdl59k5w8enjp25akz2r3z"),
    };

    let out_0 = Proto::Output {
        value: 60_000,
        to_recipient: output::to_address("bc1q7g48qdshqd000aysws74pun2uzxrp598gcfum0"),
    };
    let out_1 = Proto::Output {
        value: 0,
        to_recipient: RecipientType::custom_script_pubkey(
            "6a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a3530"
                .decode_hex()
                .unwrap()
                .into()
        ),
    };
    let out_2 = Proto::Output {
        value: 4_670,
        to_recipient: output::to_address("bc1qkyu3n8k8jmekl3pwvdl59k5w8enjp25akz2r3z"),
    };

    let expected = Proto::TransactionPlan {
        inputs: vec![utxo_0],
        outputs: vec![out_0, out_1, out_2],
        available_amount: 66_406,
        send_amount: 66_406,
        vsize_estimate: 216,
        fee_estimate: 1736,
        change: 0,
        ..Proto::TransactionPlan::default()
    };

    BitcoinPsbtPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan_psbt(expected);
}
