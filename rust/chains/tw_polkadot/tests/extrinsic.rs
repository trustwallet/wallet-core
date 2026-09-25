use std::borrow::Cow;
use std::default::Default;

use tw_encoding::hex::ToHex;
use tw_number::U256;
use tw_proto::Polkadot::Proto;
use tw_proto::Polkadot::Proto::mod_Balance::{AssetTransfer, BatchAssetTransfer, Transfer};
use tw_proto::Polkadot::Proto::mod_Staking::{
    Bond, BondExtra, Chill, Nominate, Rebond, Unbond, WithdrawUnbonded,
};
use tw_substrate::EncodeResult;

use tw_polkadot::call_encoder::CallEncoder;

fn encode_input(input: &Proto::SigningInput<'_>) -> EncodeResult<Vec<u8>> {
    let encoded = CallEncoder::encode_input(input)?;
    Ok(encoded.0)
}

fn custom_call_indices(module: u8, method: u8) -> Option<Proto::CallIndices> {
    Some(Proto::CallIndices {
        variant: Proto::mod_CallIndices::OneOfvariant::custom(Proto::CustomCallIndices {
            module_index: module as i32,
            method_index: method as i32,
        }),
    })
}

fn balance_call(
    call: Proto::mod_Balance::OneOfmessage_oneof,
) -> Proto::mod_SigningInput::OneOfmessage_oneof {
    Proto::mod_SigningInput::OneOfmessage_oneof::balance_call(Proto::Balance {
        message_oneof: call,
    })
}

fn staking_call(
    call: Proto::mod_Staking::OneOfmessage_oneof,
) -> Proto::mod_SigningInput::OneOfmessage_oneof {
    Proto::mod_SigningInput::OneOfmessage_oneof::staking_call(Proto::Staking {
        message_oneof: call,
    })
}

#[test]
fn polkadot_encode_transfer() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: "14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD".into(),
            value: Cow::Owned(U256::from(1u64).to_big_endian().to_vec()),
            ..Default::default()
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "050000a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d04"
    );
}

#[test]
fn statemint_encode_asset_transfer() {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2619512-2

    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::asset_transfer(
            AssetTransfer {
                call_indices: custom_call_indices(0x32, 0x05),
                to_address: "14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD".into(),
                value: Cow::Owned(U256::from(999500000u64).to_big_endian().to_vec()),
                asset_id: 1984,
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "3205\
        011f\
        00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d\
        82a34cee"
    );
}

#[test]
fn statemint_encode_batch_asset_transfer() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: balance_call(
            Proto::mod_Balance::OneOfmessage_oneof::batch_asset_transfer(BatchAssetTransfer {
                call_indices: custom_call_indices(0x28, 0x00),
                fee_asset_id: 0x00,
                transfers: vec![AssetTransfer {
                    call_indices: custom_call_indices(0x32, 0x06),
                    to_address: "13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy".into(),
                    value: U256::from(808081u64).to_big_endian().to_vec().into(),
                    asset_id: 1984,
                    ..Default::default()
                }],
            }),
        ),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "2800\
        04\
        3206\
        011f\
        00\
        81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91\
        46523100"
    );
}

#[test]
fn kusama_encode_asset_transfer_without_call_indices() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: balance_call(
            Proto::mod_Balance::OneOfmessage_oneof::batch_asset_transfer(BatchAssetTransfer {
                fee_asset_id: 0x00,
                transfers: vec![AssetTransfer {
                    to_address: "13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy".into(),
                    value: U256::from(808081u64).to_big_endian().to_vec().into(),
                    asset_id: 1984,
                    ..Default::default()
                }],
                ..Default::default()
            }),
        ),
        ..Default::default()
    };

    encode_input(&input).expect_err("unexpected success");
}

#[test]
fn encode_staking_nominate() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::nominate(Nominate {
            nominators: vec![
                "13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy".into(),
                "14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD".into(),
            ],
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "0705080081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d",
    );
}

#[test]
fn encode_staking_chill() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::chill(Chill {
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "0706");
}

#[test]
fn encode_staking_bond_with_controller() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond(Bond {
            controller: "13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy".into(),
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            reward_destination: Proto::RewardDestination::CONTROLLER,
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(
        encoded.to_hex(),
        "07000081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c914652310002"
    );
}

#[test]
fn encode_staking_bond() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond(Bond {
            controller: Default::default(),
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            reward_destination: Proto::RewardDestination::STAKED,
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "07004652310000");
}

#[test]
fn encode_staking_bond_extra() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond_extra(
            BondExtra {
                value: U256::from(808081u64).to_big_endian().to_vec().into(),
                call_indices: None,
            },
        )),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "070146523100");
}

#[test]
fn encode_staking_rebond() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::rebond(Rebond {
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "071346523100");
}

#[test]
fn encode_staking_unbond() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::unbond(Unbond {
            value: U256::from(808081u64).to_big_endian().to_vec().into(),
            call_indices: None,
        })),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "070246523100");
}

#[test]
fn encode_staking_withdraw_unbonded() {
    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::withdraw_unbonded(
            WithdrawUnbonded {
                slashing_spans: 84,
                call_indices: None,
            },
        )),
        ..Default::default()
    };

    let encoded = encode_input(&input).expect("error encoding call");
    assert_eq!(encoded.to_hex(), "070354000000");
}
