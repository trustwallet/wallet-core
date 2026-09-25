// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_any_signer::tw_any_signer_plan;
use std::marker::PhantomData;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::Data;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite};

#[derive(Default)]
pub struct AnyPlannerHelper<'a, Output: MessageRead<'a>> {
    output_data: Data,
    _output_type: PhantomData<&'a Output>,
}

impl<'a, Output: MessageRead<'a>> AnyPlannerHelper<'a, Output> {
    pub fn plan<Input: MessageWrite>(&'a mut self, coin_type: CoinType, input: Input) -> Output {
        let input_data = TWDataHelper::create(serialize(&input).unwrap());

        self.output_data =
            TWDataHelper::wrap(unsafe { tw_any_signer_plan(input_data.ptr(), coin_type as u32) })
                .to_vec()
                .expect("!tw_any_signer_plan returned nullptr");

        let output: Output = deserialize(&self.output_data).unwrap();
        output
    }
}
