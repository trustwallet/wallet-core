// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_any_signer::tw_any_signer_sign;
use crate::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_compile, tw_transaction_compiler_pre_image_hashes,
};
use std::marker::PhantomData;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_memory::Data;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite};

#[derive(Default)]
pub struct AnySignerHelper<'a, Output: MessageRead<'a>> {
    output_data: Data,
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

#[derive(Default)]
pub struct PreImageHelper<'a, Output: MessageRead<'a>> {
    output_data: Data,
    _output_type: PhantomData<&'a Output>,
}

impl<'a, Output: MessageRead<'a>> PreImageHelper<'a, Output> {
    pub fn pre_image_hashes<Input: MessageWrite>(
        &'a mut self,
        coin_type: CoinType,
        input: &Input,
    ) -> Output {
        let input_data = TWDataHelper::create(serialize(input).unwrap());

        self.output_data = TWDataHelper::wrap(unsafe {
            tw_transaction_compiler_pre_image_hashes(coin_type as u32, input_data.ptr())
        })
        .to_vec()
        .expect("!tw_transaction_compiler_pre_image_hashes returned nullptr");

        let output: Output = deserialize(&self.output_data).unwrap();
        output
    }
}

#[derive(Default)]
pub struct CompilerHelper<'a, Output: MessageRead<'a>> {
    output_data: Data,
    _output_type: PhantomData<&'a Output>,
}

impl<'a, Output: MessageRead<'a>> CompilerHelper<'a, Output> {
    pub fn compile<Input: MessageWrite>(
        &'a mut self,
        coin_type: CoinType,
        input: &Input,
        signatures: Vec<Data>,
        public_keys: Vec<Data>,
    ) -> Output {
        let input_data = TWDataHelper::create(serialize(input).unwrap());

        let signatures = TWDataVectorHelper::create(signatures);
        let public_keys = TWDataVectorHelper::create(public_keys);

        self.output_data = TWDataHelper::wrap(unsafe {
            tw_transaction_compiler_compile(
                coin_type as u32,
                input_data.ptr(),
                signatures.ptr(),
                public_keys.ptr(),
            )
        })
        .to_vec()
        .expect("!tw_transaction_compiler_compile returned nullptr");

        let output: Output = deserialize(&self.output_data).unwrap();
        output
    }
}
