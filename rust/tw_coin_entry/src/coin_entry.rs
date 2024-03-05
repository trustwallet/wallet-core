// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
use crate::modules::transaction_decoder::TransactionDecoder;
use crate::modules::wallet_connector::WalletConnector;
pub use tw_proto::{ProtoError, ProtoResult};

pub type PrivateKeyBytes = Data;
pub type SignatureBytes = Data;
pub type PublicKeyBytes = Data;

pub trait CoinAddress: fmt::Display {
    fn data(&self) -> Data;
}

/// The main coin entry trait. It is responsible for address management and the transaction signing.
///
/// # Maintaining
///
/// Please sync them with the code generator template if there is need to make any changes in this trait
/// (e.g adding/deleting a method or an associated type):
/// https://github.com/trustwallet/wallet-core/blob/master/codegen-v2/src/codegen/rust/templates/blockchain_crate/entry.rs
pub trait CoinEntry {
    /// Address prefix that is used to derive or validate an address for a different network.
    /// It can be a Bech32 HRP prefix, or p2pkh/p2sh Base58 prefix, or even a set of different address prefixes.
    /// **Optional**. Use `NoPrefix` if the blockchain does not support any address prefixes.
    type AddressPrefix: Prefix;
    /// Address type that should be parsable from a string and have other required methods.
    type Address: CoinAddress;
    /// Protobuf message that provides the information about a transaction to be generated and signed.
    type SigningInput<'a>: MessageRead<'a> + MessageWrite;
    /// Protobuf message - result of the request to sign or compile a transaction.
    /// Contains a serialized transaction or an error if occurred.
    type SigningOutput: MessageWrite;
    /// Protobuf message - result of the request to obtain a transaction preimage hashes.
    type PreSigningOutput: MessageWrite;

    /// Not supported at this moment.
    /// Use `NoJsonSigner`.
    type JsonSigner: JsonSigner;
    /// Transaction Planner - the module provides transaction planning functionality.
    /// Used mostly in Bitcoin and UTXO-based chains.
    ///
    /// **Optional**. Use `NoPlanBuilder` if the blockchain does not support transaction planning.
    type PlanBuilder: PlanBuilder;
    /// Message Signer - the module provides regular message signing functionality.
    ///
    /// **Optional**. Use `NoMessageSigner` if the blockchain does not support message signing.
    type MessageSigner: MessageSigner;
    /// WalletConnect Connector - the module allows to parse transactions received in WalletConnect format.
    ///
    /// **Optional**. Use `NoWalletConnector` if the blockchain does not support WalletConnect transactions.
    type WalletConnector: WalletConnector;
    /// TransactionDecoder - the module allows to decode transactions from binary representation.
    ///
    /// **Optional**. Use `NoTransactionDecoder` if the blockchain does not support transaction decoding yet.
    type TransactionDecoder: TransactionDecoder;

    /// Tries to parse `Self::Address` from the given `address` string by `coin` type and address `prefix`.
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address>;

    /// Tries to parse `Self::Address` from the given `address` string by `coin` type.
    /// Please note that this method does not check if the address belongs to the given chain.
    fn parse_address_unchecked(
        &self,
        coin: &dyn CoinContext,
        address: &str,
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

    /// It is optional, Parsing signing requests received through WalletConnect.
    /// Returns `Ok(None)` if the blockchain does not support WalletConnect transactions.
    #[inline]
    fn wallet_connector(&self) -> Option<Self::WalletConnector> {
        None
    }

    /// It is optional, Transaction decoding.
    /// Returns `Ok(None)` if the blockchain does not support transaction decoding yet.
    #[inline]
    fn transaction_decoder(&self) -> Option<Self::TransactionDecoder> {
        None
    }
}
