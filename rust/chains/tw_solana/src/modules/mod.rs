// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use std::collections::HashMap;
use tw_keypair::ed25519;

pub mod compiled_instructions;
pub mod compiled_keys;
pub mod instruction_builder;
pub mod message_builder;
pub mod proto_builder;
pub mod transaction_decoder;
pub mod transaction_util;
pub mod tx_signer;
pub mod utils;
pub mod wallet_connect;

pub type PubkeySignatureMap = HashMap<SolanaAddress, ed25519::Signature>;
