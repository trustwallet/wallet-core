// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_evm::abi::token::Token;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::abi_encoder::AbiEncoder;
use tw_proto::EthereumAbi::Proto;

#[derive(arbitrary::Arbitrary, Debug)]
struct FunctionEncodingInput<'a> {
    function_name: &'a str,
    tokens: Vec<Token>,
}

fuzz_target!(|data: FunctionEncodingInput<'_>| {
    let tokens = data
        .tokens
        .into_iter()
        .map(AbiEncoder::<StandardEvmContext>::token_to_proto)
        .collect();

    let input = Proto::FunctionEncodingInput {
        function_name: data.function_name.into(),
        tokens,
    };
    let _ = AbiEncoder::<StandardEvmContext>::encode_contract_call(input);
});
