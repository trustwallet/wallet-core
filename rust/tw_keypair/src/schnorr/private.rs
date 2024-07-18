use crate::schnorr::bitcoin_tweak;
use crate::schnorr::public::PublicKey;
use crate::schnorr::signature::Signature;
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use bitcoin::key::TapTweak;
use secp256k1::SECP256K1;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};
use zeroize::{Zeroize, Zeroizing};

/// Represents a `schnorr` private key.
#[derive(Clone)]
pub struct PrivateKey {
    key_pair: secp256k1::KeyPair,
    no_aux_rand: bool,
}

impl PrivateKey {
    pub fn public(&self) -> PublicKey {
        PublicKey {
            public: self.key_pair.public_key(),
        }
    }

    /// Tweak the private key with a given hash.
    /// Note that the private key can be tweaked with a `None` value.
    pub fn tweak(self, tweak: Option<H256>) -> PrivateKey {
        let tweak = bitcoin_tweak(tweak);

        // Tweak the private key.
        let tweaked = self.key_pair.tap_tweak(SECP256K1, tweak);
        PrivateKey {
            key_pair: secp256k1::KeyPair::from(tweaked),
            no_aux_rand: self.no_aux_rand,
        }
    }

    /// Disable auxiliary random data when signing. ONLY recommended for testing.
    pub fn no_aux_rand(mut self) -> PrivateKey {
        self.no_aux_rand = true;
        self
    }
}

impl Zeroize for PrivateKey {
    fn zeroize(&mut self) {
        self.key_pair.non_secure_erase();
    }
}

impl Drop for PrivateKey {
    fn drop(&mut self) {
        self.zeroize();
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = H256;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        // We fully rely on the `bitcoin` and `secp256k1` crates to generate Schnorr signatures.

        // TODO consider checking `Utxo.leaf_hash` like at
        // https://github.com/trustwallet/wallet-core/blob/43bf58c0c99d78789b5a11714ebc686b4268fa06/rust/tw_bitcoin/src/modules/signer.rs#L183

        // Sign the message.
        let msg = secp256k1::Message::from_slice(message.as_slice()).expect("");
        let sig = if self.no_aux_rand {
            SECP256K1.sign_schnorr_no_aux_rand(&msg, &self.key_pair)
        } else {
            SECP256K1.sign_schnorr(&msg, &self.key_pair)
        };

        Ok(Signature::from(sig))
    }
}

impl ToBytesZeroizing for PrivateKey {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        Zeroizing::new(self.key_pair.secret_bytes().to_vec())
    }
}

impl<'a> TryFrom<&'a str> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(s: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(s).map_err(|_| KeyPairError::InvalidPublicKey)?;
        PrivateKey::try_from(bytes.as_slice())
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        let key_pair = secp256k1::KeyPair::from_seckey_slice(SECP256K1, value)
            .map_err(|_| KeyPairError::InvalidSecretKey)?;
        Ok(PrivateKey {
            key_pair,
            no_aux_rand: false,
        })
    }
}
