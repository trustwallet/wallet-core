use crate::schnorr::secp256k1::public::PublicKey;
use crate::traits::SigningKeyTrait;
use ecdsa::signature::Signer;
use k256::schnorr::{Signature, SigningKey};
use tw_hash::H256;

pub struct PrivateKey {
    pub(crate) secret: SigningKey,
}

impl PrivateKey {
    pub fn public(&self) -> PublicKey {
        PublicKey::new(self.secret.verifying_key().clone())
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = H256;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> crate::KeyPairResult<Self::Signature> {
        Ok(self.secret.sign(message.as_slice()))
    }
}
