// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::list::RlpList;
use crate::transaction::access_list::AccessList;
use crate::transaction::authorization_list::AuthorizationList;
use crate::transaction::signature::{EthSignature, Signature};
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use tw_coin_entry::error::prelude::*;
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;
use tw_number::U256;

const EIP7702_TX_TYPE: u8 = 0x04;

/// Signed authorization tuple for 7702 txn support.
pub struct SignedEip7702Auth {
    /// The chain ID of the authorization.
    pub chain_id: u64,
    /// The address of the authorization.
    pub address: Address,
    /// The nonce for the authorization.
    pub nonce: u64,
    /// signed authorization tuple.
    pub y_parity: u8,
    /// signed authorization tuple.
    pub r: U256,
    /// signed authorization tuple.
    pub s: U256,
}

/// EIP7702 transaction.
pub struct TransactionEip7702 {
    pub nonce: U256,
    pub max_inclusion_fee_per_gas: U256,
    pub max_fee_per_gas: U256,
    pub gas_limit: U256,
    pub to: Option<Address>,
    pub amount: U256,
    pub payload: Data,
    pub access_list: AccessList,
    pub authorization_list: AuthorizationList,
}

impl TransactionCommon for TransactionEip7702 {
    #[inline]
    fn payload(&self) -> Data {
        self.payload.clone()
    }
}

impl UnsignedTransaction for TransactionEip7702 {
    type SignedTransaction = SignedTransactionEip7702;

    #[inline]
    fn encode(&self, chain_id: U256) -> Data {
        encode_transaction(self, chain_id, None)
    }

    #[inline]
    fn try_into_signed(
        self,
        signature: secp256k1::Signature,
        chain_id: U256,
    ) -> SigningResult<Self::SignedTransaction> {
        Ok(SignedTransactionEip7702 {
            unsigned: self,
            signature: Signature::new(signature),
            chain_id,
        })
    }
}

pub struct SignedTransactionEip7702 {
    unsigned: TransactionEip7702,
    signature: Signature,
    chain_id: U256,
}

impl TransactionCommon for SignedTransactionEip7702 {
    #[inline]
    fn payload(&self) -> Data {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedTransactionEip7702 {
    type Signature = Signature;

    #[inline]
    fn encode(&self) -> Data {
        encode_transaction(&self.unsigned, self.chain_id, Some(&self.signature))
    }

    #[inline]
    fn signature(&self) -> &Self::Signature {
        &self.signature
    }
}

fn encode_transaction(
    tx: &TransactionEip7702,
    chain_id: U256,
    signature: Option<&Signature>,
) -> Data {
    let mut list = RlpList::new();
    list.append(&chain_id)
        .append(&tx.nonce)
        .append(&tx.max_inclusion_fee_per_gas)
        .append(&tx.max_fee_per_gas)
        .append(&tx.gas_limit)
        .append(&tx.to)
        .append(&tx.amount)
        .append(tx.payload.as_slice())
        .append(&tx.access_list)
        .append(&tx.authorization_list);

    if let Some(signature) = signature {
        list.append(&signature.v());
        list.append(&signature.r());
        list.append(&signature.s());
    }

    let tx_encoded = list.finish();

    let mut envelope = Vec::with_capacity(tx_encoded.len() + 1);
    envelope.push(EIP7702_TX_TYPE);
    envelope.extend_from_slice(tx_encoded.as_slice());
    envelope
}
