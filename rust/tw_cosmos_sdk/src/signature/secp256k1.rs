// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signature::CosmosSignature;
use tw_keypair::ecdsa::secp256k1;

pub type Secp256k1Signature = secp256k1::VerifySignature;

impl CosmosSignature for Secp256k1Signature {}
