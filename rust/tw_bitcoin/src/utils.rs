use bitcoin::key::{PublicKey, TapTweak, TweakedPublicKey};
use bitcoin::secp256k1::{self, XOnlyPublicKey};

pub(crate) fn tweak_pubkey(pubkey: PublicKey) -> TweakedPublicKey {
    let xonly = XOnlyPublicKey::from(pubkey.inner);
    let (tweaked, _) = xonly.tap_tweak(&secp256k1::Secp256k1::new(), None);
    tweaked
}
