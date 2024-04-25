use tw_encoding::hex;
use tw_hash::H256;
use zeroize::Zeroizing;

use crate::schnorr::private::PrivateKey;
use crate::schnorr::public::PublicKey;
use crate::schnorr::signature::Signature;
use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use crate::KeyPairError;

pub struct KeyPair {
    private: PrivateKey,
    public: PublicKey,
}

impl KeyPair {
    /// Disable auxiliary random data when signing. ONLY recommended for testing.
    pub fn no_aux_rand(self) -> KeyPair {
        KeyPair {
            private: self.private.no_aux_rand(),
            public: self.public,
        }
    }
}

impl KeyPairTrait for KeyPair {
    type Private = PrivateKey;
    type Public = PublicKey;

    fn public(&self) -> &Self::Public {
        &self.public
    }

    fn private(&self) -> &Self::Private {
        &self.private
    }
}

impl SigningKeyTrait for KeyPair {
    type SigningMessage = H256;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> crate::KeyPairResult<Self::Signature> {
        self.private.sign(message)
    }
}

impl VerifyingKeyTrait for KeyPair {
    type SigningMessage = H256;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.public.verify(signature, message)
    }
}

impl<'a> TryFrom<&'a [u8]> for KeyPair {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        let private = PrivateKey::try_from(value)?;
        let public = private.public();
        Ok(KeyPair { private, public })
    }
}

impl<'a> TryFrom<&'a str> for KeyPair {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = Zeroizing::new(hex::decode(hex).map_err(|_| KeyPairError::InvalidSecretKey)?);
        Self::try_from(bytes.as_slice())
    }
}
