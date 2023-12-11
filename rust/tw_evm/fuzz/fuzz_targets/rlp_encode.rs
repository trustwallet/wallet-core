// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::rlp_encoder::RlpEncoder;
use tw_proto::EthereumRlp::Proto;

fuzz_target!(|input: Proto::EncodingInput<'_>| {
    let _ = RlpEncoder::<StandardEvmContext>::encode_with_proto(input);
});
