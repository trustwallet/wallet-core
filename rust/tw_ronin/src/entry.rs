// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use crate::ronin_context::RoninContext;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_evm::modules::compiler::Compiler;
use tw_evm::modules::input_builder::EthInputBuilder;
use tw_evm::modules::json_signer::EthJsonSigner;
use tw_evm::modules::signer::Signer;
use tw_keypair::tw::PublicKey;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct RoninEntry;

impl CoinEntry for RoninEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = EthJsonSigner<RoninContext>;
    type InputBuilder = EthInputBuilder<RoninContext>;
    type PlanBuilder = NoPlanBuilder;

    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Ok(Address::with_secp256k1_pubkey(public_key))
    }

    fn sign(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Signer::<RoninContext>::sign_proto(input)
    }

    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        Compiler::<RoninContext>::preimage_hashes(input)
    }

    fn compile(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        Compiler::<RoninContext>::compile(input, signatures, public_keys)
    }

    fn json_signer(&self) -> Option<Self::JsonSigner> {
        Some(EthJsonSigner::default())
    }

    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        None
    }

    fn signing_input_builder(&self) -> Option<Self::InputBuilder> {
        Some(EthInputBuilder::default())
    }
}
