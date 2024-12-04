use std::borrow::Cow;
use std::default::Default;

use tw_encoding::hex::ToHex;
use tw_number::U256;
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::Transfer,
    mod_Identity::mod_AddAuthorization::{
        mod_Authorization::OneOfauth_oneof as AuthVariant, Authorization,
    },
    mod_SecondaryKeyPermissions::{
        AssetPermissions, ExtrinsicPermissions, PortfolioPermissions, RestrictionKind,
    },
    mod_Staking::{Bond, BondExtra, Chill, Nominate, Rebond, Unbond, WithdrawUnbonded},
    SecondaryKeyPermissions,
};
use tw_substrate::EncodeResult;

use tw_polymesh::call_encoder::CallEncoder;

fn encode_input(input: &Proto::SigningInput<'_>) -> EncodeResult<Vec<u8>> {
    let encoded = CallEncoder::encode_input(input)?;
    Ok(encoded.0)
}

fn polymesh_identity_call(
    call: Proto::mod_Identity::OneOfmessage_oneof,
) -> Option<Proto::RuntimeCall<'_>> {
    Some(Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::identity_call(Proto::Identity {
            message_oneof: call,
        }),
    })
}

fn polymesh_add_auth_call(
    add_auth: Proto::mod_Identity::AddAuthorization,
) -> Option<Proto::RuntimeCall<'_>> {
    polymesh_identity_call(Proto::mod_Identity::OneOfmessage_oneof::add_authorization(
        add_auth,
    ))
}

fn polymesh_join_identity(auth_id: u64) -> Option<Proto::RuntimeCall<'static>> {
    polymesh_identity_call(
        Proto::mod_Identity::OneOfmessage_oneof::join_identity_as_key(
            Proto::mod_Identity::JoinIdentityAsKey {
                call_indices: None,
                auth_id,
            },
        ),
    )
}

fn balance_call(call: Proto::mod_Balance::OneOfmessage_oneof) -> Option<Proto::RuntimeCall<'_>> {
    Some(Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::balance_call(Proto::Balance {
            message_oneof: call,
        }),
    })
}

fn staking_call(call: Proto::mod_Staking::OneOfmessage_oneof) -> Option<Proto::RuntimeCall<'_>> {
    Some(Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::staking_call(Proto::Staking {
            message_oneof: call,
        }),
    })
}

#[test]
fn polymesh_encode_transfer_with_memo() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x0501004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e0204014d454d4f20504144444544205749544820535041434553000000000000000000

    let input = Proto::SigningInput {
        network: 12,
        runtime_call: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: "2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF".into(),
            value: Cow::Owned(U256::from(1u64).to_big_endian().to_vec()),
            memo: "MEMO PADDED WITH SPACES".into(),
            ..Default::default()
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "0501004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e0204014d454d4f20504144444544205749544820535041434553000000000000000000"
    );
}

#[test]
fn polymesh_encode_authorization_join_identity() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000

    let input = Proto::SigningInput {
        network: 12,
        runtime_call: polymesh_add_auth_call(Proto::mod_Identity::AddAuthorization {
            target: "2FM6FpjQ6r5HTt7FGYSzskDNkwUyFsonMtwBpsnr9vwmCjhc".into(),
            authorization: Some(Authorization {
                auth_oneof: AuthVariant::join_identity(SecondaryKeyPermissions::default()),
            }),
            ..Default::default()
        }),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000"
    );
}

#[test]
fn polymesh_encode_authorization_join_identity_with_zero_data() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320501000100010000

    let input = Proto::SigningInput {
        network: 12,
        runtime_call: polymesh_add_auth_call(Proto::mod_Identity::AddAuthorization {
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
        }),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320501000100010000"
    );
}

#[test]
fn polymesh_encode_authorization_join_identity_allowing_everything() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000

    let input = Proto::SigningInput {
        network: 12,
        runtime_call: polymesh_add_auth_call(Proto::mod_Identity::AddAuthorization {
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
        }),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "070a0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000"
    );
}

#[test]
fn polymesh_encode_identity() {
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x07040b13000000000000

    let input = Proto::SigningInput {
        network: 12,
        runtime_call: polymesh_join_identity(4875),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "07040b13000000000000");
}

#[test]
fn encode_staking_nominate() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::nominate(Nominate {
            nominators: vec![
                "2DxgKKS53wsAeETAZXhmT5A1bTt7h1aV4bKdtkMDwwSzSMXm".into(),
                "2HqjMm2goapWvXQBqjjEdVaTZsUmunWwEq1TSToDR1pDzQ1F".into(),
            ],
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "1105080042ef301451c7f596f974daec8ca1234f66809905d13e16c18e23896b0c57e53e00ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148"
    );
}

#[test]
fn encode_staking_chill() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::chill(Chill {
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "1106");
}

#[test]
fn encode_staking_bond() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond(Bond {
            controller: "2HqjMm2goapWvXQBqjjEdVaTZsUmunWwEq1TSToDR1pDzQ1F".into(),
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            reward_destination: Proto::RewardDestination::STAKED,
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "110000ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e671484652310000"
    );
}

#[test]
fn encode_staking_bond_extra() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond_extra(
            BondExtra {
                value: U256::from(808081u64).to_big_endian().to_vec().into(),
                call_indices: None,
            },
        )),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "110146523100");
}

#[test]
fn encode_staking_rebond() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::rebond(Rebond {
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "111346523100");
}

#[test]
fn encode_staking_unbond() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::unbond(Unbond {
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "110246523100");
}

#[test]
fn encode_staking_withdraw_unbonded() {
    let input = Proto::SigningInput {
        network: 12,
        runtime_call: staking_call(Proto::mod_Staking::OneOfmessage_oneof::withdraw_unbonded(
            WithdrawUnbonded {
                slashing_spans: 84,
                call_indices: None,
            },
        )),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "110354000000");
}
