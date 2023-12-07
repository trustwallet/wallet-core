// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::tw_any_signer::tw_any_signer_sign;
use std::marker::PhantomData;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite};

#[derive(Default)]
pub struct AnySignerHelper<'a, Output: MessageRead<'a>> {
    output_data: Vec<u8>,
    _output_type: PhantomData<&'a Output>,
}

impl<'a, Output: MessageRead<'a>> AnySignerHelper<'a, Output> {
    pub fn sign<Input: MessageWrite>(&'a mut self, coin_type: CoinType, input: Input) -> Output {
        let input_data = TWDataHelper::create(serialize(&input).unwrap());

        self.output_data =
            TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), coin_type as u32) })
                .to_vec()
                .expect("!tw_any_signer_sign returned nullptr");

        let output: Output = deserialize(&self.output_data).unwrap();
        output
    }
}
