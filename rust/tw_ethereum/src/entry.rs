// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use crate::modules::input_builder::EthInputBuilder;
use crate::modules::json_signer::EthJsonSigner;
use crate::signer::Signer;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressError;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::{PublicKey, Signature};
use tw_proto::Ethereum::Proto;

pub struct EthereumEntry;

impl CoinEntry for EthereumEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = tw_proto::TxCompiler::Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = EthJsonSigner;
    type InputBuilder = EthInputBuilder;
    type PlanBuilder = NoPlanBuilder;

    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> Result<Self::Address, AddressError> {
        Address::from_str(address)
    }

    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> Result<Self::Address, AddressError> {
        let public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Ok(Address::with_secp256k1_pubkey(public_key))
    }

    fn sign(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Signer::sign_proto(input)
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
        _signatures: Vec<Signature>,
        _public_keys: Vec<PublicKey>,
    ) -> Self::SigningOutput {
        todo!()
    }

    fn json_signer(&self) -> Option<Self::JsonSigner> {
        Some(EthJsonSigner)
    }

    fn signing_input_builder(&self) -> Option<Self::InputBuilder> {
        Some(EthInputBuilder)
    }
}
