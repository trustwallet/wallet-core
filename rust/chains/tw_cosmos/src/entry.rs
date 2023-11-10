// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_cosmos_sdk::address::{Address, CosmosAddress};
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::signer::tw_signer::TWSigner;
use tw_keypair::tw;
use tw_proto::Cosmos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct CosmosEntry;

impl CoinEntry for CosmosEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;

    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::from_str_with_coin(coin, address)
    }

    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: tw::PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::with_public_key_coin_context(coin, &public_key)
    }

    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        TWSigner::<StandardCosmosContext>::sign(coin, input)
    }

    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        _input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        todo!()
    }

    fn compile(
        &self,
        _coin: &dyn CoinContext,
        _input: Self::SigningInput<'_>,
        _signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        todo!()
    }
}
