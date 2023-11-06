mod common;

use common::{hex, MINER_FEE, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2tr_key_path() {
    let coin = TestCoinContext::default();

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
        value: ONE_BTC * 50,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2tr_key_path(bob_pubkey.as_slice().into()),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        disable_change_output: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(&encoded, "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000");

    let txid: Vec<u8> = hex("9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 50 - MINER_FEE,
        sighash_type: UtxoProto::SighashType::UseDefault,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2tr_key_path(Proto::mod_Input::InputTaprootKeyPath {
                public_key: bob_pubkey.as_slice().into(),
                one_prevout: false,
            }),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE - MINER_FEE,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2tr_key_path(alice_pubkey.as_slice().into()),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: bob_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        disable_change_output: true,
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(&encoded, "02000000000101ac6058397e18c277e98defda1bc38bdf3ab304563d7df7afed0ca5f63220589a0000000000ffffffff01806de72901000000225120a5c027857e359d19f625e52a106b8ac6ca2d6a8728f6cf2107cd7958ee0787c20140ec2d3910d41506b60aaa20520bb72f15e2d2cbd97e3a8e26ee7bad5f4c56b0f2fb0ceaddac33cb2813a33ba017ba6b1d011bab74a0426f12a2bcf47b4ed5bc8600000000");
}
