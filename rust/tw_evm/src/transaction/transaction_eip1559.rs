// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::list::RlpList;
use crate::transaction::signature::{EthSignature, Signature};
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use tw_coin_entry::error::SigningResult;
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;
use tw_number::U256;

const EIP1559_TX_TYPE: u8 = 0x02;

/// EIP1559 transaction.
pub struct TransactionEip1559 {
    pub nonce: U256,
    pub max_inclusion_fee_per_gas: U256,
    pub max_fee_per_gas: U256,
    pub gas_limit: U256,
    pub to: Option<Address>,
    pub amount: U256,
    pub payload: Data,
}

impl TransactionCommon for TransactionEip1559 {
    #[inline]
    fn payload(&self) -> Data {
        self.payload.clone()
    }
}

impl UnsignedTransaction for TransactionEip1559 {
    type SignedTransaction = SignedTransactionEip1559;

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
        Ok(SignedTransactionEip1559 {
            unsigned: self,
            signature: Signature::new(signature),
            chain_id,
        })
    }
}

pub struct SignedTransactionEip1559 {
    unsigned: TransactionEip1559,
    signature: Signature,
    chain_id: U256,
}

impl TransactionCommon for SignedTransactionEip1559 {
    #[inline]
    fn payload(&self) -> Data {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedTransactionEip1559 {
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
    tx: &TransactionEip1559,
    chain_id: U256,
    signature: Option<&Signature>,
) -> Data {
    let mut list = RlpList::new();
    list.append(chain_id)
        .append(tx.nonce)
        .append(tx.max_inclusion_fee_per_gas)
        .append(tx.max_fee_per_gas)
        .append(tx.gas_limit)
        .append(tx.to)
        .append(tx.amount)
        .append(tx.payload.as_slice())
        // empty `access_list`.
        .append_empty_list();

    if let Some(signature) = signature {
        list.append(signature.v());
        list.append(signature.r());
        list.append(signature.s());
    }

    let tx_encoded = list.finish();

    let mut envelope = Vec::with_capacity(tx_encoded.len() + 1);
    envelope.push(EIP1559_TX_TYPE);
    envelope.extend_from_slice(tx_encoded.as_slice());
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
