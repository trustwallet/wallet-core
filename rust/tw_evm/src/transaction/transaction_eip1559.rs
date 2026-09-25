// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::list::RlpList;
use crate::transaction::access_list::AccessList;
use crate::transaction::signature::{EthSignature, Signature};
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use tw_coin_entry::error::prelude::*;
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
    pub access_list: AccessList,
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
    list.append(&chain_id)
        .append(&tx.nonce)
        .append(&tx.max_inclusion_fee_per_gas)
        .append(&tx.max_fee_per_gas)
        .append(&tx.gas_limit)
        .append(&tx.to)
        .append(&tx.amount)
        .append(tx.payload.as_slice())
        .append(&tx.access_list);

    if let Some(signature) = signature {
        list.append(&signature.v());
        list.append(&signature.r());
        list.append(&signature.s());
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
    use crate::transaction::access_list::Access;
    use tw_encoding::hex;
    use tw_hash::H256;

    #[test]
    fn test_encode_transaction_eip1559() {
        let tx = TransactionEip1559 {
            nonce: U256::from(6_u64),
            max_inclusion_fee_per_gas: U256::from(2_000_000_000_u64),
            max_fee_per_gas: U256::from(3_000_000_000_u64),
            gas_limit: U256::from(21100_u64),
            to: Some(Address::from("0x6b175474e89094c44da98b954eedeac495271d0f")),
            amount: U256::zero(),
            payload: hex::decode("a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1").unwrap(),
            access_list: AccessList::default(),
        };
        let chain_id = U256::from(10u64);
        let actual = tx.encode(chain_id);

        let expected = "02f86c0a06847735940084b2d05e0082526c946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1c0";
        assert_eq!(hex::encode(actual, false), expected);
    }

    #[test]
    fn test_encode_transaction_eip1559_with_access_list() {
        let mut access = Access::new(Address::from("0xdAC17F958D2ee523a2206206994597C13D831ec7"));

        #[rustfmt::skip]
        access
            .add_storage_key(H256::from("0x76c8f33bcdf467e4f1313522c10a40512a867cdcd34f2b898232ad4669200764"))
            .add_storage_key(H256::from("0x000000000000000000000000000000000000000000000000000000000000000a"))
            .add_storage_key(H256::from("0x0000000000000000000000000000000000000000000000000000000000000003"))
            .add_storage_key(H256::from("0x0000000000000000000000000000000000000000000000000000000000000004"))
            .add_storage_key(H256::from("0xb12459e057d0da4389f95b7ff0ce45a52ad71b02913a5466ffaab252e7ce918a"))
            .add_storage_key(H256::from("0x1bba044274699cc8c429fbe84bdad5d5a49519e29430f25309cbbab31dc63043"))
            .add_storage_key(H256::from("0x0000000000000000000000000000000000000000000000000000000000000000"));

        let mut access_list = AccessList::default();
        access_list.add_access(access);

        let tx = TransactionEip1559 {
            nonce: U256::from(1u64),
            max_inclusion_fee_per_gas: U256::from(2_000_000_000_u64),
            max_fee_per_gas: U256::from(3_000_000_000_u64),
            gas_limit: U256::from(100_000_u64),
            to: Some(Address::from("0xdAC17F958D2ee523a2206206994597C13D831ec7")),
            amount: U256::zero(),
            payload: hex::decode("a9059cbb000000000000000000000000b2fb4372e663b2e53da97d98100433d1fd06ca5500000000000000000000000000000000000000000000000000000000000f4240").unwrap(),
            access_list,
        };
        let chain_id = U256::from(1_u64);
        let actual = tx.encode(chain_id);

        let expected = "02f9016f0101847735940084b2d05e00830186a094dac17f958d2ee523a2206206994597c13d831ec780b844a9059cbb000000000000000000000000b2fb4372e663b2e53da97d98100433d1fd06ca5500000000000000000000000000000000000000000000000000000000000f4240f90100f8fe94dac17f958d2ee523a2206206994597c13d831ec7f8e7a076c8f33bcdf467e4f1313522c10a40512a867cdcd34f2b898232ad4669200764a0000000000000000000000000000000000000000000000000000000000000000aa00000000000000000000000000000000000000000000000000000000000000003a00000000000000000000000000000000000000000000000000000000000000004a0b12459e057d0da4389f95b7ff0ce45a52ad71b02913a5466ffaab252e7ce918aa01bba044274699cc8c429fbe84bdad5d5a49519e29430f25309cbbab31dc63043a00000000000000000000000000000000000000000000000000000000000000000";
        assert_eq!(hex::encode(actual, false), expected);
    }
}
