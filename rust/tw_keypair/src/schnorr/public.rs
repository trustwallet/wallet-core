use crate::schnorr::Signature;
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use tw_hash::{H256, H264};
use tw_misc::traits::ToBytesVec;

#[derive(Clone, PartialEq)]
pub struct PublicKey {
    pub(crate) public: secp256k1::PublicKey,
}

impl PublicKey {
    pub fn compressed(&self) -> H264 {
        H264::from(self.public.serialize())
    }
}

impl VerifyingKeyTrait for PublicKey {
    type SigningMessage = H256;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let message = secp256k1::Message::from_slice(message.as_slice())
            .expect("Expected a valid secp256k1 message");

        let (x_only_pubkey, _parity) = self.public.x_only_public_key();
        signature.signature.verify(&message, &x_only_pubkey).is_ok()
    }
}

impl ToBytesVec for PublicKey {
    fn to_vec(&self) -> Vec<u8> {
        self.public.serialize().to_vec()
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        let public =
            secp256k1::PublicKey::from_slice(value).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(PublicKey { public })
    }
}
