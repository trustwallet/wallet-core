// A throwaway coin for Sui devnet tests only. The treasury is a SHARED object so that anyone can mint:
// no key, no secret, nothing of value. Used to test SIP-58 token withdrawals from an address balance.
module test_coin::test_coin;

use sui::coin::{Self, Coin, TreasuryCap};

public struct TEST_COIN has drop {}

fun init(witness: TEST_COIN, ctx: &mut TxContext) {
    let (treasury, metadata) = coin::create_currency(
        witness,
        6,
        b"TST",
        b"SIP-58 test coin",
        b"Devnet only. Anyone can mint.",
        option::none(),
        ctx,
    );
    transfer::public_freeze_object(metadata);
    transfer::public_share_object(treasury);
}

/// Mints `amount` and returns the coin, so a PTB can send it wherever it wants (an address balance too).
public fun mint(treasury: &mut TreasuryCap<TEST_COIN>, amount: u64, ctx: &mut TxContext): Coin<TEST_COIN> {
    coin::mint(treasury, amount, ctx)
}
