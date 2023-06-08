use std::str::FromStr;

use crate::{tweak_pubkey, Error, Recipient, Result};
use bitcoin::key::{KeyPair, PublicKey, TweakedPublicKey};
use bitcoin::{PubkeyHash, WPubkeyHash};

// TODO: Needed?
impl Recipient<PubkeyHash> {
    pub fn pubkey_hash(&self) -> &PubkeyHash {
        &self.t
    }
}

impl Recipient<WPubkeyHash> {
    pub fn wpubkey_hash(&self) -> &WPubkeyHash {
        &self.t
    }
}

impl Recipient<PublicKey> {
    pub fn from_slice(slice: &[u8]) -> Result<Self> {
        Ok(Recipient {
            t: PublicKey::from_slice(slice).map_err(|_| Error::Todo)?,
        })
    }
}

impl FromStr for Recipient<PublicKey> {
    type Err = Error;

    fn from_str(string: &str) -> Result<Self> {
        Self::from_slice(string.as_bytes())
    }
}

impl From<PublicKey> for Recipient<PublicKey> {
    fn from(t: PublicKey) -> Self {
        Recipient { t }
    }
}

impl From<KeyPair> for Recipient<PublicKey> {
    fn from(keypair: KeyPair) -> Self {
        Self::from(&keypair)
    }
}

impl From<&KeyPair> for Recipient<PublicKey> {
    fn from(keypair: &KeyPair) -> Self {
        Recipient {
            t: PublicKey::new(keypair.public_key()),
        }
    }
}

impl From<PublicKey> for Recipient<TweakedPublicKey> {
    fn from(pubkey: PublicKey) -> Self {
        let tweaked = tweak_pubkey(pubkey);
        Recipient { t: tweaked }
    }
}

impl From<Recipient<PublicKey>> for Recipient<TweakedPublicKey> {
    fn from(recipient: Recipient<PublicKey>) -> Self {
        Recipient {
            t: Self::from(recipient.t).t,
        }
    }
}

impl From<KeyPair> for Recipient<TweakedPublicKey> {
    fn from(keypair: KeyPair) -> Self {
        Self::from(&keypair)
    }
}

impl From<&KeyPair> for Recipient<TweakedPublicKey> {
    fn from(keypair: &KeyPair) -> Self {
        let pk = Recipient::<PublicKey>::from(keypair);
        let tweaked = Recipient::<TweakedPublicKey>::from(pk);

        Recipient { t: tweaked.t }
    }
}

impl From<PublicKey> for Recipient<WPubkeyHash> {
    fn from(pubkey: PublicKey) -> Self {
        Recipient {
            // TODO: When does this fail?
            t: pubkey.wpubkey_hash().unwrap(),
        }
    }
}

impl From<Recipient<PublicKey>> for Recipient<WPubkeyHash> {
    fn from(recipient: Recipient<PublicKey>) -> Self {
        Recipient {
            t: Self::from(recipient.t).t,
        }
    }
}

impl From<&KeyPair> for Recipient<WPubkeyHash> {
    fn from(keypair: &KeyPair) -> Self {
        let pubkey = Recipient::<PublicKey>::from(keypair);
        Self::from(pubkey.t)
    }
}

impl From<KeyPair> for Recipient<WPubkeyHash> {
    fn from(keypair: KeyPair) -> Self {
        Self::from(&keypair)
    }
}

impl From<PublicKey> for Recipient<PubkeyHash> {
    fn from(pubkey: PublicKey) -> Self {
        Recipient { t: pubkey.into() }
    }
}

impl From<Recipient<PublicKey>> for Recipient<PubkeyHash> {
    fn from(recipient: Recipient<PublicKey>) -> Self {
        Recipient {
            t: Self::from(recipient.t).t,
        }
    }
}

impl From<KeyPair> for Recipient<PubkeyHash> {
    fn from(keypair: KeyPair) -> Self {
        Self::from(&keypair)
    }
}

impl From<&KeyPair> for Recipient<PubkeyHash> {
    fn from(keypair: &KeyPair) -> Self {
        let pk = Recipient::<PublicKey>::from(keypair);

        Recipient { t: pk.t.into() }
    }
}
