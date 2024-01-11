// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signing_input::ThorchainSigningInput;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::compiler::tw_compiler::TWTransactionCompiler;
use tw_proto::Cosmos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct ThorchainCompiler;

impl ThorchainCompiler {
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        ThorchainSigningInput::prepare_signing_input(&mut input);
        TWTransactionCompiler::<StandardCosmosContext>::preimage_hashes(coin, input)
    }

    pub fn compile(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        ThorchainSigningInput::prepare_signing_input(&mut input);
        TWTransactionCompiler::<StandardCosmosContext>::compile(
            coin,
            input,
            signatures,
            public_keys,
        )
    }
}
