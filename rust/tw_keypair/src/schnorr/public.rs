use crate::schnorr::{bitcoin_tweak, Signature};
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use bitcoin::key::TapTweak;
use secp256k1::SECP256K1;
use tw_hash::{H256, H264};
use tw_misc::traits::ToBytesVec;

#[derive(Clone, Debug, PartialEq)]
pub struct PublicKey {
    pub(crate) public: secp256k1::PublicKey,
}

impl PublicKey {
    pub fn compressed(&self) -> H264 {
        H264::from(self.public.serialize())
    }

    pub fn tweak(&self, tweak: Option<H256>) -> PublicKey {
        let tweak = bitcoin_tweak(tweak);

        let (x_only_pubkey, _parity) = self.public.x_only_public_key();
        let (tweaked_pubkey, tweaked_parity) = x_only_pubkey.tap_tweak(SECP256K1, tweak);

        PublicKey {
            public: secp256k1::PublicKey::from_x_only_public_key(
                tweaked_pubkey.to_inner(),
                tweaked_parity,
            ),
        }
    }

    pub fn x_only(&self) -> XOnlyPublicKey {
        let (x_only_pubkey, _parity) = self.public.x_only_public_key();
        XOnlyPublicKey {
            public: x_only_pubkey,
        }
    }
}

impl VerifyingKeyTrait for PublicKey {
    type SigningMessage = H256;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.x_only().verify(signature, message)
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

pub struct XOnlyPublicKey {
    pub(crate) public: secp256k1::XOnlyPublicKey,
}

impl XOnlyPublicKey {
    pub fn bytes(&self) -> H256 {
        H256::from(self.public.serialize())
    }
}

impl VerifyingKeyTrait for XOnlyPublicKey {
    type SigningMessage = H256;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let message = secp256k1::Message::from_slice(message.as_slice())
            .expect("Expected a valid secp256k1 message");

        signature.signature.verify(&message, &self.public).is_ok()
    }
}

impl<'a> TryFrom<&'a [u8]> for XOnlyPublicKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        let x_only_slice = if value.len() == H264::LEN {
            // Drop the first parity byte.
            &value[1..]
        } else {
            // Otherwise it should be `H256::LEN`. It will be checked later at [`secp256k1::XOnlyPublicKey::from_slice`].
            value
        };

        let public = secp256k1::XOnlyPublicKey::from_slice(x_only_slice)
            .map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(XOnlyPublicKey { public })
    }
}
