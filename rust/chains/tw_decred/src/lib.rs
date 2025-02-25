// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! The `tw_decred` crate provides an implementation of the Decred blockchain in Rust.
//!
//! This crate reuses Bitcoin's transaction Signer, SigHasher, and compiler APIs, ensuring a familiar
//! and efficient experience for developers who have previously integrated Bitcoin via WalletCore.
//!
//! Unlike Bitcoin, Decred introduces a different transaction structure and transaction builder.
//!
//! ## Key Features
//!
//! - **Transaction Signer**: Utilizes Bitcoin's transaction signing mechanisms.
//! - **Pre-image hashing**: Utilizes Bitcoin's hashing algorithms in a specific way for Decred transaction.
//! - **Compiler APIs**: Utilizes Bitcoin's compiler APIs to compile a transaction with externally signed signatures.
//! - **Protobuf-Based Transaction Builder**: Provides a transaction builder based on Protobuf
//!   definitions, facilitating the creation and manipulation of Decred transactions.
//! - **Fee calculation**: Utilizes Bitcoin's transaction fee estimation and UTXO selection.
//!
//! ## Rust Usage
//!
//! ```rust,no_run
//! # use std::str::FromStr;
//! # use tw_decred::address::DecredAddress;
//! # use tw_decred::modules::transaction_builder::DecredTransactionBuilder;
//! # use tw_hash::H256;
//! # use tw_keypair::ecdsa::secp256k1;
//! # use tw_utxo::modules::keys_manager::KeysManager;
//! # use tw_utxo::modules::tx_signer::TxSigner;
//! # use tw_utxo::sighash::{SighashBase, SighashType};
//! # use tw_utxo::transaction::standard_transaction::builder::{OutputBuilder, UtxoBuilder};
//!
//! # let public_key: secp256k1::PublicKey;
//! # let txid: H256;
//! # let keys_manager: KeysManager = unimplemented!();
//!
//! // Step 1 - Build an unsigned transaction.
//!
//! let mut tx_builder = DecredTransactionBuilder::new();
//!
//! // Add a P2PKH UTXO.
//! let (utxo, utxo_args) = UtxoBuilder::new()
//!     .prev_txid(txid)
//!     .prev_index(0)
//!     .sequence(u32::MAX)
//!     .amount(100_000_000) // 0.1 DCR
//!     .sighash_type(SighashType::from_base(SighashBase::All))
//!     .p2pkh(&public_key)
//!     .unwrap();
//! tx_builder.push_standard_input(utxo, utxo_args).unwrap();
//!
//! // Add a P2PKH Output.
//! let recipient_addr = DecredAddress::from_str("Dsofok7qyhDLVRXcTqYdFgmGsUFSiHonbWH").unwrap();
//! let output = OutputBuilder::new(10_000_000).p2pkh_from_hash(&recipient_addr.payload());
//! tx_builder.push_standard_output(output);
//!
//! let unsigned_tx = tx_builder.build().unwrap();
//!
//! // Step 2 - Sign the transaction.
//! let signed_tx = TxSigner::sign_tx(unsigned_tx, &keys_manager).unwrap();
//! ```
//!
//! ## Integration with WalletCore
//!
//! If you are using WalletCore in Kotlin, Swift, TypeScript, or Golang, you can seamlessly integrate
//! Groestlcoin functionality using the `AnySigner` or `TransactionCompiler` modules.
//! To do this, pass a `BitcoinV2.Proto.SigningInput` to generate a transaction and sign/pre-image hash/compile it.
//!
//! ### TypeScript Example
//!
//! ```ts
//! const utxo0 = Proto.Input.create({
//!     outPoint: {
//!         hash: utxoTxId,
//!         vout: 0,
//!     },
//!     value: new Long(100_000_000),
//!     sighashType: BitcoinSigHashType.all.value,
//!     receiverAddress: "DscNJ2Ki7HUPHrLGF2teBxSda3uxKSY7Fm6",
//! });
//!
//! const out0 = Proto.Output.create({
//!     value: new Long(10_000_000),
//!     toAddress: "Dsofok7qyhDLVRXcTqYdFgmGsUFSiHonbWH",
//! });
//!
//! const signingInput = Proto.SigningInput.create({
//!     builder: {
//!         version: Proto.TransactionVersion.UseDefault,
//!         inputs: [utxo0],
//!         outputs: [out0],
//!         inputSelector: Proto.InputSelector.SelectDescending,
//!         fixedDustThreshold: dustAmount,
//!     },
//!     privateKeys: [privateKeyData],
//!     chainInfo: {
//!         p2pkhPrefix: 63,
//!         p2shPrefix: 26,
//!     },
//! });
//!
//! // Wrap `BitcoinV2.Proto.SigningInput` signing request into a legacy `Bitcoin.Proto.SigningInput`.
//! const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
//!     signingV2: signingInput,
//!     coinType: CoinType.decred.value,
//! });
//!
//! const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
//! const outputData = AnySigner.sign(encoded, CoinType.decred);
//! const output = TW.Decred.Proto.SigningOutput.decode(outputData);
//!
//! assert.equal(output.error, TW.Common.Proto.SigningError.OK);
//! assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
//! ```

pub mod address;
pub mod context;
pub mod entry;
pub mod modules;
pub mod transaction;
