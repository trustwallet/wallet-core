// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use move_core_types::account_address::AccountAddress;
use serde::Serialize;
use tw_keypair::ed25519::sha512::{KeyPair, PublicKey};
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_number::Sign;
use crate::transaction_payload::{EntryFunction, TransactionPayload};

#[derive(Clone, Serialize)]
pub enum TransactionAuthenticator {
    /// Single Ed25519 signature
    Ed25519 {
        public_key: Vec<u8>,
        signature: Vec<u8>,
    }
}

impl TransactionAuthenticator {
    pub fn get_signature(&self) -> Vec<u8> {
        match self { TransactionAuthenticator::Ed25519 { public_key: _public_key, signature } => { signature.clone() } }
    }
}

/// RawTransaction is the portion of a transaction that a client signs.
#[derive(Clone, Serialize)]
pub struct RawTransaction {
    /// Sender's address.
    sender: AccountAddress,

    /// Sequence number of this transaction. This must match the sequence number
    /// stored in the sender's account at the time the transaction executes.
    sequence_number: u64,

    /// The transaction payload, e.g., a script to execute.
    payload: TransactionPayload,

    /// Maximal total gas to spend for this transaction.
    max_gas_amount: u64,

    /// Price to be paid per gas unit.
    gas_unit_price: u64,

    /// Expiration timestamp for this transaction, represented
    /// as seconds from the Unix Epoch. If the current blockchain timestamp
    /// is greater than or equal to this time, then the transaction has
    /// expired and will be discarded. This can be set to a large value far
    /// in the future to indicate that a transaction does not expire.
    expiration_timestamp_secs: u64,

    /// Chain ID of the Aptos network this transaction is intended for.
    chain_id: u8,
}

impl RawTransaction {
    /// Create a new `RawTransaction` with a payload.
    ///
    /// It can be either to publish a module, to execute a script, or to issue a writeset
    /// transaction.
    pub fn new(
        sender: AccountAddress,
        sequence_number: u64,
        payload: TransactionPayload,
        max_gas_amount: u64,
        gas_unit_price: u64,
        expiration_timestamp_secs: u64,
        chain_id: u8,
    ) -> Self {
        RawTransaction {
            sender,
            sequence_number,
            payload,
            max_gas_amount,
            gas_unit_price,
            expiration_timestamp_secs,
            chain_id,
        }
    }

    /// Create a new `RawTransaction` with an entry function.
    pub fn new_entry_function(
        sender: AccountAddress,
        sequence_number: u64,
        entry_function: EntryFunction,
        max_gas_amount: u64,
        gas_unit_price: u64,
        expiration_timestamp_secs: u64,
        chain_id: u8,
    ) -> Self {
        RawTransaction {
            sender,
            sequence_number,
            payload: TransactionPayload::EntryFunction(entry_function),
            max_gas_amount,
            gas_unit_price,
            expiration_timestamp_secs,
            chain_id,
        }
    }

    pub fn sign(
        self,
        key_pair: KeyPair,
    ) -> Result<SignedTransaction, String> {
        let mut serialized = bcs::to_bytes(&self).unwrap();
        let mut to_sign = tw_hash::sha3::sha3_256("APTOS::RawTransaction".as_bytes());
        to_sign.extend_from_slice(serialized.as_slice());
        let signed = key_pair.private().sign(to_sign.clone()).unwrap();
        let auth = TransactionAuthenticator::Ed25519 {
            public_key: key_pair.public().as_slice().to_vec(),
            signature: signed.to_bytes().into_vec(),
        };
        let mut encoded = serialized.clone();
        encoded.extend_from_slice(bcs::to_bytes(&auth).unwrap().as_slice());
        Ok(SignedTransaction {
            raw_txn: self.clone(),
            authenticator: auth.clone(),
            raw_txn_bytes: serialized.to_vec(),
            encoded,
        })
    }
}

/// A transaction that has been signed.
///
/// A `SignedTransaction` is a single transaction that can be atomically executed. Clients submit
/// these to validator nodes, and the validator and executor submits these to the VM.
///
#[derive(Clone, Serialize)]
pub struct SignedTransaction {
    /// The raw transaction
    raw_txn: RawTransaction,

    /// Public key and signature to authenticate
    authenticator: TransactionAuthenticator,

    #[serde(skip_serializing)]
    /// Raw txs bytes
    raw_txn_bytes: Vec<u8>,

    #[serde(skip_serializing)]
    /// Encoded bytes to be broadcast
    encoded: Vec<u8>,
}

impl SignedTransaction {
    pub fn raw_txn(&self) -> &RawTransaction {
        &self.raw_txn
    }
    pub fn authenticator(&self) -> &TransactionAuthenticator {
        &self.authenticator
    }
    pub fn raw_txn_bytes(&self) -> &Vec<u8> {
        &self.raw_txn_bytes
    }
    pub fn encoded(&self) -> &Vec<u8> {
        &self.encoded
    }
}