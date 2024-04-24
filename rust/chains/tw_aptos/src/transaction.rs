// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::constants::APTOS_SALT;
use crate::transaction_payload::TransactionPayload;
use move_core_types::account_address::AccountAddress;
use serde::Serialize;
use serde_json::{json, Value};
use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::encode;
use tw_encoding::{bcs, EncodingResult};
use tw_keypair::ed25519::sha512::KeyPair;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_memory::Data;
use tw_proto::Aptos::Proto;

#[derive(Clone, Serialize)]
pub enum TransactionAuthenticator {
    /// Single Ed25519 signature
    Ed25519 {
        public_key: Vec<u8>,
        signature: Vec<u8>,
    },
}

impl From<TransactionAuthenticator> for Proto::TransactionAuthenticator<'_> {
    fn from(from: TransactionAuthenticator) -> Self {
        Proto::TransactionAuthenticator {
            signature: Cow::from(from.get_signature()),
            public_key: Cow::from(from.get_public_key()),
        }
    }
}

impl TransactionAuthenticator {
    pub fn get_signature(&self) -> Vec<u8> {
        match self {
            TransactionAuthenticator::Ed25519 {
                public_key: _public_key,
                signature,
            } => signature.clone(),
        }
    }

    pub fn get_public_key(&self) -> Vec<u8> {
        match self {
            TransactionAuthenticator::Ed25519 {
                public_key,
                signature: _signature,
            } => public_key.clone(),
        }
    }

    pub fn to_json(&self) -> Value {
        match self {
            TransactionAuthenticator::Ed25519 {
                public_key,
                signature,
            } => {
                json!({"public_key": encode(public_key, true),
                       "signature": encode(signature, true),
                       "type": "ed25519_signature"})
            },
        }
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

    /// Create a new `RawTransaction` with an entry function
    fn serialize(&self) -> EncodingResult<Data> {
        bcs::encode(&self)
    }

    fn msg_to_sign(&self) -> SigningResult<Data> {
        let serialized = self
            .serialize()
            .into_tw()
            .context("Error serializing RawTransaction")?;
        let mut preimage = tw_hash::sha3::sha3_256(APTOS_SALT);
        preimage.extend_from_slice(serialized.as_slice());
        Ok(preimage)
    }

    pub fn pre_image(&self) -> SigningResult<Vec<u8>> {
        self.msg_to_sign()
    }

    pub fn compile(
        &self,
        signature: Vec<u8>,
        public_key: Vec<u8>,
    ) -> SigningResult<SignedTransaction> {
        let serialized = self.serialize()?;
        let auth = TransactionAuthenticator::Ed25519 {
            public_key,
            signature,
        };
        let mut encoded = serialized.clone();
        encoded.extend_from_slice(bcs::encode(&auth)?.as_slice());
        Ok(SignedTransaction {
            raw_txn: self.clone(),
            authenticator: auth,
            raw_txn_bytes: serialized.to_vec(),
            encoded,
        })
    }

    pub fn sign(self, key_pair: KeyPair) -> SigningResult<SignedTransaction> {
        let to_sign = self.pre_image()?;
        let signature = key_pair.private().sign(to_sign)?.to_bytes().into_vec();
        let pubkey = key_pair.public().as_slice().to_vec();
        self.compile(signature, pubkey)
    }

    pub fn to_json(&self) -> Value {
        json!({
            "expiration_timestamp_secs": self.expiration_timestamp_secs.to_string(),
            "gas_unit_price": self.gas_unit_price.to_string(),
            "max_gas_amount": self.max_gas_amount.to_string(),
            "payload": self.payload.to_json(),
            "sender": self.sender.to_hex_literal(),
            "sequence_number": self.sequence_number.to_string()
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
    pub fn authenticator(&self) -> &TransactionAuthenticator {
        &self.authenticator
    }
    pub fn raw_txn_bytes(&self) -> &Vec<u8> {
        &self.raw_txn_bytes
    }
    pub fn encoded(&self) -> &Vec<u8> {
        &self.encoded
    }

    pub fn to_json(&self) -> Value {
        let mut json_value = self.raw_txn.to_json();
        json_value["signature"] = self.authenticator.to_json();
        json_value
    }
}
