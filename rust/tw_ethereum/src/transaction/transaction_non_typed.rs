// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use crate::rlp::u256::RlpU256;
use crate::transaction::signature::{EthSignature, SignatureEip155};
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use rlp::RlpStream;
use tw_keypair::ecdsa::secp256k1;
use tw_number::U256;

const TX_FIELDS_LEN: usize = 9;

pub struct TransactionNonTyped {
    pub nonce: U256,
    pub gas_price: U256,
    pub gas_limit: U256,
    pub to: Address,
    pub amount: U256,
    pub payload: Vec<u8>,
}

impl TransactionCommon for TransactionNonTyped {
    fn payload(&self) -> Vec<u8> {
        self.payload.to_vec()
    }
}

impl UnsignedTransaction for TransactionNonTyped {
    type SignedTransaction = SignedTransactionNonTyped;

    fn encode(&self, chain_id: U256) -> Vec<u8> {
        encode_transaction(self, chain_id, None)
    }

    fn into_signed(
        self,
        signature: secp256k1::Signature,
        chain_id: U256,
    ) -> Self::SignedTransaction {
        SignedTransactionNonTyped {
            unsigned: self,
            signature: SignatureEip155::new(signature, chain_id),
            chain_id,
        }
    }
}

pub struct SignedTransactionNonTyped {
    unsigned: TransactionNonTyped,
    signature: SignatureEip155,
    chain_id: U256,
}

impl TransactionCommon for SignedTransactionNonTyped {
    fn payload(&self) -> Vec<u8> {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedTransactionNonTyped {
    type Signature = SignatureEip155;

    fn encode(&self) -> Vec<u8> {
        encode_transaction(&self.unsigned, self.chain_id, Some(&self.signature))
    }

    fn signature(&self) -> &Self::Signature {
        &self.signature
    }
}

fn encode_transaction(
    tx: &TransactionNonTyped,
    chain_id: U256,
    signature: Option<&SignatureEip155>,
) -> Vec<u8> {
    let mut stream = RlpStream::new_list(TX_FIELDS_LEN);
    stream
        .append(&RlpU256::from(tx.nonce))
        .append(&RlpU256::from(tx.gas_price))
        .append(&RlpU256::from(tx.gas_limit))
        .append(&tx.to.as_slice())
        .append(&RlpU256::from(tx.amount))
        .append(&tx.payload.as_slice());

    let (v, r, s) = match signature {
        Some(sign) => (sign.v(), sign.r(), sign.s()),
        None => (chain_id, U256::zero(), U256::zero()),
    };

    stream
        .append(&RlpU256::from(v))
        .append(&RlpU256::from(r))
        .append(&RlpU256::from(s));
    stream.out().to_vec()
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
            to: Address::default(),
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
            gas_price: U256::from(42_000_000_000u64),
            gas_limit: U256::from(78009u32),
            to: Address::from("0x6b175474e89094c44da98b954eedeac495271d0f"),
            amount: U256::zero(),
            payload: hex::decode("a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000").unwrap(),
        };
        let chain_id = U256::from(10u8);
        let actual = tx.encode(chain_id);

        let expected = "f86a808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec800000a8080";
        assert_eq!(hex::encode(actual, false), expected);
    }
}
