// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::rlp_encoder::RlpEncoder;
use tw_proto::EthereumRlp::Proto;

fuzz_target!(|input: Proto::EncodingInput<'_>| {
    let _ = RlpEncoder::<StandardEvmContext>::encode_with_proto(input);
});
