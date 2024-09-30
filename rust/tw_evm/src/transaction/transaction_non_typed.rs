// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::list::RlpList;
use crate::transaction::signature::{EthSignature, SignatureEip155};
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use tw_coin_entry::error::prelude::*;
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;
use tw_number::U256;

/// Original transaction format, with no explicit type, legacy as pre-EIP2718.
pub struct TransactionNonTyped {
    pub nonce: U256,
    pub gas_price: U256,
    pub gas_limit: U256,
    pub to: Option<Address>,
    pub amount: U256,
    pub payload: Data,
}

impl TransactionCommon for TransactionNonTyped {
    #[inline]
    fn payload(&self) -> Data {
        self.payload.to_vec()
    }
}

impl UnsignedTransaction for TransactionNonTyped {
    type SignedTransaction = SignedTransactionNonTyped;

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
        Ok(SignedTransactionNonTyped {
            unsigned: self,
            signature: SignatureEip155::new(signature, chain_id)?,
            chain_id,
        })
    }
}

pub struct SignedTransactionNonTyped {
    unsigned: TransactionNonTyped,
    signature: SignatureEip155,
    chain_id: U256,
}

impl TransactionCommon for SignedTransactionNonTyped {
    #[inline]
    fn payload(&self) -> Data {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedTransactionNonTyped {
    type Signature = SignatureEip155;

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
    tx: &TransactionNonTyped,
    chain_id: U256,
    signature: Option<&SignatureEip155>,
) -> Data {
    let mut list = RlpList::new();
    list.append(&tx.nonce)
        .append(&tx.gas_price)
        .append(&tx.gas_limit)
        .append(&tx.to)
        .append(&tx.amount)
        .append(tx.payload.as_slice());

    let (v, r, s) = match signature {
        Some(sign) => (sign.v(), sign.r(), sign.s()),
        None => (chain_id, U256::zero(), U256::zero()),
    };
    list.append(&v).append(&r).append(&s);
    list.finish()
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex;

    #[test]
    fn test_encode_unsigned_non_typed_zero() {
        let tx = TransactionNonTyped {
            nonce: U256::zero(),
            gas_price: U256::zero(),
            gas_limit: U256::zero(),
            to: Some(Address::default()),
            amount: U256::zero(),
            payload: Vec::new(),
        };
        let chain_id = U256::zero();
        let actual = tx.encode(chain_id);

        let expected = "dd8080809400000000000000000000000000000000000000008080808080";
        assert_eq!(hex::encode(actual, false), expected);
    }

    #[test]
    fn test_encode_unsigned_non_typed() {
        let tx = TransactionNonTyped {
            nonce: U256::zero(),
            gas_price: U256::from(42_000_000_000_u64),
            gas_limit: U256::from(78009_u32),
            to: Some(Address::from("0x6b175474e89094c44da98b954eedeac495271d0f")),
            amount: U256::zero(),
            payload: hex::decode("a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000").unwrap(),
        };
        let chain_id = U256::from(10_u8);
        let actual = tx.encode(chain_id);

        let expected = "f86a808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec800000a8080";
        assert_eq!(hex::encode(actual, false), expected);
    }

    #[test]
    fn test_encode_unsigned_non_typed_pre_hash() {
        let tx = TransactionNonTyped {
            nonce: U256::from(9_u64),
            gas_price: U256::from(20_000_000_000_u64),
            gas_limit: U256::from(21_000_u64),
            to: Some(Address::from("0x3535353535353535353535353535353535353535")),
            amount: U256::from(1_000_000_000_000_000_000_u64),
            payload: Vec::default(),
        };
        let chain_id = U256::from(1_u64);
        let actual = tx.pre_hash(chain_id);

        let expected = "daf5a779ae972f972197303d7b574746c7ef83eadac0f2791ad23db92e4c8e53";
        assert_eq!(hex::encode(actual, false), expected);
    }
}
