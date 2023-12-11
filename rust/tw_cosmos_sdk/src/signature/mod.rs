// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_keypair::KeyPairResult;
use tw_memory::Data;

pub mod secp256k1;

pub trait CosmosSignature: Sized {
    fn from_bytes(signature_bytes: &[u8]) -> KeyPairResult<Self>;

    fn to_bytes(&self) -> Data;
}
