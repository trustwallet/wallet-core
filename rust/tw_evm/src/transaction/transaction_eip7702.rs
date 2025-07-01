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

/// EIP7702 transaction.
pub struct TransactionEip7702 {
    pub nonce: U256,
    pub max_inclusion_fee_per_gas: U256,
    pub max_fee_per_gas: U256,
    pub gas_limit: U256,
    pub to: Address,
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::transaction::authorization_list::{Authorization, SignedAuthorization};
    use std::str::FromStr;
    use tw_encoding::hex::{DecodeHex, ToHex};
    use tw_hash::H256;

    #[test]
    fn test_encode_transaction_eip7702() {
        let authorization = SignedAuthorization {
            authorization: Authorization {
                chain_id: U256::from(6_u32),
                address: Address::from_str("0x0202020202020202020202020202020202020202").unwrap(),
                nonce: U256::from(2_u32),
            },
            y_parity: 0,
            r: U256::from_str("0x42556c4f2a3f4e4e639cca524d1da70e60881417d4643e5382ed110a52719eaf")
                .unwrap(),
            s: U256::from_str("0x172f591a2a763d0bd6b13d042d8c5eb66e87f129c9dc77ada66b6041012db2b3")
                .unwrap(),
        };

        let tx = TransactionEip7702 {
            nonce: U256::from(1_u32),
            max_inclusion_fee_per_gas: U256::from(2_u32),
            max_fee_per_gas: U256::from(3_u32),
            gas_limit: U256::from(4_u32),
            to: Address::from_str("0x0101010101010101010101010101010101010101").unwrap(),
            amount: U256::from(5_u32),
            payload: "0x1234".decode_hex().unwrap(),
            access_list: AccessList::default(),
            authorization_list: AuthorizationList::from(vec![authorization]),
        };

        let r = H256::from("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47");
        let s = H256::from("786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a");
        let v = 0x00;
        let signature = secp256k1::Signature::try_from_parts(r, s, v).unwrap();
        let chain_id = U256::from(56_u32);
        let signed_tx = tx.try_into_signed(signature, chain_id).unwrap();

        assert_eq!(
            signed_tx.encode().to_hex(),
            "04f8c0380102030494010101010101010101010101010101010101010105821234c0f85cf85a069402020202020202020202020202020202020202020280a042556c4f2a3f4e4e639cca524d1da70e60881417d4643e5382ed110a52719eafa0172f591a2a763d0bd6b13d042d8c5eb66e87f129c9dc77ada66b6041012db2b380a0d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47a0786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a"
        );
    }
}
