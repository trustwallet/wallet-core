// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::ton::cell_example::{
    comment_cell, contract_address_from_state_init, doge_chatbot_state_init,
};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TheOpenNetwork::Proto::mod_Transfer::OneOfpayload as PayloadType;

/// The same Cell can be BoC encoded differently.
/// Use this function to compare inner Cells closing eyes on the encoding.
#[track_caller]
pub(crate) fn assert_eq_boc(left: &str, right: &str) {
    use tw_ton_sdk::boc::BagOfCells;

    let left_boc = BagOfCells::parse_base64(left).unwrap();
    let right_boc = BagOfCells::parse_base64(right).unwrap();

    assert_eq!(left_boc, right_boc);
}

#[test]
fn test_ton_sign_transfer_and_deploy() {
    let private_key = "63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8";

    let transfer = Proto::Transfer {
        dest: "EQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRR3n0".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        expire_at: 1671135440,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/6ZzWOFKZt_m3kZjbwfbATwLaVwmUOdDp0xjhuY7PO3k=
    assert_eq_boc(&output.encoded, "te6ccgICABoAAQAAA8sAAAJFiADN98eLgHfrkE8l8gmT8X5REpTVR6QnqDhArTbKlVvbZh4ABAABAZznxvGBhoRXhPogxNY8QmHlihJWxg5t6KptqcAIZlVks1r+Z+r1avCWNCeqeLC/oaiVN4mDx/E1+Zhi33G25rcIKamjF/////8AAAAAAAMAAgFiYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4hQAAAAAAAAAAAAAAAAAQADAAACATQABgAFAFEAAAAAKamjF/Qsd/kxvqIOxdAVBzEna7suKGCUdmEkWyMZ74Ez7o1BQAEU/wD0pBP0vPLICwAHAgEgAA0ACAT48oMI1xgg0x/TH9MfAvgju/Jk7UTQ0x/TH9P/9ATRUUO68qFRUbryogX5AVQQZPkQ8qP4ACSkyMsfUkDLH1Iwy/9SEPQAye1U+A8B0wchwACfbFGTINdKltMH1AL7AOgw4CHAAeMAIcAC4wABwAORMOMNA6TIyx8Syx/L/wAMAAsACgAJAAr0AMntVABsgQEI1xj6ANM/MFIkgQEI9Fnyp4IQZHN0cnB0gBjIywXLAlAFzxZQA/oCE8tqyx8Syz/Jc/sAAHCBAQjXGPoA0z/IVCBHgQEI9FHyp4IQbm90ZXB0gBjIywXLAlAGzxZQBPoCFMtqEssfyz/Jc/sAAgBu0gf6ANTUIvkABcjKBxXL/8nQd3SAGMjLBcsCIs8WUAX6AhTLaxLMzMlz+wDIQBSBAQj0UfKnAgIBSAAXAA4CASAAEAAPAFm9JCtvaiaECAoGuQ+gIYRw1AgIR6STfSmRDOaQPp/5g3gSgBt4EBSJhxWfMYQCASAAEgARABG4yX7UTQ1wsfgCAVgAFgATAgEgABUAFAAZrx32omhAEGuQ64WPwAAZrc52omhAIGuQ64X/wAA9sp37UTQgQFA1yH0BDACyMoHy//J0AGBAQj0Cm+hMYALm0AHQ0wMhcbCSXwTgItdJwSCSXwTgAtMfIYIQcGx1Z70ighBkc3RyvbCSXwXgA/pAMCD6RAHIygfL/8nQ7UTQgQFA1yH0BDBcgQEI9ApvoTGzkl8H4AXTP8glghBwbHVnupI4MOMNA4IQZHN0crqSXwbjDQAZABgAilAEgQEI9Fkw7UTQgQFA1yDIAc8W9ADJ7VQBcrCOI4IQZHN0coMesXCAGFAFywVQA88WI/oCE8tqyx/LP8mAQPsAkl8D4gB4AfoA9AQw+CdvIjBQCqEhvvLgUIIQcGx1Z4MesXCAGFAEywUmzxZY+gIZ9ADLaRfLH1Jgyz8gyYBA+wAG");
    assert_eq!(
        output.hash.to_hex(),
        "b3d9462c13a8c67e19b62002447839c386de51415ace3ff6473b1e6294299819"
    );
}

#[test]
fn test_ton_sign_transfer_and_deploy_4b1d9f() {
    // UQApdOSfRTScoB9bMNGeo3bn-DGpQD8gywA27UaZVvAQsrHg
    let private_key = "e97620499dfee0107c0cd7f0ecb2afb3323d385b3a82320a5e3fa1fbdca6e722";

    let transfer = Proto::Transfer {
        dest: "UQA6whN_oU5h9jPljnlDSWRYQNDPkLaUqqaEWULNB_Zoykuu".into(),
        // 0.0001 TON
        amount: U256::encode_be_compact(100_000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        expire_at: 1721892371,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/4b1d9f09856af70ea1058b557a87c9ba2abb0bca2029e0cbbe8c659d5dae4ce1
    assert_eq_boc(&output.encoded, "te6cckECGgEAA7QAAkWIAFLpyT6KaTlAPrZhoz1G7c/wY1KAfkGWAG3ajTKt4CFkHgECAgE0AwQBnPhb8QZRko0VcLtSmZsGLICPVXj7+QmBkrCrCL1R1bdHYWibC86XsAcA2dgem8QSl8xxsChuCd2oVG1FoMVUUgEpqaMX/////wAAAAAAAwUBFP8A9KQT9LzyyAsGAFEAAAAAKamjFyoNBiYB1ihabNBrDwYVwXUbvpGlthGturFziw2Dl/PLQAFmYgAdYQm/0Kcw+xnyxzyhpLIsIGhnyFtKVVNCLKFmg/s0ZRgMNQAAAAAAAAAAAAAAAAABBwIBIAgJAAACAUgKCwT48oMI1xgg0x/TH9MfAvgju/Jk7UTQ0x/TH9P/9ATRUUO68qFRUbryogX5AVQQZPkQ8qP4ACSkyMsfUkDLH1Iwy/9SEPQAye1U+A8B0wchwACfbFGTINdKltMH1AL7AOgw4CHAAeMAIcAC4wABwAORMOMNA6TIyx8Syx/L/wwNDg8C5tAB0NMDIXGwkl8E4CLXScEgkl8E4ALTHyGCEHBsdWe9IoIQZHN0cr2wkl8F4AP6QDAg+kQByMoHy//J0O1E0IEBQNch9AQwXIEBCPQKb6Exs5JfB+AF0z/IJYIQcGx1Z7qSODDjDQOCEGRzdHK6kl8G4w0QEQIBIBITAG7SB/oA1NQi+QAFyMoHFcv/ydB3dIAYyMsFywIizxZQBfoCFMtrEszMyXP7AMhAFIEBCPRR8qcCAHCBAQjXGPoA0z/IVCBHgQEI9FHyp4IQbm90ZXB0gBjIywXLAlAGzxZQBPoCFMtqEssfyz/Jc/sAAgBsgQEI1xj6ANM/MFIkgQEI9Fnyp4IQZHN0cnB0gBjIywXLAlAFzxZQA/oCE8tqyx8Syz/Jc/sAAAr0AMntVAB4AfoA9AQw+CdvIjBQCqEhvvLgUIIQcGx1Z4MesXCAGFAEywUmzxZY+gIZ9ADLaRfLH1Jgyz8gyYBA+wAGAIpQBIEBCPRZMO1E0IEBQNcgyAHPFvQAye1UAXKwjiOCEGRzdHKDHrFwgBhQBcsFUAPPFiP6AhPLassfyz/JgED7AJJfA+ICASAUFQBZvSQrb2omhAgKBrkPoCGEcNQICEekk30pkQzmkD6f+YN4EoAbeBAUiYcVnzGEAgFYFhcAEbjJftRNDXCx+AA9sp37UTQgQFA1yH0BDACyMoHy//J0AGBAQj0Cm+hMYAIBIBgZABmtznaiaEAga5Drhf/AABmvHfaiaEAQa5DrhY/AaTiNAg==");
    assert_eq!(
        output.hash.to_hex(),
        "4b1d9f09856af70ea1058b557a87c9ba2abb0bca2029e0cbbe8c659d5dae4ce1"
    );
}

#[test]
fn test_ton_sign_transfer_ordinary() {
    let private_key = "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0";

    let transfer = Proto::Transfer {
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 6,
        expire_at: 1671132440,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
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
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: U256::encode_be_compact(0),
        mode: Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 7,
        expire_at: 1681102222,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
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
        dest: "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV".into(),
        amount: U256::encode_be_compact(0),
        mode: Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: false,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 8,
        expire_at: 1681102222,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
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
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        comment: "test comment".into(),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 10,
        expire_at: 1681102222,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
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
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        comment: "тестовый комментарий".into(),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 11,
        expire_at: 1681102222,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
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
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        expire_at: 1671135440,
        wallet_version: Proto::WalletVersion::WALLET_V3_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::Error_not_supported);
}

#[test]
fn test_ton_sign_transfer_jettons() {
    let private_key = "c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee";

    let jetton_transfer = Proto::JettonTransfer {
        query_id: 69,
        // Transfer 1 testtwt (decimal precision is 9).
        jetton_amount: U256::encode_be_compact(1000 * 1000 * 1000),
        to_owner: "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8".into(),
        // Send unused toncoins back to sender.
        response_address: "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk".into(),
        forward_amount: U256::encode_be_compact(1),
        ..Proto::JettonTransfer::default()
    };

    let transfer = Proto::Transfer {
        dest: "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja".into(),
        amount: U256::encode_be_compact(100 * 1000 * 1000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        payload: PayloadType::jetton_transfer(jetton_transfer),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 0,
        expire_at: 1787693046,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://testnet.tonviewer.com/transaction/2HOPGAXhez3v6sdfj-5p8mPHX4S4T0CgxVbm0E2swxE=
    assert_eq_boc(&output.encoded, "te6ccgICABoAAQAABCMAAAJFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKh4ABAABAZz3iNHD1z2mxbtpFAtmbVevYMnB4yHPkF3WAsL3KHcrqCw0SWezOg4lVz1zzSReeFDx98ByAqY9+eR5VF3xyugAKamjF/////8AAAAAAAMAAgFoYgAxNB+Hnam4Pt4pSYNuGp+1rhx1QxEXrrZTGnfPOq6D8yAvrwgAAAAAAAAAAAAAAAAAAQADAKoPin6lAAAAAAAAAEVDuaygCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAgE0AAYABQBRAAAAACmpoxfOamBhePRNnx/pqQViBzW0dDCy/+1WLV1VhgbVTL6i30ABFP8A9KQT9LzyyAsABwIBIAANAAgE+PKDCNcYINMf0x/THwL4I7vyZO1E0NMf0x/T//QE0VFDuvKhUVG68qIF+QFUEGT5EPKj+AAkpMjLH1JAyx9SMMv/UhD0AMntVPgPAdMHIcAAn2xRkyDXSpbTB9QC+wDoMOAhwAHjACHAAuMAAcADkTDjDQOkyMsfEssfy/8ADAALAAoACQAK9ADJ7VQAbIEBCNcY+gDTPzBSJIEBCPRZ8qeCEGRzdHJwdIAYyMsFywJQBc8WUAP6AhPLassfEss/yXP7AABwgQEI1xj6ANM/yFQgR4EBCPRR8qeCEG5vdGVwdIAYyMsFywJQBs8WUAT6AhTLahLLH8s/yXP7AAIAbtIH+gDU1CL5AAXIygcVy//J0Hd0gBjIywXLAiLPFlAF+gIUy2sSzMzJc/sAyEAUgQEI9FHypwICAUgAFwAOAgEgABAADwBZvSQrb2omhAgKBrkPoCGEcNQICEekk30pkQzmkD6f+YN4EoAbeBAUiYcVnzGEAgEgABIAEQARuMl+1E0NcLH4AgFYABYAEwIBIAAVABQAGa8d9qJoQBBrkOuFj8AAGa3OdqJoQCBrkOuF/8AAPbKd+1E0IEBQNch9AQwAsjKB8v/ydABgQEI9ApvoTGAC5tAB0NMDIXGwkl8E4CLXScEgkl8E4ALTHyGCEHBsdWe9IoIQZHN0cr2wkl8F4AP6QDAg+kQByMoHy//J0O1E0IEBQNch9AQwXIEBCPQKb6Exs5JfB+AF0z/IJYIQcGx1Z7qSODDjDQOCEGRzdHK6kl8G4w0AGQAYAIpQBIEBCPRZMO1E0IEBQNcgyAHPFvQAye1UAXKwjiOCEGRzdHKDHrFwgBhQBcsFUAPPFiP6AhPLassfyz/JgED7AJJfA+IAeAH6APQEMPgnbyIwUAqhIb7y4FCCEHBsdWeDHrFwgBhQBMsFJs8WWPoCGfQAy2kXyx9SYMs/IMmAQPsABg==");
    assert_eq!(
        output.hash.to_hex(),
        "3e4dac37acdc99ca670b3747ab2730e818727d9d25c80d3987abe501356d0da0"
    );
}

#[test]
fn test_ton_sign_transfer_jettons_with_comment() {
    let private_key = "c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee";

    let jetton_transfer = Proto::JettonTransfer {
        query_id: 0,
        // Transfer 0.5 testtwt (decimal precision is 9).
        jetton_amount: U256::encode_be_compact(500 * 1000 * 1000),
        to_owner: "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8".into(),
        // Send unused toncoins back to sender.
        response_address: "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk".into(),
        forward_amount: U256::encode_be_compact(1),
        ..Proto::JettonTransfer::default()
    };

    let transfer = Proto::Transfer {
        dest: "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja".into(),
        amount: U256::encode_be_compact(100 * 1000 * 1000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        comment: "test comment".into(),
        payload: PayloadType::jetton_transfer(jetton_transfer),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 1,
        expire_at: 1787693046,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://testnet.tonviewer.com/transaction/12bfe84f947740aec3faa54f04a50690900e3aae9ac9596cfa6804a61a48f429
    assert_eq_boc(&output.encoded, "te6ccgICAAQAAQAAARgAAAFFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKgwAAQGcaIWVosi1XnveAmoG9y0/mPeNUqUu7GY76mdbRAaVeNeDOPDlh5M3BEb26kkc6XoYDekV60o2iOobN+TGS76jBSmpoxdqjgf2AAAAAQADAAIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEAAwDKD4p+pQAAAAAAAAAAQdzWUAgAC4GWcwteHQM+mcQoV+aZ+myCd1jYqUiawhCzuxMdEzkAFoogyrkCban+t9HUgVPCw1YfEGQ6ktObBRqKfkWPiWVCAgAAAAB0ZXN0IGNvbW1lbnQ=");
    assert_eq!(
        output.hash.to_hex(),
        "c98c205c8dd37d9a6ab5db6162f5b9d37cefa067de24a765154a5eb7a359f22f"
    );
}

#[test]
fn test_ton_sign_transfer_custom_payload() {
    // UQApdOSfRTScoB9bMNGeo3bn-DGpQD8gywA27UaZVvAQsrHg
    let private_key = "e97620499dfee0107c0cd7f0ecb2afb3323d385b3a82320a5e3fa1fbdca6e722";

    let transfer = Proto::Transfer {
        dest: "UQA6whN_oU5h9jPljnlDSWRYQNDPkLaUqqaEWULNB_Zoykuu".into(),
        // 0.00025 TON
        amount: U256::encode_be_compact(250_000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        payload: PayloadType::custom_payload(comment_cell("Hi there sir").into()),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        expire_at: 1721906219,
        messages: vec![transfer],
        sequence_number: 2,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/4ca4442921d0737d518b4863f8eafc2eb26824e9362ebaa9bc59373950b7fa86
    assert_eq_boc(&output.encoded, "te6cckEBBAEAvwABRYgAUunJPoppOUA+tmGjPUbtz/BjUoB+QZYAbdqNMq3gIWQMAQGc981GQ9a8Yr4m2YeIeuuNIWlzdHliyW6MRq3RDs5kgvXJP+iNhdZU7o79DJnm/OKuzWI5FbiNy3SF0fGGBObDDCmpoxdmojQrAAAAAgADAgFmYgAdYQm/0Kcw+xnyxzyhpLIsIGhnyFtKVVNCLKFmg/s0ZRgehIAAAAAAAAAAAAAAAAABAwAgAAAAAEhpIHRoZXJlIHNpcoAlI8E=");
    assert_eq!(
        output.hash.to_hex(),
        "4ca4442921d0737d518b4863f8eafc2eb26824e9362ebaa9bc59373950b7fa86"
    );
}

#[test]
fn test_ton_sign_transfer_custom_payload_with_state_init() {
    // UQD9XBth5b0D9F35h8No4nrF2au8E5H9XUc25iJMOx9tLfy3
    let private_key = "5525e673087587bc0efd7ab09920ef7d3c1bf6b854a661430244ca59ab19e9d1";

    let current_timestamp = 1721939114;
    let expire_at = current_timestamp + 60 * 10;

    let doge_state_init = doge_chatbot_state_init(current_timestamp);
    let doge_contract_address = contract_address_from_state_init(&doge_state_init);
    assert_eq!(
        doge_state_init,
        "te6cckEBBAEAUwACATQBAgEU/wD0pBP0vPLICwMAEAAAAZDrkbgQAGrTMAGCCGlJILmRMODQ0wMx+kAwi0ZG9nZYcCCAGMjLBVAEzxaARfoCE8tqEssfAc8WyXP7AO4ioYU="
    );
    assert_eq!(
        doge_contract_address,
        "0:3042cd5480da232d5ac1d9cbe324e3c9eb58f167599f6b7c20c6e638aeed0335",
    );

    let transfer = Proto::Transfer {
        dest: doge_contract_address.into(),
        // 0.069 TON
        amount: U256::encode_be_compact(69_000_000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: false,
        state_init: doge_state_init.into(),
        payload: PayloadType::custom_payload(
            comment_cell("This transaction deploys Doge Chatbot contract").into(),
        ),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        expire_at,
        messages: vec![transfer],
        sequence_number: 4,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/f4b7ed2247b1adf54f33dd2fd99216fbd61beefb281542d0b330ccea9b8d0338
    assert_eq_boc(&output.encoded, "te6cckECCAEAATcAAUWIAfq4NsPLegfou/MPhtHE9YuzV3gnI/q6jm3MRJh2PtpaDAEBnPbyCSsWrOZpEjb7ZFxz5yYi+an6M6Lnq7rI7TFWdDS76LEtGBrVVrhMGziwxuy6LCVtsMBikI7RPVQ89FCIAAYpqaMXZqK3AgAAAAQAAwICaUIAGCFmqkBtEZatYOzl8ZJx5PWseLOsz7W+EGNzHFd2gZqgIObaAAAAAAAAAAAAAAAAAAPAAwQCATQFBgBkAAAAAFRoaXMgdHJhbnNhY3Rpb24gZGVwbG95cyBEb2dlIENoYXRib3QgY29udHJhY3QBFP8A9KQT9LzyyAsHABAAAAGQ65G4EABq0zABgghpSSC5kTDg0NMDMfpAMItGRvZ2WHAggBjIywVQBM8WgEX6AhPLahLLHwHPFslz+wAa2r/S");
    assert_eq!(
        output.hash.to_hex(),
        "724735eea1ab0663aff42af421fb90f7f0c0f7ca3dda424d97c1daf1a21036ad"
    );
}
