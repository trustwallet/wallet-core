use crate::{keypair_from_wif, Recipient};
use bitcoin::{Network, PublicKey};

// This private key was used in a Bitcoin regtest environment.
pub const ALICE_WIF: &str = "cQUNzeMnF9xPPLqZhH7hMVYGwSuu3b78zznuc5UrxgXnYQBq6Bx1";

#[test]
fn addresses() {
    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(&alice);

    assert_eq!(
        recipient.legacy_address_string(Network::Bitcoin),
        "1MrZNGN7mfWZiZNQttrzHjfw72jnJC2JNx"
    );
    assert_eq!(
        recipient.segwit_address_string(Network::Bitcoin).unwrap(),
        "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff"
    );
    assert_eq!(
        recipient.taproot_address_string(Network::Bitcoin),
        "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5"
    );
}
