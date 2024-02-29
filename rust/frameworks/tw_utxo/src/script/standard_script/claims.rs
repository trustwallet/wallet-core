use tw_hash::H264;

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
// TODO: Use BitcoinEcdsaSignature here?
pub fn new_p2pk(sig: &[u8]) -> Script {
    let mut s = Script::with_capacity(35);
    s.append(sig);
    s
}

/// Creates a script to claim a P2PKH spending condition (_scriptSig_).
///
/// ```txt
/// <push><sig><push><pubkey>
/// ```
// TODO: Use BitcoinEcdsaSignature here?
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
// TODO: Use BitcoinEcdsaSignature here?
pub fn new_p2wpkh(sig: Vec<u8>, pubkey: H264) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig));
    w.push_item(Script::from(pubkey.into_vec()));
    w
}

pub fn new_p2tr_key_path(sig: Vec<u8>) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig));
    w
}

// TODO: Consider using newtypes for payload and control block.
pub fn new_p2tr_script_path(sig: Vec<u8>, payload: Script, control_block: Vec<u8>) -> Witness {
    let mut w = Witness::new();
    w.push_item(Script::from(sig));
    w.push_item(payload);
    w.push_item(Script::from(control_block));
    w
}
