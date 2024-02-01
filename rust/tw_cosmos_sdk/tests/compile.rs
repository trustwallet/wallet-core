// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_cosmos_sdk::test_utils::sign_utils::{
    test_compile_json, test_compile_protobuf, TestCompileInput,
};
use tw_encoding::hex::DecodeHex;
use tw_keypair::tw::PublicKeyType;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

#[test]
fn test_compile_with_signatures() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let send_msg = Proto::mod_Message::Send {
        from_address: "cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx".into(),
        to_address: "cosmos18s0hdnsllgcclweu9aymw4ngktr2k0rkygdzdp".into(),
        amounts: vec![make_amount("uatom", "400000")],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        account_number: 546179,
        chain_id: "cosmoshub-4".into(),
        sequence: 0,
        fee: Some(make_fee(200000, make_amount("uatom", "1000"))),
        public_key: "02ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d563649"
            .decode_hex()
            .unwrap()
            .into(),
        messages: vec![make_message(MessageEnum::send_coins_message(send_msg))],
        ..Proto::SigningInput::default()
    };

    test_compile_protobuf::<StandardCosmosContext>(TestCompileInput {
        coin: &coin,
        input: input.clone(),
        tx_preimage: "0a92010a8f010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126f0a2d636f736d6f73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78122d636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a64701a0f0a057561746f6d120634303030303012650a4e0a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a2102ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d56364912040a02080112130a0d0a057561746f6d12043130303010c09a0c1a0b636f736d6f736875622d342083ab21",
        tx_prehash: "fa7990e1814c900efaedf1bdbedba22c22336675befe0ae39974130fc204f3de",
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX"}"#,
        signature: "afbd513a776f4fdf470ef7f9675f21ae9d630fc4d635d8dbaa0dc0a716434cd07e02510765d4673dfa880825bae8e67cb367396ff6b976fc6b19a31fc95e8097",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJ"},"signature":"r71ROndvT99HDvf5Z18hrp1jD8TWNdjbqg3ApxZDTNB+AlEHZdRnPfqICCW66OZ8s2c5b/a5dvxrGaMfyV6Alw=="}]"#,
    });

    test_compile_json::<StandardCosmosContext>(TestCompileInput {
        coin: &coin,
        input: input.clone(),
        tx_preimage: r#"{"account_number":"546179","chain_id":"cosmoshub-4","fee":{"amount":[{"amount":"1000","denom":"uatom"}],"gas":"200000"},"memo":"","msgs":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"400000","denom":"uatom"}],"from_address":"cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx","to_address":"cosmos18s0hdnsllgcclweu9aymw4ngktr2k0rkygdzdp"}}],"sequence":"0"}"#,
        tx_prehash: "0a31f6cd50f1a5c514929ba68a977e222a7df2dc11e8470e93118cc3545e6b37",
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"uatom"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"400000","denom":"uatom"}],"from_address":"cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx","to_address":"cosmos18s0hdnsllgcclweu9aymw4ngktr2k0rkygdzdp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJ"},"signature":"tTyOrburrHEHa14qiw78e9StoZyyGmoku98IxYrWCmtN8Qo5mTeKa0BKKDfgG4LmmNdwYcrXtqQQ7F4dL3c26g=="}]}}"#,
        signature: "b53c8eadbbabac71076b5e2a8b0efc7bd4ada19cb21a6a24bbdf08c58ad60a6b4df10a3999378a6b404a2837e01b82e698d77061cad7b6a410ec5e1d2f7736ea",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJ"},"signature":"tTyOrburrHEHa14qiw78e9StoZyyGmoku98IxYrWCmtN8Qo5mTeKa0BKKDfgG4LmmNdwYcrXtqQQ7F4dL3c26g=="}]"#,
    });
}

#[test]
fn test_compile_with_signatures_direct() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let body_bytes = "0a89010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e120131".decode_hex().unwrap();
    let auth_info_bytes = "0a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180812110a0b0a046d756f6e120332303010c09a0c".decode_hex().unwrap();
    let sign_direct = Proto::mod_Message::SignDirect {
        body_bytes: Cow::from(body_bytes),
        auth_info_bytes: Cow::from(auth_info_bytes),
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        public_key: "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5"
            .decode_hex()
            .unwrap()
            .into(),
        messages: vec![make_message(MessageEnum::sign_direct_message(sign_direct))],
        ..Proto::SigningInput::default()
    };

    // real-world tx: https://www.mintscan.io/cosmos/txs/817101F3D96314AD028733248B28BAFAD535024D7D2C8875D3FE31DC159F096B
    // curl -H 'Content-Type: application/json' --data-binary '{"tx_bytes": "Cr4BCr...1yKOU=", "mode": "BROADCAST_MODE_BLOCK"}' https://api.cosmos.network/cosmos/tx/v1beta1/txs
    // also similar TX: BCDAC36B605576C8182C2829C808B30A69CAD4959D5ED1E6FF9984ABF280D603
    test_compile_protobuf::<StandardCosmosContext>(TestCompileInput {
        coin: &coin,
        input: input.clone(),
        tx_preimage: "0a8c010a89010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e12013112650a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180812110a0b0a046d756f6e120332303010c09a0c1a0a676169612d3133303033208d08",
        tx_prehash: "8a6e6f74625fd39707843360120874853cc0c1d730b087f3939f4b187c75b907",
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H"}"#,
        signature: "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"+eH0ABZXpCAJxOtoWWJdLkHpYfxy79KEKQnImOQ5/B9UmRbk7KxnbuNTx9VMWuMKKbQhC4v/Dr/cs3XhBQAvRw=="}]"#,
    });
}
