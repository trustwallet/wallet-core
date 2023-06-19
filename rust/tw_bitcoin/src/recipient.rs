use std::str::FromStr;

use crate::output::TaprootScript;
use crate::{tweak_pubkey, Error, Result};
use bitcoin::key::{KeyPair, PublicKey, TweakedPublicKey, UntweakedPublicKey};
use bitcoin::taproot::TapNodeHash;
use bitcoin::{
    secp256k1::{self, XOnlyPublicKey},
    Address, Network, PubkeyHash, WPubkeyHash,
};

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct Recipient<T> {
    t: T,
}

impl Recipient<PublicKey> {
    pub fn from_keypair(keypair: &KeyPair) -> Self {
        Recipient {
            t: PublicKey::new(keypair.public_key()),
        }
    }
    pub fn public_key(&self) -> PublicKey {
        self.t
    }
    pub fn pubkey_hash(&self) -> PubkeyHash {
        PubkeyHash::from(self.t)
    }
    pub fn wpubkey_hash(&self) -> Result<WPubkeyHash> {
        self.t.wpubkey_hash().ok_or(Error::Todo)
    }
    pub fn tweaked_pubkey(&self) -> TweakedPublicKey {
        tweak_pubkey(self.t)
    }
    pub fn untweaked_pubkey(&self) -> UntweakedPublicKey {
        XOnlyPublicKey::from(self.t.inner)
    }
    pub fn legacy_address(&self, network: Network) -> Address {
        Address::p2pkh(&self.t, network)
    }
    pub fn segwit_address(&self, network: Network) -> Result<Address> {
        Address::p2wpkh(&self.t, network).map_err(|_| Error::Todo)
    }
    pub fn taproot_address(&self, network: Network) -> Address {
        let untweaked = UntweakedPublicKey::from(self.t.inner);
        Address::p2tr(&secp256k1::Secp256k1::new(), untweaked, None, network)
    }
    pub fn legacy_address_string(&self, network: Network) -> String {
        self.legacy_address(network).to_string()
    }
    pub fn segwit_address_string(&self, network: Network) -> Result<String> {
        self.segwit_address(network).map(|addr| addr.to_string())
    }
    pub fn taproot_address_string(&self, network: Network) -> String {
        self.taproot_address(network).to_string()
    }
}

impl Recipient<PubkeyHash> {
    pub fn pubkey_hash(&self) -> &PubkeyHash {
        &self.t
    }
}

impl Recipient<WPubkeyHash> {
    pub fn from_slice(slice: &[u8]) -> Result<Self> {
        Ok(Recipient {
            t: PublicKey::from_slice(slice)
                .map_err(|_| Error::Todo)?
                .wpubkey_hash()
                .ok_or(Error::Todo)?,
        })
    }
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

impl TryFrom<PublicKey> for Recipient<WPubkeyHash> {
    type Error = Error;

    fn try_from(pubkey: PublicKey) -> Result<Self> {
        Ok(Recipient {
            t: pubkey.wpubkey_hash().unwrap(),
        })
    }
}

impl TryFrom<Recipient<PublicKey>> for Recipient<WPubkeyHash> {
    type Error = Error;

    fn try_from(recipient: Recipient<PublicKey>) -> Result<Self> {
        Ok(Recipient {
            t: Self::try_from(recipient.t)?.t,
        })
    }
}

impl TryFrom<&KeyPair> for Recipient<WPubkeyHash> {
    type Error = Error;

    fn try_from(keypair: &KeyPair) -> Result<Self> {
        let pubkey = Recipient::<PublicKey>::from(keypair);
        Self::try_from(pubkey.t)
    }
}

impl TryFrom<KeyPair> for Recipient<WPubkeyHash> {
    type Error = Error;

    fn try_from(keypair: KeyPair) -> Result<Self> {
        Self::try_from(&keypair)
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

impl Recipient<TweakedPublicKey> {
    pub fn tweaked_pubkey(&self) -> TweakedPublicKey {
        self.t
    }
}

impl Recipient<TaprootScript> {
    pub fn from_keypair(keypair: &KeyPair, merkle_root: TapNodeHash) -> Self {
        Recipient {
            t: TaprootScript {
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
            t: TaprootScript {
                pubkey: recipient.t,
                merkle_root,
            },
        }
    }
    pub fn untweaked_pubkey(&self) -> UntweakedPublicKey {
        XOnlyPublicKey::from(self.t.pubkey.inner)
    }
    pub fn merkle_root(&self) -> TapNodeHash {
        self.t.merkle_root
    }
}
