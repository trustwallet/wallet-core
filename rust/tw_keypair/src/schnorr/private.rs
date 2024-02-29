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

enum IsTweaked {
    Some(Option<bitcoin::taproot::TapNodeHash>),
    None,
}

pub struct PrivateKey {
    pub(crate) secret: SigningKey,
    _tweak: IsTweaked,
}

impl PrivateKey {
    pub fn public(&self) -> PublicKey {
        PublicKey::new(self.secret.verifying_key().clone())
    }
    #[cfg(feature = "tweak-support")]
    pub fn tweak(self, tweak: Option<&H256>) -> PrivateKey {
        use bitcoin::hashes::Hash;

        let tweak = if let Some(tweak) = tweak {
            let hash = bitcoin::hashes::sha256t::Hash::<_>::from_slice(tweak.as_slice()).unwrap();
            Some(bitcoin::taproot::TapNodeHash::from_raw_hash(hash))
        } else {
            None
        };

        PrivateKey {
            secret: self.secret,
            _tweak: IsTweaked::Some(tweak),
        }
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = H256;
    type Signature = Signature;

    #[cfg(not(feature = "tweak-support"))]
    fn sign(&self, message: Self::SigningMessage) -> crate::KeyPairResult<Self::Signature> {
        Ok(Signature::new(self.secret.sign(message.as_slice())))
    }

    #[cfg(feature = "tweak-support")]
    fn sign(&self, message: Self::SigningMessage) -> crate::KeyPairResult<Self::Signature> {
        use bitcoin::key::TapTweak;

        if let IsTweaked::Some(tweak) = self._tweak {
            // We fully rely on the `bitcoin` and `secp256k1` crates for those steps.

            // Tweak the private key.
            let secp = secp256k1::Secp256k1::new();
            let keypair = secp256k1::KeyPair::from_seckey_slice(
                &secp,
                self.secret.to_bytes().as_slice(),
            )
            .unwrap();
            // TODO: If we implement `tap_tweak` ourselves, we can avoid the `bitcoin` crate.
            let tapped = keypair.tap_tweak(&secp, tweak);
            let tweaked = secp256k1::KeyPair::from(tapped);

            // Sign the message.
            let msg = secp256k1::Message::from_slice(message.as_slice()).unwrap();
            let sig = tweaked.sign_schnorr(msg);

            Ok(Signature::from_bytes(sig.as_ref()).unwrap())
        } else {
            Ok(Signature::new(self.secret.sign(message.as_slice())))
        }
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
        Self {
            secret,
            _tweak: IsTweaked::None,
        }
    }
}
