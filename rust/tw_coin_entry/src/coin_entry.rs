// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use crate::derivation::Derivation;
use crate::error::AddressError;
use crate::prefix::Prefix;
use tw_keypair::tw::PublicKey;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite};

pub use tw_proto::{ProtoError, ProtoResult};

pub trait CoinEntry: Sized {
    type AddressPrefix: Prefix;
    type Address;
    type SigningInput<'a>: MessageRead<'a>;
    type SigningOutput: MessageWrite;

    /// Tries to parse `Self::Address` from the given `address` string by `coin` type and address `prefix`.
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> Result<Self::Address, AddressError>;

    /// Validates the given `address` by `coin` type and address `prefix`.
    fn validate_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> Result<(), AddressError> {
        <Self as CoinEntry>::parse_address(self, coin, address, prefix).map(|_| ())
    }

    /// Derives an address associated with the given `public_key` by `coin` type, `derivation` and address `prefix`.
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> Result<Self::Address, AddressError>;

    /// Signs a transaction declared as the given `input`.
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput;

    /// Signs a transaction declared as the given raw `input`.
    fn sign_raw(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Vec<u8>> {
        let input: Self::SigningInput<'_> = deserialize(input)?;
        let output = <Self as CoinEntry>::sign(self, coin, input);
        serialize(&output)
    }
}
