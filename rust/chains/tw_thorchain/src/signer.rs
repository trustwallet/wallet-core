// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::signing_input::ThorchainSigningInput;
use tw_coin_entry::coin_context::CoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::signer::tw_signer::TWSigner;
use tw_proto::Cosmos::Proto;

pub struct ThorchainSigner;

impl ThorchainSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        ThorchainSigningInput::prepare_signing_input(&mut input);
        TWSigner::<StandardCosmosContext>::sign(coin, input)
    }
}
