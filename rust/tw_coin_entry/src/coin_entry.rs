// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use crate::derivation::Derivation;
use crate::error::AddressResult;
use crate::modules::json_signer::JsonSigner;
use crate::modules::plan_builder::PlanBuilder;
use crate::prefix::Prefix;
use std::fmt;
use tw_keypair::tw::PublicKey;
use tw_memory::Data;
use tw_proto::{MessageRead, MessageWrite};

use crate::modules::message_signer::MessageSigner;
pub use tw_proto::{ProtoError, ProtoResult};

pub type PrivateKeyBytes = Data;
pub type SignatureBytes = Data;
pub type PublicKeyBytes = Data;

pub trait CoinAddress: fmt::Display {
    fn data(&self) -> Data;
}

pub trait CoinEntry {
    type AddressPrefix: Prefix;
    type Address: CoinAddress;
    type SigningInput<'a>: MessageRead<'a> + MessageWrite;
    type SigningOutput: MessageWrite;
    type PreSigningOutput: MessageWrite;

    // Optional modules:
    type JsonSigner: JsonSigner;
    type PlanBuilder: PlanBuilder;
    type MessageSigner: MessageSigner;

    /// Tries to parse `Self::Address` from the given `address` string by `coin` type and address `prefix`.
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address>;

    /// Derives an address associated with the given `public_key` by `coin` context, `derivation` and address `prefix`.
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address>;

    /// Signs a transaction declared as the given `input`.
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput;

    /// Returns hash(es) for signing, needed for external signing.
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput;

    /// Compiles a transaction with externally-supplied `signatures` and `public_keys`.
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput;

    /// It is optional, Signing JSON input with private key.
    /// Returns `Ok(None)` if the chain doesn't support signing JSON.
    #[inline]
    fn json_signer(&self) -> Option<Self::JsonSigner> {
        None
    }

    /// Planning, for UTXO chains, in preparation for signing.
    /// Returns an optional `Plan` builder. Only UTXO chains need it.
    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        None
    }

    /// It is optional, Signing regular messages.
    /// Returns `Ok(None)` if the chain doesn't support regular message signing.
    #[inline]
    fn message_signer(&self) -> Option<Self::MessageSigner> {
        None
    }
}
