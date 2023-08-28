use crate::modules::transactions::TaprootScript;
use crate::utils::tweak_pubkey;
use bitcoin::key::{KeyPair, PublicKey, TweakedPublicKey, UntweakedPublicKey};
use bitcoin::taproot::TapNodeHash;
use bitcoin::{secp256k1::XOnlyPublicKey, PubkeyHash, WPubkeyHash};

/// This type is used to specify the recipient of a Bitcoin transaction,
/// depending on the required information that's required. For example, P2PKH
/// only requires the public key hash (`Recipient<PubkeyHash>`), while P2TR
/// key-path requires the actual (tweaked) public key (`Recipient<TweakedPublicKey>`).
///
/// The recipient can easily downgrade, such as
/// ```rust,ignore
/// let pubkey = Recipient::<PublicKey>::from_keypair(keypair);
/// let hash: Recipient<PubkeyHash> = pubkey.into();
/// ```
///
/// But it cannot, for example, derive a public key from just the hash.
#[derive(Debug, Clone, Eq, PartialEq)]
pub struct Recipient<T> {
    inner: T,
}

impl Recipient<PublicKey> {
    pub fn from_keypair(keypair: &KeyPair) -> Self {
        Recipient {
            inner: PublicKey::new(keypair.public_key()),
        }
    }
    pub fn public_key(&self) -> PublicKey {
        self.inner
    }
    pub fn pubkey_hash(&self) -> PubkeyHash {
        PubkeyHash::from(self.inner)
    }
}

impl Recipient<PubkeyHash> {
    pub fn pubkey_hash(&self) -> &PubkeyHash {
        &self.inner
    }
}

impl Recipient<WPubkeyHash> {
    pub fn wpubkey_hash(&self) -> &WPubkeyHash {
        &self.inner
    }
}

impl From<PublicKey> for Recipient<PublicKey> {
    fn from(inner: PublicKey) -> Self {
        Recipient { inner }
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
            inner: PublicKey::new(keypair.public_key()),
        }
    }
}

impl From<PublicKey> for Recipient<TweakedPublicKey> {
    fn from(pubkey: PublicKey) -> Self {
        let tweaked = tweak_pubkey(pubkey);
        Recipient { inner: tweaked }
    }
}

impl From<Recipient<PublicKey>> for Recipient<TweakedPublicKey> {
    fn from(recipient: Recipient<PublicKey>) -> Self {
        Recipient {
            inner: Self::from(recipient.inner).inner,
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

        Recipient {
            inner: tweaked.inner,
        }
    }
}

impl From<PublicKey> for Recipient<PubkeyHash> {
    fn from(pubkey: PublicKey) -> Self {
        Recipient {
            inner: pubkey.into(),
        }
    }
}

impl From<Recipient<PublicKey>> for Recipient<PubkeyHash> {
    fn from(recipient: Recipient<PublicKey>) -> Self {
        Recipient {
            inner: Self::from(recipient.inner).inner,
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

        Recipient {
            inner: pk.inner.into(),
        }
    }
}

impl Recipient<TweakedPublicKey> {
    pub fn tweaked_pubkey(&self) -> TweakedPublicKey {
        self.inner
    }
}

impl Recipient<TaprootScript> {
    pub fn from_keypair(keypair: &KeyPair, merkle_root: TapNodeHash) -> Self {
        Recipient {
            inner: TaprootScript {
                pubkey: PublicKey::new(keypair.public_key()),
                merkle_root,
            },
        }
    }
    pub fn from_pubkey_recipient(
        recipient: Recipient<PublicKey>,
        merkle_root: TapNodeHash,
    ) -> Self {
        Recipient {
            inner: TaprootScript {
                pubkey: recipient.inner,
                merkle_root,
            },
        }
    }
    pub fn untweaked_pubkey(&self) -> UntweakedPublicKey {
        XOnlyPublicKey::from(self.inner.pubkey.inner)
    }
    pub fn merkle_root(&self) -> TapNodeHash {
        self.inner.merkle_root
    }
}
