// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use crate::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use crate::derivation::Derivation;
use crate::error::prelude::*;
use crate::modules::json_signer::JsonSigner;
use crate::modules::message_signer::MessageSigner;
use crate::modules::plan_builder::PlanBuilder;
use crate::modules::transaction_decoder::TransactionDecoder;
use crate::modules::transaction_util::TransactionUtil;
use crate::modules::wallet_connector::WalletConnector;
use crate::prefix::AddressPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey};
use tw_memory::Data;
use tw_proto::WalletConnect::Proto as WCProto;
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

    /// Normalizes the given `address` without checking its belonging to the `coin` network.
    fn normalize_address_unchecked(&self, address: &str) -> AddressResult<String>;

    /// Derives an address associated with the given `public_key` by `coin` context, `derivation` and address `prefix`.
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<String>;

    /// Returns underlying data (public key or key hash).
    fn address_to_data(&self, address: &str) -> AddressResult<Data>;

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
    ) -> SigningResult<String>;

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
    fn plan(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<Data>;

    /// Signs a message.
    fn sign_message(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<Data>;

    /// Computes preimage hashes of a message.
    fn message_preimage_hashes(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<Data>;

    /// Verifies a signature for a message.
    fn verify_message(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<bool>;

    /// Signs a transaction in WalletConnect format.
    fn wallet_connect_parse_request(
        &self,
        coin: &dyn CoinContext,
        input: &[u8],
    ) -> SigningResult<Data>;

    /// Decodes a transaction from binary representation.
    fn decode_transaction(&self, coin: &dyn CoinContext, tx: &[u8]) -> SigningResult<Data>;

    /// Calculate the TX hash of a transaction.
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String>;
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

    fn normalize_address_unchecked(&self, address: &str) -> AddressResult<String> {
        // Parse the address and display it.
        // Please note that `Self::Address::to_string()` returns a normalize address.
        <Self as CoinEntry>::parse_address_unchecked(self, address).map(|addr| addr.to_string())
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

    fn address_to_data(&self, address: &str) -> AddressResult<Data> {
        self.parse_address_unchecked(address)
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
    ) -> SigningResult<String> {
        let Some(json_signer) = self.json_signer() else {
            return TWError::err(SigningErrorType::Error_not_supported);
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

    fn plan(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<Data> {
        let Some(plan_builder) = self.plan_builder() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        let input: <T::PlanBuilder as PlanBuilder>::SigningInput<'_> = deserialize(input)?;
        let output = plan_builder.plan(coin, &input);
        serialize(&output).map_err(SigningError::from)
    }

    fn sign_message(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<Data> {
        let Some(message_signer) = self.message_signer() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        let input: <T::MessageSigner as MessageSigner>::MessageSigningInput<'_> =
            deserialize(input)?;
        let output = message_signer.sign_message(coin, input);
        serialize(&output).map_err(SigningError::from)
    }

    fn message_preimage_hashes(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<Data> {
        let Some(message_signer) = self.message_signer() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        let input: <T::MessageSigner as MessageSigner>::MessageSigningInput<'_> =
            deserialize(input)?;
        let output = message_signer.message_preimage_hashes(coin, input);
        serialize(&output).map_err(SigningError::from)
    }

    fn verify_message(&self, coin: &dyn CoinContext, input: &[u8]) -> SigningResult<bool> {
        let Some(message_signer) = self.message_signer() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        let input: <T::MessageSigner as MessageSigner>::MessageVerifyingInput<'_> =
            deserialize(input)?;
        Ok(message_signer.verify_message(coin, input))
    }

    fn wallet_connect_parse_request(
        &self,
        coin: &dyn CoinContext,
        input: &[u8],
    ) -> SigningResult<Data> {
        let Some(wc_connector) = self.wallet_connector() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        let input: WCProto::ParseRequestInput = deserialize(input)?;
        let output = wc_connector.parse_request(coin, input);
        serialize(&output).map_err(SigningError::from)
    }

    fn decode_transaction(&self, coin: &dyn CoinContext, tx: &[u8]) -> SigningResult<Data> {
        let Some(tx_decoder) = self.transaction_decoder() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        let output = tx_decoder.decode_transaction(coin, tx);
        serialize(&output).map_err(SigningError::from)
    }

    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let Some(tx_util) = self.transaction_util() else {
            return TWError::err(SigningErrorType::Error_not_supported);
        };

        tx_util.calc_tx_hash(coin, encoded_tx)
    }
}
