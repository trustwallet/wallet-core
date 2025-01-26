// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Substrate chain entry implementation traits and helpers.
//!
//! This module provides the base trait and implementation patterns for Substrate-based chains.
//! It handles the common functionality needed for transaction building, signing, and output generation.
//!
//! # Example
//!
//! Here's an example of implementing a custom Substrate chain entry:
//!
//! ```rust,ignore
//! use tw_proto::MyChain::Proto;
//! use tw_substrate::substrate_coin_entry::SubstrateCoinEntry;
//! use tw_substrate::TransactionBuilder;
//! use tw_keypair::ed25519::sha512::{KeyPair, PublicKey};
//! use tw_coin_entry::{coin_context::CoinContext, error::SigningResult};
//! use tw_scale::RawOwned;
//!
//! pub struct MyChainEntry;
//!
//! impl SubstrateCoinEntry for MyChainEntry {
//!     type SigningInput<'a> = Proto::SigningInput<'a>;
//!     type SigningOutput = Proto::SigningOutput<'static>;
//!     type PreSigningOutput = Proto::PreSigningOutput<'static>;
//!
//!     fn get_keypair(
//!         &self,
//!         _coin: &dyn CoinContext,
//!         input: &Self::SigningInput<'_>,
//!     ) -> SigningResult<KeyPair> {
//!         // Convert private key from input into KeyPair
//!         KeyPair::try_from(input.private_key.as_ref())
//!     }
//!
//!     fn build_transaction(
//!         &self,
//!         coin: &dyn CoinContext,
//!         public_key: Option<PublicKey>,
//!         input: &Self::SigningInput<'_>,
//!     ) -> SigningResult<TransactionBuilder> {
//!         // Build the transaction using chain-specific logic
//!         let context = my_chain::ctx_from_tw(input)?;
//!         let call = my_chain::call_encoder::encode_call(input)?;
//!         TransactionBuilder::new(context, call)
//!     }
//!
//!     fn signing_output(
//!         &self,
//!         _coin: &dyn CoinContext,
//!         result: SigningResult<RawOwned>,
//!     ) -> Self::SigningOutput {
//!         let mut output = Proto::SigningOutput::new();
//!         match result {
//!             Ok(encoded) => {
//!                 output.encoded = encoded.to_vec();
//!             }
//!             Err(e) => {
//!                 output.error = true;
//!                 output.error_message = e.to_string();
//!             }
//!         }
//!         output
//!     }
//!
//!     fn presigning_output(
//!         &self,
//!         _coin: &dyn CoinContext,
//!         result: SigningResult<RawOwned>,
//!     ) -> Self::PreSigningOutput {
//!         let mut output = Proto::PreSigningOutput::new();
//!         match result {
//!             Ok(encoded) => {
//!                 output.data = encoded.to_vec();
//!             }
//!             Err(e) => {
//!                 output.error = true;
//!                 output.error_message = e.to_string();
//!             }
//!         }
//!         output
//!     }
//! }
//! ```
//!
//! The example above demonstrates:
//! * Implementing the required associated types for protocol buffer messages
//! * Converting private keys to keypairs
//! * Building transactions using chain-specific context and call encoding
//! * Handling success and error cases in output generation
//!
//! Once implemented, the entry can be wrapped with `SubstrateEntry<T>` to provide
//! the complete `CoinEntry` implementation.

use crate::TransactionBuilder;
use tw_coin_entry::{coin_context::CoinContext, error::prelude::*};
use tw_keypair::ed25519::sha512::{KeyPair, PublicKey};
use tw_proto::{MessageRead, MessageWrite};
use tw_scale::RawOwned;

/// Trait for implementing a Substrate-based chain entry.
///
/// This trait provides the core functionality needed for Substrate-based chains,
/// including transaction building, signing, and output generation.
///
/// # Type Parameters
/// * `SigningInput`: The protobuf message type for transaction input
/// * `SigningOutput`: The protobuf message type for transaction output
/// * `PreSigningOutput`: The protobuf message type for pre-signing output
pub trait SubstrateCoinEntry {
    /// The protobuf message type for transaction input.
    type SigningInput<'a>: MessageRead<'a> + MessageWrite;
    /// The protobuf message type for transaction output.
    type SigningOutput: MessageWrite;
    /// The protobuf message type for pre-signing output.
    type PreSigningOutput: MessageWrite;

    /// Retrieves the keypair from the signing input.
    ///
    /// # Parameters
    /// * `coin`: The coin context
    /// * `input`: The signing input containing private key data
    ///
    /// # Returns
    /// Returns a `SigningResult` containing the Ed25519 keypair if successful.
    fn get_keypair(
        &self,
        coin: &dyn CoinContext,
        input: &Self::SigningInput<'_>,
    ) -> SigningResult<KeyPair>;

    /// Builds an unsigned transaction from the input data.
    ///
    /// The returned transaction builder can be used to sign the transaction.
    ///
    /// # Parameters
    /// * `coin`: The coin context
    /// * `public_key`: Optional public key for address derivation
    /// * `input`: The signing input containing transaction data
    ///
    /// # Returns
    /// Returns a `SigningResult` containing the transaction builder if successful.
    fn build_transaction(
        &self,
        coin: &dyn CoinContext,
        public_key: Option<PublicKey>,
        input: &Self::SigningInput<'_>,
    ) -> SigningResult<TransactionBuilder>;

    /// Creates the signing output from the signed transaction built from the transaction builder.
    ///
    /// # Parameters
    /// * `coin`: The coin context
    /// * `result`: The results from building and signing the transaction.
    ///
    /// # Returns
    /// Returns the protobuf signing output message.
    fn signing_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<RawOwned>,
    ) -> Self::SigningOutput;

    /// Creates the pre-signing output from the unsigned transaction built from the transaction builder.
    ///
    /// # Parameters
    /// * `coin`: The coin context
    /// * `result`: The result from building the transaction.
    ///
    /// # Returns
    /// Returns the protobuf pre-signing output message.
    fn presigning_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<RawOwned>,
    ) -> Self::PreSigningOutput;
}
