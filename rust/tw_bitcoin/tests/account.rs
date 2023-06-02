use bitcoin::{Network, PublicKey};
use tw_bitcoin::Recipient;

mod common;
use common::*;

#[test]
fn account_manager() {
    let recipient = Recipient::<PublicKey>::from_wif(ALICE_WIF, Network::Bitcoin).unwrap();

    assert_eq!(
        recipient.legacy_address_string(),
        "1MrZNGN7mfWZiZNQttrzHjfw72jnJC2JNx"
    );
    assert_eq!(
        recipient.segwit_address_string(),
        "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff"
    );
    assert_eq!(
        recipient.taproot_address_string(),
        "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5"
    );
}
