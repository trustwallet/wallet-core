// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_evm::abi::param::Param;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::abi_encoder::AbiEncoder;
use tw_proto::EthereumAbi::Proto;

#[derive(arbitrary::Arbitrary, Debug)]
struct ContractCallDecodingInput<'a> {
    encoded: &'a [u8],
    inputs: Vec<Param>,
}

fuzz_target!(|data: ContractCallDecodingInput<'_>| {
    let params = data
        .inputs
        .into_iter()
        .map(AbiEncoder::<StandardEvmContext>::param_to_proto)
        .collect();
    let decoding_input = Proto::ParamsDecodingInput {
        encoded: data.encoded.into(),
        abi: Proto::mod_ParamsDecodingInput::OneOfabi::abi_params(Proto::AbiParams { params }),
    };
    let _ = AbiEncoder::<StandardEvmContext>::decode_params(decoding_input);
});
