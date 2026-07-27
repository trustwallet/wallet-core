// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::json;
use tw_coin_entry::error::prelude::SigningErrorType;
use tw_ripple::encode::st_object::STObject;

fn base_payment() -> serde_json::Value {
    json!({
        "TransactionType": "Payment",
        "Account": "r9TeThyi5xiuUUrFjtPKZiHcDxs7K9H6Rb",
        "Amount": "1000000",
        "Fee": "12",
        "Sequence": 1,
        "Flags": 0
    })
}

fn encode(tx: serde_json::Value) -> Vec<u8> {
    STObject::try_from_value(tx, false).unwrap().0
}

/// A tagged X-address as Destination must expand to the classic address + DestinationTag,
/// producing identical bytes to the explicit classic-address form.
#[test]
fn test_x_addr_tagged_equals_classic_plus_tag() {
    let mut with_x_addr = base_payment();
    with_x_addr["Destination"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF");

    let mut with_classic = base_payment();
    with_classic["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    with_classic["DestinationTag"] = json!(12345u32);

    assert_eq!(encode(with_x_addr), encode(with_classic));
}

/// If the JSON already carries a DestinationTag that matches the X-address tag, it must succeed.
#[test]
fn test_x_addr_tagged_matches_explicit_tag() {
    let mut tx = base_payment();
    tx["Destination"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF");
    tx["DestinationTag"] = json!(12345u32);

    assert!(STObject::try_from_value(tx, false).is_ok());
}

/// If the JSON carries a DestinationTag that conflicts with the X-address tag, it must fail.
#[test]
fn test_x_addr_tagged_conflicts_explicit_tag() {
    let mut tx = base_payment();
    tx["Destination"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF");
    tx["DestinationTag"] = json!(99999u32);

    assert!(STObject::try_from_value(tx, false).is_err());
}

/// An X-address with tag=0 and tag_flag=Classic must inject DestinationTag=0,
/// producing the same bytes as classic address + explicit DestinationTag=0.
#[test]
fn test_x_addr_tag_zero_classic_injects_zero() {
    let mut with_x_addr = base_payment();
    with_x_addr["Destination"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgTsM93nriVZAPufrpE3");

    let mut with_classic = base_payment();
    with_classic["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    with_classic["DestinationTag"] = json!(0u32);

    assert_eq!(encode(with_x_addr), encode(with_classic));
}

// ── TagFlag::None: DestinationTag must not be serialized ────────────────────

/// A no-tag Destination X-address must not inject DestinationTag into the preimage.
/// Proven in two complementary steps:
///   (a) the bytes equal the classic-no-tag reference (tag absent in both), and
///   (b) those bytes differ from the classic-with-tag reference (tag present changes output),
///       which validates that step (a) is a meaningful equality, not a vacuous one.
#[test]
fn test_x_addr_no_tag_destination_no_tag_field_absent() {
    let mut with_x_addr = base_payment();
    with_x_addr["Destination"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgSxWAqcQwu9z2r5d7Tm");

    let mut classic_no_tag = base_payment();
    classic_no_tag["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");

    let mut classic_with_tag = base_payment();
    classic_with_tag["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    classic_with_tag["DestinationTag"] = json!(1u32);

    let no_tag_bytes = encode(with_x_addr);
    assert_eq!(
        no_tag_bytes,
        encode(classic_no_tag),
        "TagFlag::None must not inject DestinationTag"
    );
    assert_ne!(
        no_tag_bytes,
        encode(classic_with_tag),
        "sanity: adding DestinationTag changes the encoding"
    );
}

/// A no-tag Destination X-address alongside an explicit DestinationTag must return
/// Error_invalid_params (not a generic error).
#[test]
fn test_x_addr_no_tag_destination_explicit_tag_is_invalid_params() {
    let mut tx = base_payment();
    tx["Destination"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgSxWAqcQwu9z2r5d7Tm");
    tx["DestinationTag"] = json!(42u32);

    let err = STObject::try_from_value(tx, false).unwrap_err();
    assert_eq!(
        *err.error_type(),
        SigningErrorType::Error_invalid_params,
        "tagless Destination X-address with explicit DestinationTag must yield Error_invalid_params"
    );
}

// ── TagFlag::None: SourceTag must not be serialized ─────────────────────────

/// A no-tag Account X-address must not inject SourceTag into the preimage.
/// Same two-step proof as the Destination variant above.
#[test]
fn test_x_addr_no_tag_account_no_source_tag_field_absent() {
    let mut with_x_addr = base_payment();
    with_x_addr["Account"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgSxWAqcQwu9z2r5d7Tm");
    with_x_addr["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");

    let mut classic_no_tag = base_payment();
    classic_no_tag["Account"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    classic_no_tag["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");

    let mut classic_with_tag = base_payment();
    classic_with_tag["Account"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    classic_with_tag["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    classic_with_tag["SourceTag"] = json!(1u32);

    let no_tag_bytes = encode(with_x_addr);
    assert_eq!(
        no_tag_bytes,
        encode(classic_no_tag),
        "TagFlag::None must not inject SourceTag"
    );
    assert_ne!(
        no_tag_bytes,
        encode(classic_with_tag),
        "sanity: adding SourceTag changes the encoding"
    );
}

/// A no-tag Account X-address alongside an explicit SourceTag must return
/// Error_invalid_params (not a generic error).
#[test]
fn test_x_addr_no_tag_account_explicit_source_tag_is_invalid_params() {
    let mut tx = base_payment();
    tx["Account"] = json!("X76UnYEMbQfEs3mUqgtjp4zFy9exgSxWAqcQwu9z2r5d7Tm");
    tx["Destination"] = json!("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    tx["SourceTag"] = json!(42u32);

    let err = STObject::try_from_value(tx, false).unwrap_err();
    assert_eq!(
        *err.error_type(),
        SigningErrorType::Error_invalid_params,
        "tagless Account X-address with explicit SourceTag must yield Error_invalid_params"
    );
}
