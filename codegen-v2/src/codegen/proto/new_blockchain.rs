// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::proto::proto_generator::ProtoGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_blockchain(coin: &CoinItem) -> Result<()> {
    ProtoGenerator::generate(coin)
}
