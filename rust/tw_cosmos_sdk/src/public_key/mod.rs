// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningResult;
use tw_proto::google;

pub mod ethermint_eth_secp256k1;
pub mod injective_eth_secp256k1;
pub mod secp256k1;

pub trait CosmosPubkey: Sized {
    fn from_private_key_bytes(private: &[u8]) -> SigningResult<Self>;

    fn from_public_key_bytes(public: &[u8]) -> SigningResult<Self>;
}

pub trait ProtobufPublicKey: CosmosPubkey {
    fn into_proto(self) -> google::protobuf::Any;
}

// pub trait JsonPublicKey {
//     fn into_json_public_key
// }
