// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use crate::rlp::address::RlpAddressOption;
use crate::rlp::u256::RlpU256;
use crate::transaction::signature::{EthSignature, Signature};
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use rlp::{RlpStream, EMPTY_LIST_RLP};
use tw_keypair::ecdsa::secp256k1;
use tw_number::U256;

const EIP1559_TX_TYPE: u8 = 0x02;
const TX_FIELDS_LEN: usize = 9;
const TX_FIELDS_WITH_SIGNATURE_LEN: usize = TX_FIELDS_LEN + 3;

pub struct TransactionEip1559 {
    pub nonce: U256,
    pub max_inclusion_fee_per_gas: U256,
    pub max_fee_per_gas: U256,
    pub gas_limit: U256,
    pub to: Option<Address>,
    pub amount: U256,
    pub payload: Vec<u8>,
}

impl TransactionCommon for TransactionEip1559 {
    fn payload(&self) -> Vec<u8> {
        self.payload.clone()
    }
}

impl UnsignedTransaction for TransactionEip1559 {
    type SignedTransaction = SignedTransactionEip1559;

    fn encode(&self, chain_id: U256) -> Vec<u8> {
        encode_transaction(self, chain_id, None)
    }

    fn into_signed(
        self,
        signature: secp256k1::Signature,
        chain_id: U256,
    ) -> Self::SignedTransaction {
        SignedTransactionEip1559 {
            unsigned: self,
            signature: Signature::new(signature),
            chain_id,
        }
    }
}

pub struct SignedTransactionEip1559 {
    unsigned: TransactionEip1559,
    signature: Signature,
    chain_id: U256,
}

impl TransactionCommon for SignedTransactionEip1559 {
    fn payload(&self) -> Vec<u8> {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedTransactionEip1559 {
    type Signature = Signature;

    fn encode(&self) -> Vec<u8> {
        encode_transaction(&self.unsigned, self.chain_id, Some(&self.signature))
    }

    fn signature(&self) -> &Self::Signature {
        &self.signature
    }
}

fn encode_transaction(
    tx: &TransactionEip1559,
    chain_id: U256,
    signature: Option<&Signature>,
) -> Vec<u8> {
    let len = match signature {
        Some(_) => TX_FIELDS_WITH_SIGNATURE_LEN,
        None => TX_FIELDS_LEN,
    };

    let mut stream = RlpStream::new_list(len);
    stream
        .append(&RlpU256::from(chain_id))
        .append(&RlpU256::from(tx.nonce))
        .append(&RlpU256::from(tx.max_inclusion_fee_per_gas))
        .append(&RlpU256::from(tx.max_fee_per_gas))
        .append(&RlpU256::from(tx.gas_limit))
        .append(&RlpAddressOption::from(tx.to))
        .append(&RlpU256::from(tx.amount))
        .append(&tx.payload.as_slice())
        // empty `access_list`.
        .append_raw(&EMPTY_LIST_RLP[..], 1);

    if let Some(signature) = signature {
        stream
            .append(&RlpU256::from(signature.v()))
            .append(&RlpU256::from(signature.r()))
            .append(&RlpU256::from(signature.s()));
    }

    let tx_encoded = stream.out();

    let mut envelope = Vec::with_capacity(tx_encoded.len() + 1);
    envelope.push(EIP1559_TX_TYPE);
    envelope.extend_from_slice(&tx_encoded);
    envelope
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex;

    #[test]
    fn test_encode_transaction_eip1559() {
        let tx = TransactionEip1559 {
            nonce: U256::from(6u64),
            max_inclusion_fee_per_gas: U256::from(2_000_000_000u64),
            max_fee_per_gas: U256::from(3_000_000_000u64),
            gas_limit: U256::from(21100u32),
            to: Some(Address::from("0x6b175474e89094c44da98b954eedeac495271d0f")),
            amount: U256::zero(),
            payload: hex::decode("a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1").unwrap(),
        };
        let chain_id = U256::from(10u64);
        let actual = tx.encode(chain_id);

        let expected = "02f86c0a06847735940084b2d05e0082526c946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1c0";
        assert_eq!(hex::encode(actual, false), expected);
    }
}
