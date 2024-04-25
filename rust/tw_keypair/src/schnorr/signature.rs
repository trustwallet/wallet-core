use crate::{KeyPairError, KeyPairResult};
use tw_misc::traits::ToBytesVec;

#[derive(Debug, PartialEq)]
pub struct Signature {
    pub(crate) signature: secp256k1::schnorr::Signature,
}

impl Signature {
    pub const LEN: usize = secp256k1::constants::SCHNORR_SIGNATURE_SIZE;

    pub(crate) fn new(signature: secp256k1::schnorr::Signature) -> Self {
        Self { signature }
    }

    pub const fn len() -> usize {
        Self::LEN
    }

    pub fn from_bytes(sig: &[u8]) -> KeyPairResult<Self> {
        Ok(Signature {
            signature: secp256k1::schnorr::Signature::from_slice(sig)
                .map_err(|_| KeyPairError::InvalidSignature)?,
        })
    }
}

impl ToBytesVec for Signature {
    fn to_vec(&self) -> Vec<u8> {
        self.signature.as_ref().to_vec()
    }
}

impl<'a> TryFrom<&'a [u8]> for Signature {
    type Error = KeyPairError;

    fn try_from(sig: &'a [u8]) -> Result<Self, Self::Error> {
        Signature::from_bytes(sig)
    }
}

impl From<secp256k1::schnorr::Signature> for Signature {
    fn from(sig: secp256k1::schnorr::Signature) -> Self {
        Self::new(sig)
    }
}
