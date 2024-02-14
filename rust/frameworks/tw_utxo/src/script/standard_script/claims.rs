use tw_hash::{H264, H520};

use crate::script::Witness;

use super::Script;

/// Creates a script to claim a P2SH spending condition (_scriptSig_). All the data items and
/// the redeem script are pushed with a length indicator.
///
/// ```txt
/// <push><data_item_1> <push><data_item_2>... <push><redeem_script>
/// ```
pub fn new_p2sh(data_items: &[Script], redeem_script: &Script) -> Script {
    let mut s = Script::with_capacity(23);
    for item in data_items {
        s.push_slice(item.as_data());
    }
    s.push_slice(redeem_script.as_data());
    s
}

/// Creates a script to claim a P2PK spending condition (_scriptSig_).
///
/// ```txt
/// <sig>
/// ```
pub fn new_p2pk(sig: &H520) -> Script {
    let mut s = Script::with_capacity(35);
    s.append(sig.as_slice());
    s
}

/// Creates a script to claim a P2PKH spending condition (_scriptSig_).
///
/// ```txt
/// <push><sig><push><pubkey>
/// ```
pub fn new_p2pkh(sig: &[u8], pubkey: &H264) -> Script {
    let mut s = Script::with_capacity(100);
    s.push_slice(sig);
    s.push_slice(pubkey.as_slice());
    s
}

/// Creates witness script items to claim a P2WSH spending condition
/// (_witness_).
///
/// ```txt
/// <witness_item_1>
/// <witness_item_2>
/// ...
/// <redeem_script>
/// ```
pub fn new_p2wsh(items: Vec<Script>, redeem_script: Script) -> Witness {
    let mut w = Witness::new();
    for item in items {
        w.push_item(item.clone());
    }
    w.push_item(redeem_script);
    w
}

/// Creates witness script items to claim a P2WPKH spending condition
/// (_witness_).
///
/// ```txt
/// <sig>
/// <pubkey>
/// ```
pub fn new_p2wpkh(sig: H520, pubkey: H264) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig.into_vec()));
    w.push_item(Script::from(pubkey.into_vec()));
    w
}
