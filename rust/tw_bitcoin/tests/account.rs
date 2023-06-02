use bitcoin::Network;
use tw_bitcoin::Account;

mod common;
use common::*;

#[test]
fn account_manager() {
    let manager = Account::from_wif(ALICE_WIF, Network::Bitcoin).unwrap();

    assert_eq!(
        manager.get_legacy_address_string(),
        "1MrZNGN7mfWZiZNQttrzHjfw72jnJC2JNx"
    );
    assert_eq!(
        manager.get_segwit_address_string(),
        "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff"
    );
    assert_eq!(
        manager.get_taproot_address_string(),
        "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5"
    );
}
