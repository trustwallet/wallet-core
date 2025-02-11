// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! The `tw_ripple` crate provides a comprehensive implementation of the Ripple/XRP blockchain in Rust.
//! This crate is designed to facilitate the development of applications that interact with the Ripple network
//! by providing essential functionalities such as address generation, transaction building, signing, and more.
//!
//! ## Source Code
//!
//! The code of this crate is mainly inspired by [xrpl-rust](https://github.com/sephynox/xrpl-rust) and [xrpl-py](https://github.com/XRPLF/xrpl-py),
//! and tested well against unit tests from `xrpl-py`.
//!
//! ## Features
//!
//! ### Address Generation
//! - **Classic and X Address Generation**: Generate both Classic and X addresses from a public key, ensuring compatibility with different address formats used in the Ripple ecosystem.
//!
//! ### Transaction Management
//! - **Transaction Builder**: Easily construct various types of transactions with the built-in transaction builder.
//! - **Transaction Signing**: Sign transactions securely using your private key.
//! - **Transaction Pre-Image Hashing and Compiling**: Hash and compile transaction pre-images,
//!   allowing you to sign transactions externally and compile them with the provided signature.
//! - **Protobuf-Based Transaction Builder**: Utilize Protobuf for building transactions,
//!   ensuring efficient and standardized transaction structures.
//!
//! ### Supported Transaction Types
//! The `tw_ripple` crate supports a wide range of transaction types, including:
//! - `Payment`: Transfer XRP or issued currencies between accounts.
//! - `EscrowCreate`: Create an escrow to hold XRP until certain conditions are met.
//! - `EscrowCancel`: Cancel an existing escrow.
//! - `EscrowFinish`: Complete an escrow and release the held XRP.
//! - `NftokenBurn`: Permanently remove an NFT from the ledger.
//! - `NftokenCreateOffer`: Create an offer to buy or sell an NFT.
//! - `NftokenCancelOffer`: Cancel an existing NFT offer.
//! - `NftokenAcceptOffer`: Accept an offer to buy or sell an NFT.
//! - `TrustSet`: Establish a trust line between two accounts for issued currencies.
//!
//! ## Rust Usage
//!
//! ```rust,no_run
//! # use std::str::FromStr;
//! # use tw_keypair::ecdsa::secp256k1;
//! # use tw_keypair::traits::KeyPairTrait;
//! # use tw_ripple::address::classic_address::ClassicAddress;
//! # use tw_ripple::address::RippleAddress;
//! # use tw_ripple::encode::{encode_tx, EncodeMode};
//! # use tw_ripple::modules::transaction_signer::TransactionSigner;
//! # use tw_ripple::transaction::transaction_builder::TransactionBuilder;
//! # use tw_ripple::transaction::transaction_type::TransactionType;
//! # use tw_ripple::types::amount::Amount;
//! # use tw_ripple::types::amount::native_amount::NativeAmount;
//! #
//! # let key: secp256k1::KeyPair = todo!();
//!
//! let mut builder = TransactionBuilder::default();
//! builder
//!     .fee(NativeAmount::new(10).unwrap())
//!     .sequence(32_268_248_u32)
//!     .last_ledger_sequence(32_268_269_u32)
//!     .account_str("rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq")
//!     .unwrap()
//!     .signing_pub_key(&key.public());
//!
//! let xrp_amount = Amount::NativeAmount(NativeAmount::new(10).unwrap());
//! let destination = ClassicAddress::from_str("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF").unwrap();
//! let destination_tag = None;
//!
//! let payment = builder.payment(xrp_amount, destination, destination_tag).unwrap();
//! let unsigned_tx = TransactionType::Payment(payment);
//!
//! let signed_tx = TransactionSigner::sign(unsigned_tx, key.private()).unwrap();
//! let _encoded_tx = encode_tx(&signed_tx, EncodeMode::All).unwrap();
//! ```
//!
//! ## Integration with WalletCore
//!
//! If you are using WalletCore in Kotlin, Swift, TypeScript, or Golang, you can seamlessly integrate
//! XRP functionality using the `AnySigner` or `TransactionCompiler` modules.
//! To do this, pass a `Ripple.Proto.SigningInput` to generate a transaction and sign/pre-image hash/compile it.
//!
//! ### TypeScript Example
//!
//! ```ts
//! const txDataInput = TW.Ripple.Proto.SigningInput.create({
//!     fee: new Long(10),
//!     sequence: 32_268_248,
//!     lastLedgerSequence: 32_268_269,
//!     account: "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq",
//!     privateKey: privateKey.data(),
//!     opPayment: {
//!         amount: new Long(10),
//!         destination: "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF",
//!     },
//! });
//! const input = TW.Ripple.Proto.SigningInput.encode(txDataInput).finish();
//! const outputData = AnySigner.sign(input, CoinType.xrp);
//! const output = TW.Ripple.Proto.SigningOutput.decode(outputData);
//!
//! assert.equal(output.error, TW.Common.Proto.SigningError.OK);
//! ```

pub mod address;
pub mod compiler;
pub mod definitions;
pub mod encode;
pub mod entry;
pub mod modules;
pub mod signer;
pub mod transaction;
pub mod types;
