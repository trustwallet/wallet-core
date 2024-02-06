use crate::{traits::VerifyingKeyTrait, KeyPairError};
use ecdsa::signature::Verifier;
use k256::schnorr::{Signature, VerifyingKey};
use tw_encoding::hex;
use tw_hash::H256;

pub struct PublicKey {
    pub(crate) public: VerifyingKey,
}

impl PublicKey {
    pub(crate) fn new(public: VerifyingKey) -> PublicKey {
        PublicKey { public }
    }
}

impl VerifyingKeyTrait for PublicKey {
    type SigningMessage = H256;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.public.verify(message.as_slice(), &signature).is_ok()
    }
}

impl<'a> TryFrom<&'a str> for PublicKey {
    type Error = KeyPairError;

    fn try_from(s: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(s).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(PublicKey::try_from(bytes.as_slice())?)
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        Ok(PublicKey::new(
            VerifyingKey::from_bytes(value).map_err(|_| KeyPairError::InvalidPublicKey)?,
        ))
    }
}
