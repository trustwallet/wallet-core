// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof as ActionType;

/// The same Cell can be BoC encoded differently.
/// Use this function to compare inner Cells closing eyes on the encoding.
#[track_caller]
fn assert_eq_boc(left: &str, right: &str) {
    use tw_ton_sdk::boc::BagOfCells;

    let left_boc = BagOfCells::parse_base64(left).unwrap();
    let right_boc = BagOfCells::parse_base64(right).unwrap();

    assert_eq!(left_boc, right_boc);
}

#[test]
fn test_ton_sign_transfer_and_deploy() {
    let private_key = "63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        dest: "EQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRR3n0".into(),
        amount: 10,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1671135440,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://tonviewer.com/transaction/6ZzWOFKZt_m3kZjbwfbATwLaVwmUOdDp0xjhuY7PO3k=
    assert_eq_boc(&output.encoded, "te6ccgICABoAAQAAA8sAAAJFiADN98eLgHfrkE8l8gmT8X5REpTVR6QnqDhArTbKlVvbZh4ABAABAZznxvGBhoRXhPogxNY8QmHlihJWxg5t6KptqcAIZlVks1r+Z+r1avCWNCeqeLC/oaiVN4mDx/E1+Zhi33G25rcIKamjF/////8AAAAAAAMAAgFiYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4hQAAAAAAAAAAAAAAAAAQADAAACATQABgAFAFEAAAAAKamjF/Qsd/kxvqIOxdAVBzEna7suKGCUdmEkWyMZ74Ez7o1BQAEU/wD0pBP0vPLICwAHAgEgAA0ACAT48oMI1xgg0x/TH9MfAvgju/Jk7UTQ0x/TH9P/9ATRUUO68qFRUbryogX5AVQQZPkQ8qP4ACSkyMsfUkDLH1Iwy/9SEPQAye1U+A8B0wchwACfbFGTINdKltMH1AL7AOgw4CHAAeMAIcAC4wABwAORMOMNA6TIyx8Syx/L/wAMAAsACgAJAAr0AMntVABsgQEI1xj6ANM/MFIkgQEI9Fnyp4IQZHN0cnB0gBjIywXLAlAFzxZQA/oCE8tqyx8Syz/Jc/sAAHCBAQjXGPoA0z/IVCBHgQEI9FHyp4IQbm90ZXB0gBjIywXLAlAGzxZQBPoCFMtqEssfyz/Jc/sAAgBu0gf6ANTUIvkABcjKBxXL/8nQd3SAGMjLBcsCIs8WUAX6AhTLaxLMzMlz+wDIQBSBAQj0UfKnAgIBSAAXAA4CASAAEAAPAFm9JCtvaiaECAoGuQ+gIYRw1AgIR6STfSmRDOaQPp/5g3gSgBt4EBSJhxWfMYQCASAAEgARABG4yX7UTQ1wsfgCAVgAFgATAgEgABUAFAAZrx32omhAEGuQ64WPwAAZrc52omhAIGuQ64X/wAA9sp37UTQgQFA1yH0BDACyMoHy//J0AGBAQj0Cm+hMYALm0AHQ0wMhcbCSXwTgItdJwSCSXwTgAtMfIYIQcGx1Z70ighBkc3RyvbCSXwXgA/pAMCD6RAHIygfL/8nQ7UTQgQFA1yH0BDBcgQEI9ApvoTGzkl8H4AXTP8glghBwbHVnupI4MOMNA4IQZHN0crqSXwbjDQAZABgAilAEgQEI9Fkw7UTQgQFA1yDIAc8W9ADJ7VQBcrCOI4IQZHN0coMesXCAGFAFywVQA88WI/oCE8tqyx/LP8mAQPsAkl8D4gB4AfoA9AQw+CdvIjBQCqEhvvLgUIIQcGx1Z4MesXCAGFAEywUmzxZY+gIZ9ADLaRfLH1Jgyz8gyYBA+wAG");
    assert_eq!(
        output.hash.to_hex(),
        "b3d9462c13a8c67e19b62002447839c386de51415ace3ff6473b1e6294299819"
    );
}

#[test]
fn test_ton_sign_transfer_ordinary() {
    let private_key = "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: 10,
        sequence_number: 6,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1671132440,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://tonviewer.com/transaction/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
    assert_eq_boc(&output.encoded, "te6ccgICAAQAAQAAALAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAA");
    assert_eq!(
        output.hash.to_hex(),
        "3908cf8b570c1d3d261c62620c9f368db11f6e821a07614cff64de2e7319f81b"
    );
}

#[test]
fn test_ton_sign_transfer_all_balance() {
    let private_key = "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: 0,
        sequence_number: 7,
        mode: Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1681102222,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://tonviewer.com/transaction/cVcXgI9DWNWlN2iyTsteaWJckTswVqWZnRVvX5krXeA=
    assert_eq_boc(&output.encoded, "te6ccgICAAQAAQAAAK8AAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGc58rMUQc/u78bg+Wtt8ETkyM0udf7S+F7wWk7lnPib2KChnBx9dZ7a/zLzhfLq+W9LjLZZfx995J17+0sbkvGCympoxdkM5WOAAAABwCCAAIBYGIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmAAAAAAAAAAAAAAAAAAQADAAA=");
    assert_eq!(
        output.hash.to_hex(),
        "d5c5980c9083f697a7f114426effbbafac6d5c88554297d290eb65c8def3008e"
    );
}

#[test]
fn test_ton_sign_transfer_all_balance_non_bounceable() {
    let private_key = "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        dest: "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV".into(),
        amount: 0,
        sequence_number: 8,
        mode: Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1681102222,
        bounceable: false,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://tonviewer.com/transaction/0sJkPKu6u6uObVRuSWGd_bVGiyy5lJuzEKDqSXifQEA=
    assert_eq_boc(&output.encoded, "te6ccgICAAQAAQAAAK8AAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcRQQvxdU1u4QoE2Pas0AsZQMc9lea3+wtSvaC6QfLUlyJ9oISMCFnaErpyFHelDhPu4iuZqhkoLwjkR1VYhFSCimpoxdkM5WOAAAACACCAAIBYEIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmAAAAAAAAAAAAAAAAAAQADAAA=");
    assert_eq!(
        output.hash.to_hex(),
        "e9c816780fa8e578bae309c2e098db8eb16aa25545b3ad2b61bb711ec9562795"
    );
}

#[test]
fn test_ton_sign_transfer_with_ascii_comment() {
    let private_key = "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: 10,
        sequence_number: 10,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1681102222,
        bounceable: true,
        comment: "test comment".into(),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://tonviewer.com/transaction/9wjD-VrgEDpa0D9u1g03KSD7kvTNsxRocR7LEdQtCNQ=
    assert_eq_boc(&output.encoded, "te6ccgICAAQAAQAAAMAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcY4XlvKqu7spxyjL6vyBSKjbskDgqkHhqBsdTe900RGrzExtpvwc04j94v8HOczEWSMCXjTXk0z+CVUXSL54qCimpoxdkM5WOAAAACgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAgAAAAAHRlc3QgY29tbWVudA==");
    assert_eq!(
        output.hash.to_hex(),
        "a8c6943d5587f590c43fcdb0e894046f1965c615e19bcaf0c8407e9ccb74518d"
    );
}

#[test]
fn test_ton_sign_transfer_with_utf8_comment() {
    let private_key = "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: 10,
        sequence_number: 11,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1681102222,
        bounceable: true,
        comment: "тестовый комментарий".into(),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://tonviewer.com/transaction/9wjD-VrgEDpa0D9u1g03KSD7kvTNsxRocR7LEdQtCNQ=
    assert_eq_boc(&output.encoded, "te6ccgICAAQAAQAAANsAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGchoDa7EdGQuPuehHy3+0X9WNVEvYxdBtaEWn15oYUX8PEKyzztYy94Xq0T2XdhVvj2H7PTSQ+D/Ny1IBRCxk0BimpoxdkM5WOAAAACwADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwBWAAAAANGC0LXRgdGC0L7QstGL0Lkg0LrQvtC80LzQtdC90YLQsNGA0LjQuQ==");
    assert_eq!(
        output.hash.to_hex(),
        "1091dfae81583d3972825633592c24eab0d3d74c91f60fda9d4afe7535103633"
    );
}

#[test]
fn test_ton_sign_transfer_invalid_wallet_version() {
    let private_key = "63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8";

    let transfer = Proto::Transfer {
        wallet_version: Proto::WalletVersion::WALLET_V3_R2,
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: 10,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        expire_at: 1671135440,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        action_oneof: ActionType::transfer(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::Error_not_supported);
}
