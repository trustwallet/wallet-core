// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::sign;
use tw_any_coin::test_utils::sign_utils::CompilerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::Data;
use tw_proto::BitcoinV2::Proto;

pub use crate::chains::common::bitcoin::sign::Expected;

pub struct BitcoinCompileHelper<'a> {
    input: &'a Proto::SigningInput<'a>,
    coin_type: Option<CoinType>,
    public_keys: Vec<Data>,
    signatures: Vec<Data>,
}

impl<'a> BitcoinCompileHelper<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
        BitcoinCompileHelper {
            input,
            coin_type: None,
            public_keys: Vec::default(),
            signatures: Vec::default(),
        }
    }

    pub fn coin(mut self, coin_type: CoinType) -> Self {
        self.coin_type = Some(coin_type);
        self
    }

    pub fn add_pubkey_sig(mut self, pubkey: Data, sighash: Data) -> Self {
        self.public_keys.push(pubkey);
        self.signatures.push(sighash);
        self
    }
}

impl<'a> BitcoinCompileHelper<'a> {
    pub fn compile(self, expected: Expected) {
        let coin_type = self
            .coin_type
            .expect("'BitcoinPreImageHelper::coin_type' is not set");
        let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
        let compile_output =
            compiler.compile(coin_type, self.input, self.signatures, self.public_keys);

        sign::BitcoinSignHelper::new(self.input)
            .coin(coin_type)
            .verify_output(compile_output, expected);
    }
}
