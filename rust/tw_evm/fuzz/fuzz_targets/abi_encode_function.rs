// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::abi_encoder::AbiEncoder;
use tw_proto::EthereumAbi::Proto;

fuzz_target!(|input: Proto::FunctionEncodingInput<'_>| {
    let _ = AbiEncoder::<StandardEvmContext>::encode_contract_call(input);
});
