// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_hash::H520;
use tw_keypair::ecdsa::secp256k1;
use tw_number::U256;

pub trait EthSignature {
    fn v(&self) -> U256;

    fn r(&self) -> U256;

    fn s(&self) -> U256;
}

pub struct Signature {
    v: U256,
    r: U256,
    s: U256,
    rsv: H520,
}

impl Signature {
    pub fn new(sign: secp256k1::Signature) -> Self {
        Signature {
            v: U256::from(sign.v()),
            r: U256::from_little_endian(sign.r()),
            s: U256::from_little_endian(sign.s()),
            rsv: sign.to_bytes(),
        }
    }

    pub fn to_rsv_bytes(&self) -> H520 {
        self.rsv
    }
}

impl EthSignature for Signature {
    fn v(&self) -> U256 {
        self.v
    }

    fn r(&self) -> U256 {
        self.r
    }

    fn s(&self) -> U256 {
        self.s
    }
}

pub struct SignatureEip155 {
    v: U256,
    r: U256,
    s: U256,
}

impl SignatureEip155 {
    pub fn new(sign: secp256k1::Signature, chain_id: U256) -> Self {
        let v = replay_protection(chain_id, sign.v());
        SignatureEip155 {
            v,
            r: U256::from_little_endian(sign.r()),
            s: U256::from_little_endian(sign.s()),
        }
    }
}

impl EthSignature for SignatureEip155 {
    fn v(&self) -> U256 {
        self.v
    }

    fn r(&self) -> U256 {
        self.r
    }

    fn s(&self) -> U256 {
        self.s
    }
}

/// Embeds `chain_id` in `v` param, for replay protection, legacy (EIP155).
pub fn replay_protection(chain_id: U256, v: u8) -> U256 {
    if chain_id.is_zero() {
        U256::from(v + 27)
    } else {
        chain_id + chain_id + 35u8
    }
}
