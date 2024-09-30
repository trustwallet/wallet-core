use tw_hash::H264;

use crate::script::Witness;
use crate::signature::{BitcoinEcdsaSignature, BitcoinSchnorrSignature};

use super::Script;

/// Creates a script to claim a P2PK spending condition (_scriptSig_).
///
/// ```txt
/// <sig>
/// ```
pub fn new_p2pk(sig: &BitcoinEcdsaSignature) -> Script {
    let mut s = Script::with_capacity(35);
    s.push_slice(&sig.serialize());
    s
}

// TODO next iteration.
// /// Creates script items to claim a P2SH spending condition.
// /// (_witness_).
// ///
// /// ```txt
// /// <push><item_1><push><item_2><push><redeem_script>
// /// ```
// pub fn new_p2sh(items: Vec<Script>, redeem_script: Script) -> Script {
//     let mut s = Script::new();
//     for item in items {
//         s.push_slice(item.as_slice());
//     }
//     s.push_slice(redeem_script.as_slice());
//     s
// }

/// Creates a script to claim a P2PKH spending condition (_scriptSig_).
/// See [`tw_keypair::ecdsa::der::Signature::der_bytes`].
///
/// ```txt
/// <push><sig><push><pubkey>
/// ```
pub fn new_p2pkh(sig: &BitcoinEcdsaSignature, pubkey: &H264) -> Script {
    let mut s = Script::with_capacity(100);
    s.push_slice(&sig.serialize());
    s.push_slice(pubkey.as_slice());
    s
}

// TODO next iteration.
// /// Creates witness script items to claim a P2WSH spending condition
// /// (_witness_).
// ///
// /// ```txt
// /// <witness_item_1>
// /// <witness_item_2>
// /// ...
// /// <redeem_script>
// /// ```
// pub fn new_p2wsh(items: Vec<Script>, redeem_script: Script) -> Witness {
//     let mut w = Witness::new();
//     for item in items {
//         w.push_item(item.clone());
//     }
//     w.push_item(redeem_script);
//     w
// }

/// Creates witness script items to claim a P2WPKH spending condition
/// (_witness_).
///
/// ```txt
/// <sig>
/// <pubkey>
/// ```
pub fn new_p2wpkh(sig: &BitcoinEcdsaSignature, pubkey: H264) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig.serialize()));
    w.push_item(Script::from(pubkey.into_vec()));
    w
}

/// Creates witness script items to claim a P2TR spending condition
/// (_witness_).
///
/// ```txt
/// <sig>
/// ```
pub fn new_p2tr_key_path(sig: Vec<u8>) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig));
    w
}

/// Creates witness script items to claim a P2TR complex Taproot transaction
/// (_witness_).
///
/// ```txt
/// <sig>
/// <payload>
/// <control_block>
/// ```
pub fn new_p2tr_script_path(
    sig: &BitcoinSchnorrSignature,
    payload: Script,
    control_block: Vec<u8>,
) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig.serialize()));
    w.push_item(payload);
    w.push_item(Script::from(control_block));
    w
}
