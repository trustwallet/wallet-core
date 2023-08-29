mod common;

use common::hex;
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

use crate::common::ONE_BTC;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2tr_key_path() {
    let coin = EmptyCoinContext;

    let pubkey_one = hex("02c6047f9441ed7d6d3045406e95c07cd85c778e4b8cef3ca7abac09b95c709ee5");
    let pubkey_two = hex("03f210d02e2b7ea0cc8d46e8818aa0b6623ce6b712063233d97e8f7b3f6f2a05d9");
    let pubkey_three = hex("023c6b4d7d8f7b7b1e763e8b6b3738c5241b566096c2127db99c564a537f9e8a3b");

    let redeem_script = hex("522102c6047f9441ed7d6d3045406e95c07cd85c778e4b8cef3ca7abac09b95c709ee52103f210d02e2b7ea0cc8d46e8818aa0b6623ce6b712063233d97e8f7b3f6f2a05d921023c6b4d7d8f7b7b1e763e8b6b3738c5241b566096c2127db99c564a537f9e8a3b53ae");

    let alice_private_key = hex("12ce558df23528f1aa86f1f51ac7e13a197a06bda27610fa89e13b04c40ee999");
    let alice_pubkey = hex("0351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058");
    let bob_private_key = hex("26c2566adcc030a1799213bfd546e615f6ab06f72085ec6806ff1761da48d227");
    let bob_pubkey = hex("02c0938cf377023dfde55e9c96b3cff4ca8894fb6b5d2009006bd43c0bff69cac9");

    let txid: Vec<u8> = hex("c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 2 * ONE_BTC,
        sequence: u32::MAX,
        sequence_enable_zero: false,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2sh(redeem_script.as_slice().into()),
        }),
    };

    let out1 = Proto::Output {
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2sh(Proto::mod_Output::OutputRedeemScriptOrHash {
                variant:
                    Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant::redeem_script(
                        redeem_script.as_slice().into(),
                    ),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        version: 2,
        private_key: alice_private_key.as_slice().into(),
        lock_time: Default::default(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        fee_per_vb: 0,
        change_output: Default::default(),
        disable_change_output: true,
    };

    let output = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(output.encoded, false);
    assert_eq!(&encoded, "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000");
}
