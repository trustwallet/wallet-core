// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;

/// Deposit 0.0001 BTC from bc1q2sphzvc2uqmxqte2w9dd4gzy4sy9vvfv0me9ke to 0xa8491D40d4F71A752cA41DA0516AEd80c33a1B56 on ZETA mainnet.
/// https://www.zetachain.com/docs/developers/omnichain/bitcoin/#example-1-deposit-btc-into-an-account-in-zevm
///
/// Original test: https://github.com/trustwallet/wallet-core/blob/55ca3e561d6c0815a5601a51fd66ffc679061660/tests/chains/Bitcoin/TWBitcoinSigningTests.cpp#L888-L962
/// *Note* that if we enable change output, current selection algorithm will calculate
/// different change and fee amounts than in the original test.
#[test]
fn test_bitcoin_deposit_to_zetachain() {
    let my_private_key = "428d66be0b5a620f126a00fa67637222ce3dc9badfe5c605189520760810cfac"
        .decode_hex()
        .unwrap();
    let my_address = "bc1q2sphzvc2uqmxqte2w9dd4gzy4sy9vvfv0me9ke";
    let my_evm_zetachain_address = "0xa8491D40d4F71A752cA41DA0516AEd80c33a1B56";

    // https://www.zetachain.com/docs/reference/glossary/#tss
    let zeta_tss_address = "bc1qm24wp577nk8aacckv8np465z3dvmu7ry45el6y";

    let utxo_hash_0 = "17a6adb5db1e33c87467a58aa31cddbb3800052315015cf3cf1c2b0119310e20";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 0),
        value: 20_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(my_address),
        ..Default::default()
    };

    let out_0 = Proto::Output {
        value: 10_000,
        to_recipient: output::to_address(zeta_tss_address),
    };
    // OP_RETURN must be the second output after the actual amount and before the change.
    let out_1 = Proto::Output {
        value: 0,
        // Pass my ZetaChain EVM address bytes (20) as the OP_RETURN.
        to_recipient: output::op_return(my_evm_zetachain_address.decode_hex().unwrap()),
    };
    let explicit_change_output = Proto::Output {
        value: 7_420,
        to_recipient: output::to_address(my_address),
    };

    let builder = Proto::TransactionBuilder {
        inputs: vec![utxo_0],
        outputs: vec![out_0, out_1, explicit_change_output],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        fee_per_vb: 1,
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![my_private_key.into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // Successfully broadcasted:
    // https://mempool.space/tx/2b871b6c1112ad0a777f6db1f7a7709154c4d9af8e771ba4eca148915f830e9d
    // https://explorer.zetachain.com/cc/tx/0x269e319478f8849247abb28b33a7b8e0a849dab4551bab328bf58bf67b02a807
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "01000000000101200e3119012b1ccff35c011523050038bbdd1ca38aa56774c8331edbb5ada6170000000000ffffffff031027000000000000160014daaae0d3de9d8fdee31661e61aea828b59be78640000000000000000166a14a8491d40d4f71a752ca41da0516aed80c33a1b56fc1c000000000000160014540371330ae036602f2a715adaa044ac0856312c02483045022100e29731f7474f9103c6df3434c8c62a540a21ad0e10e23df343b1e81e4b26110602202d37fb4fee5341a41f9e4e65ba2d3e0d2309425ea9806d94eb268efe6f21007001210369cdaf80b4a5fdad91e9face90e848225512884ec2e3ed572ca11dc68e75054700000000",
            txid: "2b871b6c1112ad0a777f6db1f7a7709154c4d9af8e771ba4eca148915f830e9d",
            inputs: vec![20_000],
            outputs: vec![10_000, 0, 7_420],
            vsize: 172,
            weight: 686,
            // sum(inputs) - sum(outputs)
            fee: 2580,
        });
}
