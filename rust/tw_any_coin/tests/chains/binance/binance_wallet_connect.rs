// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::chains::binance::ACCOUNT_19_PRIVATE_KEY;
use serde_json::json;
use tw_any_coin::test_utils::sign_utils::WalletConnectSignHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_misc::assert_eq_json;
use tw_proto::Common::Proto::SigningError;
use tw_proto::WalletConnect::Proto as WCProto;

const WC_SIGN_REQUEST_CASE_1: &str = include_str!("data/wc_sign_request_case_1.json");
const WC_SIGN_RESPONSE_CASE_1: &str = include_str!("data/wc_sign_response_case_1.json");

#[test]
fn test_binance_sign_wallet_connect_case_1() {
    let input = WCProto::SigningInput {
        method: "cosmos_signAmino".into(),
        private_key: ACCOUNT_19_PRIVATE_KEY.decode_hex().unwrap().into(),
        payload: WC_SIGN_REQUEST_CASE_1.to_string().into(),
    };

    let mut signer = WalletConnectSignHelper::<WCProto::SigningOutput>::default();
    let output = signer.sign_wallet_connect(CoinType::Binance, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq_json!(output.result, WC_SIGN_RESPONSE_CASE_1);
}
