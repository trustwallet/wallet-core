// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Binance::Proto;
use tw_proto::Binance::Proto::mod_SigningInput::OneOforder_oneof as OrderEnum;

const ACCOUNT_12_PRIVATE_KEY: &str =
    "90335b9d2153ad1a9799a3ccc070bd64b4164e9642ee1dd48053c33f9a3a05e9";

#[test]
fn test_binance_sign() {
    // bnb1hgm0p7khfk85zpz5v0j8wnej3a90w709vhkdfu
    let sender_key_hash = "ba36f0fad74d8f41045463e4774f328f4af779e5"
        .decode_hex()
        .unwrap();

    let new_order = Proto::TradeOrder {
        sender: sender_key_hash.into(),
        id: "BA36F0FAD74D8F41045463E4774F328F4AF779E5-36".into(),
        symbol: "NNB-338_BNB".into(),
        ordertype: 2,
        side: 1,
        price: 136350000,
        quantity: 100000000,
        timeinforce: 1,
    };

    let input = Proto::SigningInput {
        chain_id: "chain-bnb".into(),
        account_number: 12,
        sequence: 35,
        source: 1,
        private_key: ACCOUNT_12_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::trade_order(new_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    assert_eq!(
        output.encoded.to_hex(),
        "dc01f0625dee0a64ce6dc0430a14ba36f0fad74d8f41045463e4774f328f4af779e5122b424133364630464144373444384634313034353436334534373734463332384634414637373945352d33361a0b4e4e422d3333385f424e422002280130b09282413880c2d72f4001126e0a26eb5ae98721029729a52e4e3c2b4a4e52aa74033eedaf8ba1df5ab6d1f518fd69e67bbd309b0e12409123cb6906bb20aeb753f4a121d4d88ff0e9750ba75b0c4e10d76caee1e7d2481290fa3b9887a6225d6997f5f939ef834ea61d596a314237c48e560da9e17b5a180c20232001"
    );
}
