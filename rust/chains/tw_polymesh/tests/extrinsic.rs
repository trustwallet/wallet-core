use std::borrow::Cow;
use std::default::Default;

use tw_encoding::hex::ToHex;
use tw_number::U256;
use tw_proto::Polkadot::Proto::RewardDestination;
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::{OneOfmessage_oneof as BalanceVariant, Transfer},
    mod_Identity::{
        mod_AddAuthorization::{mod_Authorization::OneOfauth_oneof as AuthVariant, Authorization},
        AddAuthorization, JoinIdentityAsKey, OneOfmessage_oneof as IdentityVariant,
    },
    mod_RuntimeCall::OneOfpallet_oneof as CallVariant,
    mod_SecondaryKeyPermissions::{
        AssetPermissions, ExtrinsicPermissions, PortfolioPermissions, RestrictionKind,
    },
    mod_Staking::{
        Bond, BondExtra, Chill, Nominate, OneOfmessage_oneof as StakingVariant, Rebond, Unbond,
        WithdrawUnbonded,
    },
    mod_Utility::{Batch, BatchKind, OneOfmessage_oneof as UtilityVariant},
    Balance, Identity, RuntimeCall, SecondaryKeyPermissions, Staking, Utility,
};

use tw_polymesh::call_encoder::CallEncoder;

fn expect_encoded(input: &Proto::SigningInput<'_>, expected_value: &str) {
    let encoded = CallEncoder::encode_input(input).expect("error encoding call");
    assert_eq!(encoded.0.to_hex(), expected_value);
}

fn polymesh_identity_call(call: IdentityVariant) -> RuntimeCall<'_> {
    RuntimeCall {
        pallet_oneof: CallVariant::identity_call(Identity {
            message_oneof: call,
        }),
    }
}

fn polymesh_add_auth_call(add_auth: AddAuthorization) -> RuntimeCall<'_> {
    polymesh_identity_call(IdentityVariant::add_authorization(add_auth))
}

fn polymesh_join_identity(auth_id: u64) -> RuntimeCall<'static> {
    polymesh_identity_call(IdentityVariant::join_identity_as_key(JoinIdentityAsKey {
        call_indices: None,
        auth_id,
    }))
}

fn balance_call(call: BalanceVariant) -> RuntimeCall<'_> {
    RuntimeCall {
        pallet_oneof: CallVariant::balance_call(Balance {
            message_oneof: call,
        }),
    }
}

fn staking_call(call: StakingVariant) -> RuntimeCall<'_> {
    RuntimeCall {
        pallet_oneof: CallVariant::staking_call(Staking {
            message_oneof: call,
        }),
    }
}

fn batch_calls(kind: BatchKind, calls: Vec<RuntimeCall<'static>>) -> RuntimeCall<'static> {
    RuntimeCall {
        pallet_oneof: CallVariant::utility_call(Utility {
            message_oneof: UtilityVariant::batch(Batch {
                kind,
                calls,
                ..Default::default()
            }),
        }),
    }
}

fn build_input(runtime_call: RuntimeCall<'_>) -> Proto::SigningInput<'_> {
    Proto::SigningInput {
        network: 12,
        transaction_version: 7,
        runtime_call: Some(runtime_call),
        ..Default::default()
    }
}

/// Test POLYX transfer with memo.
#[test]
fn polymesh_encode_transfer_with_memo() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x0501004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e0204014d454d4f20504144444544205749544820535041434553000000000000000000

    let input = build_input(balance_call(
        Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: "2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF".into(),
            value: Cow::Owned(U256::from(1u64).to_big_endian().to_vec()),
            // The memo field is padded with nulls.
            memo: "MEMO PADDED WITH SPACES".into(),
            ..Default::default()
        }),
    ));

    expect_encoded(&input, "0501004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e0204014d454d4f20504144444544205749544820535041434553000000000000000000");
}

/// Test add authorization to join identity with default permissions (`Whole` meaning all permissions).
#[test]
fn polymesh_encode_authorization_join_identity() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000

    let input = build_input(polymesh_add_auth_call(AddAuthorization {
        target: "2FM6FpjQ6r5HTt7FGYSzskDNkwUyFsonMtwBpsnr9vwmCjhc".into(),
        authorization: Some(Authorization {
            auth_oneof: AuthVariant::join_identity(SecondaryKeyPermissions::default()),
        }),
        ..Default::default()
    }));

    expect_encoded(
        &input,
        "070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000",
    );
}

/// Test add authorization to join identity with no permissions.
#[test]
fn polymesh_encode_authorization_join_identity_with_zero_data() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320501000100010000

    let input = build_input(polymesh_add_auth_call(AddAuthorization {
        target: "2FM6FpjQ6r5HTt7FGYSzskDNkwUyFsonMtwBpsnr9vwmCjhc".into(),
        authorization: Some(Authorization {
            auth_oneof: AuthVariant::join_identity(SecondaryKeyPermissions {
                // No asset permissions.
                asset: Some(AssetPermissions {
                    kind: RestrictionKind::These,
                    assets: vec![],
                }),
                // No extrinsic permissions.
                extrinsic: Some(ExtrinsicPermissions {
                    kind: RestrictionKind::These,
                    pallets: vec![],
                }),
                // No portfolio permissions.
                portfolio: Some(PortfolioPermissions {
                    kind: RestrictionKind::These,
                    portfolios: vec![],
                }),
            }),
        }),
        ..Default::default()
    }));

    expect_encoded(
        &input,
        "070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320501000100010000",
    );
}

/// Test add authorization to join identity with all permissions.  Each permission is set to `None`, which defaults to `Whole`.
#[test]
fn polymesh_encode_authorization_join_identity_allowing_everything() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000

    let input = build_input(polymesh_add_auth_call(AddAuthorization {
        target: "2FM6FpjQ6r5HTt7FGYSzskDNkwUyFsonMtwBpsnr9vwmCjhc".into(),
        authorization: Some(Authorization {
            auth_oneof: AuthVariant::join_identity(SecondaryKeyPermissions {
                // All asset permissions.
                asset: None,
                // All extrinsic permissions.
                extrinsic: None,
                // All portfolio permissions.
                portfolio: None,
            }),
        }),
        ..Default::default()
    }));

    expect_encoded(
        &input,
        "070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000",
    );
}

/// Test accepting a join identity authorization.
#[test]
fn polymesh_encode_identity_join_identity() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x07040b13000000000000

    let input = build_input(polymesh_join_identity(4875));

    expect_encoded(&input, "07040b13000000000000");
}

/// Test staking nominate.
#[test]
fn encode_staking_nominate() {
    let input = build_input(staking_call(StakingVariant::nominate(Nominate {
        nominators: vec![
            "2DxgKKS53wsAeETAZXhmT5A1bTt7h1aV4bKdtkMDwwSzSMXm".into(),
            "2HqjMm2goapWvXQBqjjEdVaTZsUmunWwEq1TSToDR1pDzQ1F".into(),
        ],
        call_indices: None,
    })));

    expect_encoded(&input, "1105080042ef301451c7f596f974daec8ca1234f66809905d13e16c18e23896b0c57e53e00ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148");
}

/// Test staking chill.
#[test]
fn encode_staking_chill() {
    let input = build_input(staking_call(StakingVariant::chill(Chill {
        call_indices: None,
    })));

    expect_encoded(&input, "1106");
}

/// Test staking bond.
#[test]
fn encode_staking_bond() {
    let input = build_input(staking_call(StakingVariant::bond(Bond {
        controller: "2HqjMm2goapWvXQBqjjEdVaTZsUmunWwEq1TSToDR1pDzQ1F".into(),
        value: U256::from(808081u64).to_big_endian().to_vec().into(),
        reward_destination: RewardDestination::STAKED,
        call_indices: None,
    })));

    expect_encoded(
        &input,
        "110000ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e671484652310000",
    );
}

/// Test staking bond extra.
#[test]
fn encode_staking_bond_extra() {
    let input = build_input(staking_call(StakingVariant::bond_extra(BondExtra {
        value: U256::from(808081u64).to_big_endian().to_vec().into(),
        call_indices: None,
    })));

    expect_encoded(&input, "110146523100");
}

/// Test staking rebond.
#[test]
fn encode_staking_rebond() {
    let input = build_input(staking_call(StakingVariant::rebond(Rebond {
        value: U256::from(808081u64).to_big_endian().to_vec().into(),
        call_indices: None,
    })));

    expect_encoded(&input, "111346523100");
}

/// Test staking unbond.
#[test]
fn encode_staking_unbond() {
    let input = build_input(staking_call(StakingVariant::unbond(Unbond {
        value: U256::from(808081u64).to_big_endian().to_vec().into(),
        call_indices: None,
    })));

    expect_encoded(&input, "110246523100");
}

/// Test staking withdraw unbonded.
#[test]
fn encode_staking_withdraw_unbonded() {
    let input = build_input(staking_call(StakingVariant::withdraw_unbonded(
        WithdrawUnbonded {
            slashing_spans: 84,
            call_indices: None,
        },
    )));

    expect_encoded(&input, "110354000000");
}

/// Test atomic batching staking calls bond and nominate.
#[test]
fn encode_staking_batch_bond_and_nominate() {
    let input = build_input(batch_calls(
        BatchKind::Atomic,
        vec![
            staking_call(StakingVariant::bond(Bond {
                controller: "2HqjMm2goapWvXQBqjjEdVaTZsUmunWwEq1TSToDR1pDzQ1F".into(),
                value: U256::from(808081u64).to_big_endian().to_vec().into(),
                reward_destination: RewardDestination::STAKED,
                call_indices: None,
            })),
            staking_call(StakingVariant::nominate(Nominate {
                nominators: vec![
                    "2DxgKKS53wsAeETAZXhmT5A1bTt7h1aV4bKdtkMDwwSzSMXm".into(),
                    "2HqjMm2goapWvXQBqjjEdVaTZsUmunWwEq1TSToDR1pDzQ1F".into(),
                ],
                call_indices: None,
            })),
        ],
    ));

    expect_encoded(
        &input,
        "290208110000ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e6714846523100001105080042ef301451c7f596f974daec8ca1234f66809905d13e16c18e23896b0c57e53e00ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148"
    );
}

/// Test atomic batching of staking calls chill and unbond.
#[test]
fn encode_staking_batch_chill_and_unbond() {
    let input = build_input(batch_calls(
        BatchKind::Atomic,
        vec![
            staking_call(StakingVariant::chill(Chill { call_indices: None })),
            staking_call(StakingVariant::unbond(Unbond {
                value: U256::from(808081u64).to_big_endian().to_vec().into(),
                call_indices: None,
            })),
        ],
    ));

    expect_encoded(&input, "2902081106110246523100");
}

/// Test optimistic batch of POLYX transfers.
#[test]
fn encode_batch_transfers() {
    let input = build_input(batch_calls(
        BatchKind::Optimistic,
        vec![
            balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                to_address: "2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF".into(),
                value: Cow::Owned(U256::from(1u64).to_big_endian().to_vec()),
                ..Default::default()
            })),
            balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                to_address: "2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys".into(),
                value: Cow::Owned(U256::from(2u64).to_big_endian().to_vec()),
                // The memo field is padded with nulls.
                memo: "MEMO PADDED WITH SPACES".into(),
                ..Default::default()
            })),
        ],
    ));

    expect_encoded(
        &input,
        "2904080500004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e02040501004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c08014d454d4f20504144444544205749544820535041434553000000000000000000",
    );
}

/// Test stop on first error batch of POLYX transfers.
#[test]
fn encode_batch_transfers_stop_on_first_error() {
    let input = build_input(batch_calls(
        BatchKind::StopOnError,
        vec![
            balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                to_address: "2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF".into(),
                value: Cow::Owned(U256::from(1u64).to_big_endian().to_vec()),
                ..Default::default()
            })),
            balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                to_address: "2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys".into(),
                value: Cow::Owned(U256::from(2u64).to_big_endian().to_vec()),
                // The memo field is padded with nulls.
                memo: "MEMO PADDED WITH SPACES".into(),
                ..Default::default()
            })),
        ],
    ));

    expect_encoded(
        &input,
        "2900080500004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e02040501004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c08014d454d4f20504144444544205749544820535041434553000000000000000000",
    );
}

/// Test that nesting of batch calls is not allowed.
#[test]
fn encode_nested_batch_calls() {
    let input = build_input(batch_calls(
        BatchKind::Atomic,
        vec![
            balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                to_address: "2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF".into(),
                value: Cow::Owned(U256::from(1u64).to_big_endian().to_vec()),
                ..Default::default()
            })),
            batch_calls(
                BatchKind::Atomic,
                vec![balance_call(
                    Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                        to_address: "2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys".into(),
                        value: Cow::Owned(U256::from(2u64).to_big_endian().to_vec()),
                        // The memo field is padded with nulls.
                        memo: "MEMO PADDED WITH SPACES".into(),
                        ..Default::default()
                    }),
                )],
            ),
        ],
    ));

    let tw_err =
        CallEncoder::encode_input(&input).expect_err("nested batch calls should not be allowed");
    assert_eq!(
        tw_err.error_type(),
        &tw_substrate::EncodeError::NotSupported
    );
    // Ensure the error message contains the expected context.
    let context = format!("{:?}", tw_err);
    assert!(context.contains("Nested batch calls not allowed"));
}
