// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_keypair::KeyPairError;

pub type Result<T> = core::result::Result<T, Error>;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum Error {
    /// BIP32 error
    BIP32(bip32::Error),

    /// Unsupported curve
    UnsupportedCurve(u32),

    /// Invalid length
    InvalidLength,

    /// Decode error
    Decode,

    /// Invalid key data
    InvalidKeyData,

    /// Invalid checksum
    InvalidChecksum,

    /// Base58 error
    Base58,

    /// Invalid depth
    InvalidDepth,

    /// Invalid child number
    InvalidChildNumber,

    /// Derivation failed
    DerivationFailed,

    /// Invalid derivation path
    InvalidDerivationPath,

    /// Invalid chain code
    InvalidChainCode,
    
    /// InvalidDomainSeparator
    InvalidDomainSeparator,
}

impl From<bip32::Error> for Error {
    fn from(error: bip32::Error) -> Error {
        Error::BIP32(error)
    }
}

impl From<core::array::TryFromSliceError> for Error {
    fn from(_: core::array::TryFromSliceError) -> Error {
        Error::Decode
    }
}

impl From<tw_encoding::hex::FromHexError> for Error {
    fn from(_: tw_encoding::hex::FromHexError) -> Error {
        Error::Decode
    }
}

impl From<bs58::decode::Error> for Error {
    fn from(_: bs58::decode::Error) -> Error {
        Error::Base58
    }
}

impl From<bs58::encode::Error> for Error {
    fn from(_: bs58::encode::Error) -> Error {
        Error::Base58
    }
}

impl From<KeyPairError> for Error {
    fn from(_: KeyPairError) -> Error {
        Error::InvalidKeyData
    }
}
