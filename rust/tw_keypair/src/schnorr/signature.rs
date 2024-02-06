use crate::{KeyPairError, KeyPairResult};
use tw_misc::traits::ToBytesVec;

pub struct Signature {
    pub(crate) signature: k256::schnorr::Signature,
}

impl Signature {
    pub const LEN: usize = k256::schnorr::Signature::BYTE_SIZE;

    pub(crate) fn new(signature: k256::schnorr::Signature) -> Self {
        Self { signature }
    }

    pub const fn len() -> usize {
        Self::LEN
    }

    pub fn from_bytes(sig: &[u8]) -> KeyPairResult<Self> {
        Ok(Signature {
            signature: k256::schnorr::Signature::try_from(sig)
                .map_err(|_| KeyPairError::InvalidSignature)?,
        })
    }
}

impl ToBytesVec for Signature {
    fn to_vec(&self) -> Vec<u8> {
        self.signature.to_bytes().to_vec()
    }
}

impl<'a> TryFrom<&'a [u8]> for Signature {
    type Error = KeyPairError;

    fn try_from(sig: &'a [u8]) -> Result<Self, Self::Error> {
        Signature::from_bytes(sig)
    }
}

impl From<k256::schnorr::Signature> for Signature {
    fn from(sig: k256::schnorr::Signature) -> Self {
        Self::new(sig)
    }
}
