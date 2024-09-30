// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Transactions can be:
//! - Non-typed (legacy, pre-EIP2718) transactions:
//!   - simple ETH transfer
//!   - others with payload, function call, e.g. ERC20 transfer
//! - Typed transactions (enveloped, EIP2718), with specific type and transaction payload
//! - User operations (EIP4337)

use crate::transaction::signature::EthSignature;
use tw_coin_entry::error::prelude::*;
use tw_hash::{sha3::keccak256, H256};
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;
use tw_number::U256;

pub mod access_list;
pub mod signature;
pub mod transaction_eip1559;
pub mod transaction_non_typed;
pub mod user_operation;

pub trait TransactionCommon {
    fn payload(&self) -> Data;
}

pub trait UnsignedTransaction: TransactionCommon {
    type SignedTransaction: SignedTransaction + 'static;

    fn pre_hash(&self, chain_id: U256) -> H256 {
        let hash = keccak256(&self.encode(chain_id));
        H256::try_from(hash.as_slice()).expect("keccak256 returns 32 bytes")
    }

    fn encode(&self, chain_id: U256) -> Data;

    fn try_into_signed(
        self,
        signature: secp256k1::Signature,
        chain_id: U256,
    ) -> SigningResult<Self::SignedTransaction>;
}

pub trait SignedTransaction: TransactionCommon {
    type Signature: EthSignature;

    fn encode(&self) -> Data;

    fn signature(&self) -> &Self::Signature;
}

pub trait UnsignedTransactionBox: TransactionCommon {
    fn into_boxed(self) -> Box<dyn UnsignedTransactionBox + 'static>
    where
        Self: Sized + 'static,
    {
        Box::new(self)
    }

    fn pre_hash(&self, chain_id: U256) -> H256;

    fn encode(&self, chain_id: U256) -> Data;

    fn try_into_signed(
        self: Box<Self>,
        signature: secp256k1::Signature,
        chain_id: U256,
    ) -> SigningResult<Box<dyn SignedTransactionBox>>;
}

impl<T> UnsignedTransactionBox for T
where
    T: UnsignedTransaction,
{
    fn pre_hash(&self, chain_id: U256) -> H256 {
        <Self as UnsignedTransaction>::pre_hash(self, chain_id)
    }

    fn encode(&self, chain_id: U256) -> Data {
        <Self as UnsignedTransaction>::encode(self, chain_id)
    }

    fn try_into_signed(
        self: Box<Self>,
        signature: secp256k1::Signature,
        chain_id: U256,
    ) -> SigningResult<Box<dyn SignedTransactionBox>> {
        let signed = <Self as UnsignedTransaction>::try_into_signed(*self, signature, chain_id)?;
        Ok(Box::new(signed))
    }
}

pub trait SignedTransactionBox: TransactionCommon {
    fn encode(&self) -> Data;

    fn signature(&self) -> &dyn EthSignature;
}

impl<T> SignedTransactionBox for T
where
    T: SignedTransaction,
{
    fn encode(&self) -> Data {
        <Self as SignedTransaction>::encode(self)
    }

    fn signature(&self) -> &dyn EthSignature {
        <Self as SignedTransaction>::signature(self)
    }
}
