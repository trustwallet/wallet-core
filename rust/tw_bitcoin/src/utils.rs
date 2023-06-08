use crate::{Error, Result};
use bitcoin::key::{KeyPair, PrivateKey, PublicKey, TapTweak, TweakedPublicKey};
use bitcoin::secp256k1::{self, XOnlyPublicKey};

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
