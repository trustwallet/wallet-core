// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::abi_encoder::AbiEncoder;
use tw_proto::EthereumAbi::Proto;

#[derive(arbitrary::Arbitrary, Debug)]
struct ValueDecodingInput<'a> {
    encoded: &'a [u8],
    param_type: &'a str,
}

fuzz_target!(|data: ValueDecodingInput<'_>| {
    let input = Proto::ValueDecodingInput {
        encoded: data.encoded.into(),
        param_type: data.param_type.into(),
    };
    let _ = AbiEncoder::<StandardEvmContext>::decode_value(input);
});
