// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signature::replay_protection;
use tw_hash::H520;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::{KeyPairError, KeyPairResult};
use tw_number::U256;

pub trait EthSignature {
    fn v(&self) -> U256;

    fn r(&self) -> U256;

    fn s(&self) -> U256;
}

/// R-S-V Signature values.
pub struct Signature {
    v: U256,
    r: U256,
    s: U256,
    rsv: H520,
}

impl Signature {
    #[inline]
    pub fn new(sign: secp256k1::Signature) -> Self {
        Signature {
            v: U256::from(sign.v()),
            r: U256::from_big_endian(sign.r()),
            s: U256::from_big_endian(sign.s()),
            rsv: sign.to_bytes(),
        }
    }

    #[inline]
    pub fn to_rsv_bytes(&self) -> H520 {
        self.rsv
    }
}

impl EthSignature for Signature {
    #[inline]
    fn v(&self) -> U256 {
        self.v
    }

    #[inline]
    fn r(&self) -> U256 {
        self.r
    }

    #[inline]
    fn s(&self) -> U256 {
        self.s
    }
}

/// R-S-V Signature values EIP115.
pub struct SignatureEip155 {
    v: U256,
    r: U256,
    s: U256,
}

impl SignatureEip155 {
    #[inline]
    pub fn new(sign: secp256k1::Signature, chain_id: U256) -> KeyPairResult<Self> {
        let v =
            replay_protection(chain_id, sign.v()).map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(SignatureEip155 {
            v,
            r: U256::from_big_endian(sign.r()),
            s: U256::from_big_endian(sign.s()),
        })
    }
}

impl EthSignature for SignatureEip155 {
    #[inline]
    fn v(&self) -> U256 {
        self.v
    }

    #[inline]
    fn r(&self) -> U256 {
        self.r
    }

    #[inline]
    fn s(&self) -> U256 {
        self.s
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex;
    use tw_hash::H256;

    #[test]
    fn test_signature_no_eip155() {
        let secp_bytes = hex::decode("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a00").unwrap();
        let secp_sign = secp256k1::Signature::from_bytes(secp_bytes.as_slice()).unwrap();

        let eth_sign = Signature::new(secp_sign);

        let r = H256::from("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47");
        let s = H256::from("786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a");
        let v = hex::decode("00").unwrap();

        assert_eq!(eth_sign.r(), U256::from_big_endian(r));
        assert_eq!(eth_sign.s(), U256::from_big_endian(s));
        assert_eq!(
            eth_sign.v(),
            U256::from_big_endian_slice(v.as_slice()).unwrap()
        );
    }

    #[test]
    fn test_signature_eip155_legacy() {
        let secp_bytes = hex::decode("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a00").unwrap();
        let secp_sign = secp256k1::Signature::from_bytes(secp_bytes.as_slice()).unwrap();

        let chain_id = U256::zero();
        let eth_sign = SignatureEip155::new(secp_sign, chain_id).unwrap();

        let r = H256::from("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47");
        let s = H256::from("786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a");
        let v = hex::decode("1b").unwrap();

        assert_eq!(eth_sign.r(), U256::from_big_endian(r));
        assert_eq!(eth_sign.s(), U256::from_big_endian(s));
        assert_eq!(
            eth_sign.v(),
            U256::from_big_endian_slice(v.as_slice()).unwrap()
        );
    }

    #[test]
    fn test_signature_eip155_v0() {
        let secp_bytes = hex::decode("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a00").unwrap();
        let secp_sign = secp256k1::Signature::from_bytes(secp_bytes.as_slice()).unwrap();

        let chain_id = U256::from(1_u64);
        let eth_sign = SignatureEip155::new(secp_sign, chain_id).unwrap();

        let r = H256::from("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47");
        let s = H256::from("786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a");
        let v = hex::decode("25").unwrap();

        assert_eq!(eth_sign.r(), U256::from_big_endian(r));
        assert_eq!(eth_sign.s(), U256::from_big_endian(s));
        assert_eq!(
            eth_sign.v(),
            U256::from_big_endian_slice(v.as_slice()).unwrap()
        );
    }

    #[test]
    fn test_signature_eip155_v1() {
        let secp_bytes = hex::decode("42556c4f2a3f4e4e639cca524d1da70e60881417d4643e5382ed110a52719eaf172f591a2a763d0bd6b13d042d8c5eb66e87f129c9dc77ada66b6041012db2b301").unwrap();
        let secp_sign = secp256k1::Signature::from_bytes(secp_bytes.as_slice()).unwrap();

        let chain_id = U256::from(1_u64);
        let eth_sign = SignatureEip155::new(secp_sign, chain_id).unwrap();

        let r = H256::from("42556c4f2a3f4e4e639cca524d1da70e60881417d4643e5382ed110a52719eaf");
        let s = H256::from("172f591a2a763d0bd6b13d042d8c5eb66e87f129c9dc77ada66b6041012db2b3");
        let v = hex::decode("26").unwrap();

        assert_eq!(eth_sign.r(), U256::from_big_endian(r));
        assert_eq!(eth_sign.s(), U256::from_big_endian(s));
        assert_eq!(
            eth_sign.v(),
            U256::from_big_endian_slice(v.as_slice()).unwrap()
        );
    }
}
