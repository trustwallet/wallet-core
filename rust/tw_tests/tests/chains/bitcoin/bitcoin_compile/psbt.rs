// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::psbt_sign::Expected;
use crate::chains::common::bitcoin::{btc_info, preimage, psbt_compile, transaction_psbt};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ecdsa;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_bitcoin_compile_psbt_thorchain_swap_witness() {
    let private_key = "f00ffbe44c5c2838c13d2778854ac66b75e04eb6054f0241989e223223ad5e55"
        .decode_hex()
        .unwrap();
    let private_key = ecdsa::secp256k1::PrivateKey::try_from(private_key.as_slice()).unwrap();
    let pubkey = private_key.public().compressed();
    assert_eq!(
        pubkey.to_hex(),
        "0306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f3"
    );

    let psbt = "70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d00000000";
    let signing = Proto::SigningInput {
        public_keys: vec![pubkey.to_vec().into()],
        chain_info: btc_info(),
        transaction: transaction_psbt(psbt),
        ..Proto::SigningInput::default()
    };

    let sighash = "4c831a5b2b5210082f27af001e6e13276d90cc8c5e3eb171e24a6fe47b32b9de"
        .decode_hex()
        .unwrap();
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_expected_segwit(pubkey.to_vec(), sighash.clone())
        .pre_image();

    let signature = private_key
        .sign(sighash.as_slice().try_into().unwrap())
        .unwrap();
    assert_eq!(
        signature.to_vec().to_hex(),
        "b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b5774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101"
    );

    psbt_compile::BitcoinPsbtCompileHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_pubkey_sig(pubkey.to_vec(), signature.to_vec())
        .compile(Expected {
            psbt: "70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d01086c02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000",
            encoded: "02000000000101147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000",
            txid: "634a416e82ac710166725f6a4090ac7b5db69687e86b2d2e38dcb3d91c956c32",
            vsize: 216,
            weight: 862,
            fee: 1736,
        });
}
