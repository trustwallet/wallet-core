// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! The `tw_zcash` crate provides an implementation of the Zcash blockchain in Rust.
//!
//! This crate reuses Bitcoin's transaction Signer, SigHasher, and compiler APIs, ensuring a familiar
//! and efficient experience for developers who have previously integrated Bitcoin via WalletCore.
//!
//! Unlike Bitcoin, ZCash introduces a different transaction structure (transparent only)
//! and transaction builder.
//!
//! ## Key Features
//!
//! - **Transaction Signer**: Utilizes Bitcoin's transaction signing mechanisms.
//! - **Pre-image hashing**: Utilizes Bitcoin's hashing algorithms in a specific way for Zcash transaction.
//! - **Compiler APIs**: Utilizes Bitcoin's compiler APIs to compile a transaction with externally signed signatures.
//! - **Zcash-Specific Transaction Structure**: Implements a (transparent) transaction [`ZcashTransaction`] tailored
//!   to the requirements of the Zcash blockchain.
//! - **Protobuf-Based Transaction Builder**: Provides a transaction builder based on Protobuf
//!   definitions, facilitating the creation and manipulation of Zcash transactions.
//! - **Fee calculation**: Utilizes Bitcoin's transaction fee estimation and UTXO selection.
//!   In addition, implements [ZIP-0317](https://zips.z.cash/zip-0317#fee-calculation) algorithm.
//!
//! ## Rust Usage
//!
//! ```rust,no_run
//! # use std::str::FromStr;
//! # use tw_hash::{H256, H32};
//! # use tw_keypair::ecdsa::secp256k1;
//! # use tw_utxo::modules::keys_manager::KeysManager;
//! # use tw_utxo::modules::tx_signer::TxSigner;
//! # use tw_utxo::sighash::{SighashBase, SighashType};
//! # use tw_utxo::transaction::standard_transaction::builder::{OutputBuilder, UtxoBuilder};
//! # use tw_zcash::modules::transaction_builder::ZcashTransactionBuilder;
//! # use tw_zcash::t_address::TAddress;
//! #
//! # let private_key: secp256k1::PrivateKey;
//! # let public_key: secp256k1::PublicKey;
//! # let branch_id: H32;
//! # let txid: H256;
//! # let keys_manager: KeysManager = unimplemented!();
//!
//! // Step 1 - Build an unsigned transaction.
//!
//! let mut tx_builder = ZcashTransactionBuilder::new();
//! // BranchId is required.
//! tx_builder.branch_id(branch_id);
//!
//! // Add a P2PKH UTXO.
//! let (utxo, utxo_args) = UtxoBuilder::new()
//!     .prev_txid(txid)
//!     .prev_index(0)
//!     .sequence(u32::MAX)
//!     .amount(494_000)
//!     .sighash_type(SighashType::from_base(SighashBase::All))
//!     .p2pkh(&public_key)
//!     .unwrap();
//! tx_builder.push_input(utxo, utxo_args);
//!
//! // Add a P2PKH Output.
//! let recipient_addr = TAddress::from_str("t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS").unwrap();
//! let output = OutputBuilder::new(488_000).p2pkh_from_hash(&recipient_addr.payload());
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
//! Zcash functionality using the `AnySigner` or `TransactionCompiler` modules.
//! To do this, pass a `BitcoinV2.Proto.SigningInput` along with `Zcash.TransactionBuilderExtraData` parameters
//! to generate a transaction and sign/pre-image hash/compile it.
//!
//! ### TypeScript Example
//!
//! ```ts
//! const utxo0 = Proto.Input.create({
//!     outPoint: {
//!         hash: utxoTxId,
//!         vout: 0,
//!     },
//!     value: new Long(494_000),
//!     sighashType: BitcoinSigHashType.all.value,
//!     receiverAddress: "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz",
//! });
//!
//! const out0 = Proto.Output.create({
//!     value: new Long(488_000),
//!     toAddress: "t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS",
//! });
//!
//! const signingInput = Proto.SigningInput.create({
//!     builder: {
//!         version: Proto.TransactionVersion.UseDefault,
//!         inputs: [utxo0],
//!         outputs: [out0],
//!         inputSelector: Proto.InputSelector.SelectDescending,
//!         fixedDustThreshold: dustAmount,
//!         // Set ZCash specific extra parameters.
//!         zcashExtraData: {
//!             branchId: sapplingBranchId,
//!         },
//!     },
//!     privateKeys: [privateKeyData],
//!     chainInfo: {
//!         p2pkhPrefix: 184,
//!         p2shPrefix: 189,
//!     },
//! });
//!
//! // Wrap `BitcoinV2.Proto.SigningInput` signing request into a legacy `Bitcoin.Proto.SigningInput`.
//! const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
//!     signingV2: signingInput,
//!     coinType: CoinType.zcash.value,
//! });
//!
//! const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
//! const outputData = AnySigner.sign(encoded, CoinType.zcash);
//! const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);
//!
//! assert.equal(output.error, TW.Common.Proto.SigningError.OK);
//! assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
//! ```

pub mod context;
pub mod entry;
pub mod modules;
pub mod t_address;
pub mod transaction;
