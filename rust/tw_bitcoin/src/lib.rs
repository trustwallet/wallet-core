extern crate serde;

use bitcoin::key::{KeyPair, TweakedPublicKey, UntweakedPublicKey};
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::{TapNodeHash, TaprootSpendInfo};
use bitcoin::{
    secp256k1::{self, XOnlyPublicKey},
    Network, PublicKey,
};
use bitcoin::{Address, PubkeyHash, WPubkeyHash};

pub mod brc20;
pub mod claim;
pub mod ffi;
pub mod input;
pub mod ordinals;
pub mod output;
mod recipient_impl;
#[cfg(test)]
mod tests;
pub mod utils;
pub mod transaction;

// Reexports
pub use input::*;
pub use output::*;
pub use utils::*;
pub use transaction::*;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone)]
pub struct TaprootProgram {
    script: ScriptBuf,
    spend_info: TaprootSpendInfo,
}

#[derive(Debug, Clone)]
pub enum Error {
    Todo,
}

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

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct TaprootScript {
    pubkey: PublicKey,
    merkle_root: TapNodeHash,
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
