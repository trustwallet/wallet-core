// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_keypair::ecdsa::secp256k1;

pub type BinanceSignature = secp256k1::VerifySignature;
