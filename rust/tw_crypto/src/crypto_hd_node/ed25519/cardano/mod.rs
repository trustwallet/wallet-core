// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

mod common;
mod derivation;
mod key;
mod v2;

use std::str::FromStr;

use bip32::{ChainCode, ChildNumber, DerivationPath};
use derivation::DerivationScheme;
use key::{XPRV_SIZE, XPUB_SIZE};
use tw_keypair::{ed25519, tw::Curve};
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};
use zeroize::Zeroizing;

use crate::crypto_hd_node::error::{Error, Result};
use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, bip32_public_key::BIP32PublicKey,
};

impl BIP32PrivateKey for ed25519::cardano::ExtendedPrivateKey {
    type BIP32PublicKey = ed25519::cardano::ExtendedPublicKey;

    fn derive_tweak(
        &self,
        chain_code: &bip32::ChainCode,
        _child_number: bip32::ChildNumber,
    ) -> Result<(Zeroizing<Vec<u8>>, bip32::ChainCode)> {
        Ok((Zeroizing::new(vec![]), *chain_code))
    }

    fn derive_child(&self, _other: &[u8], child_number: ChildNumber) -> Result<Self> {
        let bytes = self.to_zeroizing_vec();
        let bytes: [u8; XPRV_SIZE] = bytes.as_slice()[..XPRV_SIZE]
            .try_into()
            .expect("Should not fail");
        let bip32_xpr = key::XPrv::from_bytes_verified(bytes).map_err(|_| Error::InvalidKeyData)?;
        let child: key::XPrv = bip32_xpr.derive(DerivationScheme::V2, child_number.0);
        Self::try_from(child.as_ref()).map_err(|_| Error::InvalidKeyData)
    }

    fn curve() -> Curve {
        Curve::Ed25519ExtendedCardano
    }

    fn bip32_name() -> &'static str {
        "ed25519 cardano seed"
    }

    fn public_key(&self) -> Self::BIP32PublicKey {
        self.public()
    }
}

impl BIP32PublicKey for ed25519::cardano::ExtendedPublicKey {
    fn curve() -> Curve {
        Curve::Ed25519ExtendedCardano
    }

    fn derive_child(
        &self,
        _chain_code: &ChainCode,
        child_number: ChildNumber,
    ) -> Result<(Self, ChainCode)> {
        let bytes = self.to_vec();
        let bytes: [u8; XPUB_SIZE] = bytes[..XPUB_SIZE].try_into().expect("Should not fail");
        let bip32_xpub = key::XPub::from_bytes(bytes);
        let child: key::XPub = bip32_xpub
            .derive(DerivationScheme::V2, child_number.0)
            .map_err(|_| Error::DerivationFailed)?;
        let public_key = Self::try_from(child.as_ref()).map_err(|_| Error::InvalidKeyData)?;
        let chaincode = *child.chain_code();
        Ok((public_key, chaincode))
    }
}

pub fn cardano_staking_derivation_path(path: &DerivationPath) -> Result<DerivationPath> {
    if path.len() < 4 {
        return Err(Error::InvalidDerivationPath);
    }
    let mut staking_path = DerivationPath::from_str("m")?;
    for (i, item) in path.iter().enumerate() {
        if i == 3 {
            if item.index() > 1 {
                return Err(Error::InvalidDerivationPath);
            }
            staking_path.push(ChildNumber::new(2, false)?);
        } else if i == 4 {
            staking_path.push(ChildNumber::new(0, false)?);
        } else {
            staking_path.push(item);
        }
    }
    Ok(staking_path)
}
