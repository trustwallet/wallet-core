// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use crate::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use crate::derivation::Derivation;
use crate::error::AddressResult;
use crate::error::SigningResult;
use crate::modules::input_builder::{BuildSigningInputArgs, InputBuilder};
use crate::modules::json_signer::JsonSigner;
use crate::modules::plan_builder::PlanBuilder;
use crate::prefix::AddressPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey};
use tw_memory::Data;
use tw_proto::{deserialize, serialize, ProtoResult};

pub type PrivateKeyBytes = Data;

/// The [`CoinEntry`] trait extension.
pub trait CoinEntryExt {
    fn validate_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<()>;

    /// Validates and normalizes the given `address`.
    fn normalize_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<String>;

    /// Derives an address associated with the given `public_key` by `coin` context, `derivation` and address `prefix`.
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<String>;

    /// Returns underlying data (public key or key hash).
    fn address_to_data(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<Data>;

    /// Signs a transaction declared as the given `input`.
    fn sign(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data>;

    /// Returns `true` if the chain supports JSON signing.
    fn supports_json_signing(&self) -> bool;

    /// Signs a transaction specified by the JSON representation of signing input and a private key.
    /// Returns the JSON representation of the signing output if the blockchain supports JSON signing.
    /// Checks [`CoinEntryExt::supports_json_signing`].
    fn sign_json(
        &self,
        coin: &dyn CoinContext,
        input_json: &str,
        private_key: PrivateKeyBytes,
    ) -> SigningResult<Option<String>>;

    /// Returns hash(es) for signing, needed for external signing.
    /// It will return a proto object named `PreSigningOutput` which will include hash.
    /// We provide a default `PreSigningOutput` in TransactionCompiler.proto.
    fn preimage_hashes(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data>;

    /// Compiles a transaction with externally-supplied `signatures` and `public_keys`.
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: &[u8],
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> ProtoResult<Data>;

    /// Plans a transaction (for UTXO chains only).
    fn plan(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Option<Data>>;

    /// Optional helper to prepare a `SigningInput` from simple parameters.
    /// Not suitable for UTXO chains. Some parameters, like chain-specific fee/gas parameters,
    /// may need to be set in the `SigningInput`.
    ///
    /// Returns `None` if the chain doesn't support creating `SigningInput` from the simple parameters.
    fn build_signing_input(
        &self,
        coin: &dyn CoinContext,
        args: BuildSigningInputArgs,
    ) -> SigningResult<Option<Data>>;
}

impl<T> CoinEntryExt for T
where
    T: CoinEntry,
{
    fn validate_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<()> {
        let prefix = prefix.map(T::AddressPrefix::try_from).transpose()?;
        self.parse_address(coin, address, prefix).map(|_| ())
    }

    fn normalize_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<String> {
        let prefix = prefix.map(T::AddressPrefix::try_from).transpose()?;
        // Parse the address and display it.
        // Please note that `Self::Address::to_string()` returns a normalize address.
        <Self as CoinEntry>::parse_address(self, coin, address, prefix).map(|addr| addr.to_string())
    }

    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<String> {
        let prefix = prefix
            .map(<Self as CoinEntry>::AddressPrefix::try_from)
            .transpose()?;
        <Self as CoinEntry>::derive_address(self, coin, public_key, derivation, prefix)
            .map(|addr| addr.to_string())
    }

    fn address_to_data(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<Data> {
        let prefix = prefix.map(T::AddressPrefix::try_from).transpose()?;

        self.parse_address(coin, address, prefix)
            .map(|addr| addr.data())
    }

    fn sign(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data> {
        let input: T::SigningInput<'_> = deserialize(input)?;
        let output = <Self as CoinEntry>::sign(self, coin, input);
        serialize(&output)
    }

    fn supports_json_signing(&self) -> bool {
        self.json_signer().is_some()
    }

    fn sign_json(
        &self,
        coin: &dyn CoinContext,
        input_json: &str,
        private_key: PrivateKeyBytes,
    ) -> SigningResult<Option<String>> {
        let Some(json_signer) = self.json_signer() else {
            return Ok(None);
        };

        let private_key = PrivateKey::new(private_key)?;
        json_signer.sign_json(coin, input_json, &private_key)
    }

    fn preimage_hashes(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data> {
        let input: T::SigningInput<'_> = deserialize(input)?;
        let output = <Self as CoinEntry>::preimage_hashes(self, coin, input);
        serialize(&output)
    }

    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: &[u8],
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> ProtoResult<Data> {
        let input: T::SigningInput<'_> = deserialize(input)?;
        let output = self.compile(coin, input, signatures, public_keys);
        serialize(&output)
    }

    fn plan(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Option<Data>> {
        let Some(plan_builder) = self.plan_builder() else {
            return Ok(None);
        };

        let input: <T::PlanBuilder as PlanBuilder>::SigningInput<'_> = deserialize(input)?;
        let output = plan_builder.plan(coin, input);
        Ok(Some(serialize(&output)?))
    }

    fn build_signing_input(
        &self,
        coin: &dyn CoinContext,
        args: BuildSigningInputArgs,
    ) -> SigningResult<Option<Data>> {
        let Some(input_builder) = self.signing_input_builder() else {
            return Ok(None);
        };
        let input = input_builder.build_signing_input(coin, args)?;
        Ok(Some(serialize(&input)?))
    }
}
