use crate::schnorr::public::PublicKey;
use crate::schnorr::signature::Signature;
use crate::traits::SigningKeyTrait;
use crate::KeyPairError;
use ecdsa::signature::Signer;
use k256::schnorr::SigningKey;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};
use zeroize::Zeroizing;

pub struct PrivateKey {
    pub(crate) secret: SigningKey,
}

impl PrivateKey {
    pub fn public(&self) -> PublicKey {
        PublicKey::new(self.secret.verifying_key().clone())
    }
    pub fn tweaked_public(&self) {
        // let tweaked = pk.as_affine() * *sk.to_nonzero_scalar();

        // let tweaked_pt = *pk.as_affine() * *sk.to_nonzero_scalar();
        // let tweaked_pk = k256::PublicKey::from_affine(tweaked_pt.to_affine())?;

        let pk = self.secret.verifying_key();
        let tweaked_pt = *pk.as_affine() * self.secret.as_nonzero_scalar().as_ref();
        let tweaked_pk = k256::PublicKey::from_affine(tweaked_pt.to_affine()).unwrap();
        todo!()
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = H256;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> crate::KeyPairResult<Self::Signature> {
        Ok(Signature::new(self.secret.sign(message.as_slice())))
    }
}

impl ToBytesZeroizing for PrivateKey {
    fn to_zeroizing_vec(&self) -> zeroize::Zeroizing<Vec<u8>> {
        Zeroizing::new(self.secret.to_bytes().to_vec())
    }
}

impl<'a> TryFrom<&'a str> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(s: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(s).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(PrivateKey::try_from(bytes.as_slice())?)
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        Ok(PrivateKey::from(
            SigningKey::from_bytes(value).map_err(|_| KeyPairError::InvalidPublicKey)?,
        ))
    }
}

impl From<k256::schnorr::SigningKey> for PrivateKey {
    fn from(secret: k256::schnorr::SigningKey) -> Self {
        Self { secret }
    }
}
