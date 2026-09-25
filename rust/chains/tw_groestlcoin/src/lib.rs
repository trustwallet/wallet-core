// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! The `tw_groestlcoin` crate provides an implementation of the Groestlcoin blockchain in Rust.
//!
//! This crate reuses Bitcoin's transaction Signer, SigHasher, and compiler APIs, ensuring a familiar
//! and efficient experience for developers who have previously integrated Bitcoin via WalletCore.
//!
//! Unlike Bitcoin, Groestlcoin transactions use SHA-256 hashing algorithm for sig-hashing,
//! and Groestl legacy addresses use `Groestl-512` checksum hashing.
//!
//! ## Key Features
//!
//! - **Transaction Signer**: Utilizes Bitcoin's transaction signing mechanisms.
//! - **Pre-image hashing**: Utilizes Bitcoin's hashing algorithms in a specific way for Groestlcoin transaction.
//! - **Compiler APIs**: Utilizes Bitcoin's compiler APIs to compile a transaction with externally signed signatures.
//! - **Protobuf-Based Transaction Builder**: Provides a transaction builder based on Protobuf
//!   definitions, facilitating the creation and manipulation of Groestlcoin transactions.
//! - **Fee calculation**: Utilizes Bitcoin's transaction fee estimation and UTXO selection.
//!
//! ## Rust Usage
//!
//! ```rust,no_run
//! # use std::str::FromStr;
//! # use tw_groestlcoin::address::groestlcoin_legacy::GroestlLegacyAddress;
//! # use tw_hash::H256;
//! # use tw_keypair::ecdsa::secp256k1;
//! # use tw_utxo::modules::keys_manager::KeysManager;
//! # use tw_utxo::modules::tx_signer::TxSigner;
//! # use tw_utxo::sighash::{SighashBase, SighashType};
//! # use tw_utxo::transaction::standard_transaction::builder::{OutputBuilder, TransactionBuilder, UtxoBuilder};
//! #
//! # let public_key: secp256k1::PublicKey;
//! # let txid: H256;
//! # let keys_manager: KeysManager = unimplemented!();
//!
//! // Step 1 - Build an unsigned transaction.
//!
//! let mut tx_builder = TransactionBuilder::new();
//!
//! // Add a P2PKH UTXO.
//! let (utxo, utxo_args) = UtxoBuilder::new()
//!     .prev_txid(txid)
//!     .prev_index(0)
//!     .sequence(u32::MAX)
//!     .amount(5000)
//!     .sighash_type(SighashType::from_base(SighashBase::All))
//!     .p2pkh(&public_key)
//!     .unwrap();
//! tx_builder.push_input(utxo, utxo_args);
//!
//! // Add a P2PKH Output.
//! let recipient_addr = GroestlLegacyAddress::from_str("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P").unwrap();
//! let output = OutputBuilder::new(2500).p2pkh_from_hash(&recipient_addr.payload());
//! tx_builder.push_output(output);
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
//!     value: new Long(5000),
//!     sighashType: BitcoinSigHashType.all.value,
//!     receiverAddress: "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne",
//! });
//!
//! const out0 = Proto.Output.create({
//!     value: new Long(2500),
//!     toAddress: "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P",
//! });
//!
//! const changeOut = Proto.Output.create({
//!     // Send the change amount to a different P2PKH address.
//!     // The correct amount will be calculated for us.
//!     toAddress: "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM",
//! });
//!
//! const signingInput = Proto.SigningInput.create({
//!     builder: {
//!         version: Proto.TransactionVersion.UseDefault,
//!         inputs: [utxo0],
//!         outputs: [out0],
//!         changeOutput: changeOut,
//!         inputSelector: Proto.InputSelector.SelectDescending,
//!         fixedDustThreshold: dustAmount,
//!     },
//!     privateKeys: [privateKeyData],
//!     chainInfo: {
//!         p2pkhPrefix: 36,
//!         p2shPrefix: 5,
//!     },
//! });
//!
//! // Wrap `BitcoinV2.Proto.SigningInput` signing request into a legacy `Bitcoin.Proto.SigningInput`.
//! const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
//!     signingV2: signingInput,
//!     coinType: CoinType.groestlcoin.value,
//! });
//!
//! const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
//! const outputData = AnySigner.sign(encoded, CoinType.groestlcoin);
//! const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);
//!
//! assert.equal(output.error, TW.Common.Proto.SigningError.OK);
//! assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
//! ```

pub mod address;
pub mod context;
pub mod entry;
