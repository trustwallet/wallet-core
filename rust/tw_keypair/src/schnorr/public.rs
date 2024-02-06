use crate::schnorr::Signature;
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use ecdsa::signature::Verifier;
use k256::schnorr::VerifyingKey;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;

#[derive(Clone, PartialEq)]
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
        self.public
            .verify(message.as_slice(), &signature.signature)
            .is_ok()
    }
}

impl ToBytesVec for PublicKey {
    fn to_vec(&self) -> Vec<u8> {
        self.public.to_bytes().to_vec()
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

impl From<k256::schnorr::VerifyingKey> for PublicKey {
    fn from(public: k256::schnorr::VerifyingKey) -> Self {
        PublicKey::new(public)
    }
}
