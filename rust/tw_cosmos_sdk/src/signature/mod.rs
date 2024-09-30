// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::KeyPairError;
use tw_misc::traits::{FromSlice, ToBytesVec};

pub mod secp256k1;

pub trait CosmosSignature: FromSlice<Error = KeyPairError> + ToBytesVec {}
