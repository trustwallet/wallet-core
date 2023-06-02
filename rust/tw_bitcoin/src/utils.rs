use crate::{Error, Result};
use bitcoin::address::Payload;
use bitcoin::key::{KeyPair, PrivateKey, PublicKey, TapTweak, TweakedKeyPair, TweakedPublicKey};
use bitcoin::script::ScriptBuf;
use bitcoin::secp256k1::{self, XOnlyPublicKey};
use bitcoin::{Address, PubkeyHash};
use std::str::FromStr;

pub fn pubkey_hash_from_address(string: &str) -> Result<PubkeyHash> {
    let addr = Address::from_str(string).map_err(|_| Error::Todo)?;
    match addr.payload {
        Payload::PubkeyHash(hash) => Ok(hash),
        _ => Err(Error::Todo),
    }
}

pub fn keypair_from_wif(string: &str) -> Result<KeyPair> {
    let pk = PrivateKey::from_wif(string).map_err(|_| Error::Todo)?;
    let keypair = KeyPair::from_secret_key(&secp256k1::Secp256k1::new(), &pk.inner);
    Ok(keypair)
}

pub(crate) fn tweak_pubkey(pubkey: PublicKey) -> TweakedPublicKey {
    let xonly = XOnlyPublicKey::from(pubkey.inner);
    let (tweaked, _) = xonly.tap_tweak(&secp256k1::Secp256k1::new(), None);
    tweaked
}

pub(crate) fn tweak_keypair(keypair: &KeyPair) -> TweakedKeyPair {
    keypair.tap_tweak(&secp256k1::Secp256k1::new(), None)
}

pub(crate) fn pubkey_hash_from_script(script: &ScriptBuf) -> Result<PubkeyHash> {
    match Payload::from_script(script).map_err(|_| Error::Todo)? {
        Payload::PubkeyHash(hash) => Ok(hash),
        _ => Err(Error::Todo),
    }
}
