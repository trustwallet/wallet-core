// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! SIP-58 (address balances): the transactions built from `Sui.proto` are compared, byte for byte,
//! with transactions written out literally here (inputs, commands, gas, expiration). The BCS layout of
//! the types itself is pinned separately against transactions the Sui node itself serialised (`devnet_sip58_roundtrip.rs`).

use move_core_types::identifier::Identifier;
use move_core_types::language_storage::{StructTag, TypeTag};
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;
use tw_encoding::hex::DecodeHex;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as Payload};
use tw_sui::address::SuiAddress;
use tw_sui::modules::tx_builder::{TWTransaction, TWTransactionBuilder};
use tw_sui::transaction::command::{Argument, Command};
use tw_sui::transaction::programmable_transaction::ProgrammableTransaction;
use tw_sui::transaction::sui_types::{
    sui_coin_type_tag, CallArg, ChainIdentifier, FundsWithdrawalArg, GasData, ObjectArg,
    ObjectDigest, ObjectID, ObjectRef, SequenceNumber, TransactionExpiration,
};
use tw_sui::transaction::transaction_data::{TransactionData, TransactionDataV1, TransactionKind};

const SENDER: &str = "0x3b298fff451dd0c45fd5625a899b6bdd0816f52bf76cacc6f2934ecf624fde7b";
const ALICE: &str = "0x3d498ee6ab43423781b5509957d6e75ffb30c4b98933ee0dddaafa52a7a7a634";
const BOB: &str = "0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a";
const VALIDATOR: &str = "0x61953ea72709eed72f4441dd944eec49a11b4acabfc8e04015e89c63be81b6ab";
const CHAIN: &str = "4btiuiMPvEENsttpZC7CZ53DruC3MAgfznDbASZ7DR6S";
const COIN_1: &str = "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005";
const GAS_COIN: &str = "0x0000000000000000000000000000000000000000000000000000000000000077";
const COIN_2: &str = "0x5ef77d20c7d6745d3d9b5f69e7825aae733fa5c8a3f82f7192749e3169791c8c";
const DIGEST: &str = "2eKuWbZSVfpFVfg8FXY9wP6W5AFXnTchSoUdp7obyYZ5";
const BUDGET: u64 = 5_000_000;
const PRICE: u64 = 100;
const SUI: u64 = 1_000_000_000;

// ------------------------------- proto side --------------------------------

fn proto_ref(id: &'static str, version: u64) -> Proto::ObjectRef<'static> {
    Proto::ObjectRef {
        object_id: id.into(),
        version,
        object_digest: DIGEST.into(),
    }
}

fn valid_during() -> Proto::ValidDuring<'static> {
    Proto::ValidDuring {
        min_epoch: 1288,
        max_epoch: 1289,
        chain: CHAIN.into(),
        nonce: 12345,
    }
}

/// Gas from the address balance when `ab_gas`, from a gas coin otherwise.
fn input(payload: Payload<'static>, ab_gas: bool) -> Proto::SigningInput<'static> {
    Proto::SigningInput {
        transaction_payload: payload,
        signer: SENDER.into(),
        gas_budget: BUDGET,
        reference_gas_price: PRICE,
        valid_during: ab_gas.then(valid_during),
        ..Proto::SigningInput::default()
    }
}

fn build(input: Proto::SigningInput<'static>) -> SigningResult<Vec<u8>> {
    match TWTransactionBuilder::new(input).build()? {
        TWTransaction::Transaction(tx) => Ok(bcs::encode(&tx).unwrap()),
        TWTransaction::SignDirect(_) => panic!("not a built transaction"),
    }
}

fn built(input: Proto::SigningInput<'static>) -> Vec<u8> {
    build(input).expect("transaction must build")
}

fn error_of(input: Proto::SigningInput<'static>) -> SigningErrorType {
    *build(input).expect_err("must be rejected").error_type()
}

// ------------------------------ expected side ------------------------------

/// A 32-byte address from a possibly short hex like `0x2`.
fn addr(hex: &str) -> SuiAddress {
    let digits = hex.trim_start_matches("0x");
    SuiAddress::from_str(&format!("0x{digits:0>64}")).unwrap()
}

fn object_ref(id: &str, version: u64) -> ObjectRef {
    (
        ObjectID(addr(id).into_inner()),
        SequenceNumber(version),
        ObjectDigest::from_str(DIGEST).unwrap(),
    )
}

fn pure_address(hex: &str) -> CallArg {
    CallArg::Pure(hex.trim_start_matches("0x").decode_hex().unwrap())
}

fn pure_u64(v: u64) -> CallArg {
    CallArg::Pure(v.to_le_bytes().to_vec())
}

fn owned(id: &str, version: u64) -> CallArg {
    CallArg::Object(ObjectArg::ImmOrOwnedObject(object_ref(id, version)))
}

fn withdrawal(amount: u64) -> CallArg {
    CallArg::FundsWithdrawal(FundsWithdrawalArg::balance_from_sender(
        amount,
        sui_coin_type_tag(),
    ))
}

/// `0x2::coin::redeem_funds<0x2::sui::SUI>(Input(withdrawal))`
fn redeem(withdrawal_input: u16) -> Command {
    Command::move_call(
        ObjectID(addr("0x2").into_inner()),
        Identifier::from_str("coin").unwrap(),
        Identifier::from_str("redeem_funds").unwrap(),
        vec![sui_coin_type_tag()],
        vec![Argument::Input(withdrawal_input)],
    )
}

fn window() -> TransactionExpiration {
    TransactionExpiration::ValidDuring {
        min_epoch: Some(1288),
        max_epoch: Some(1289),
        min_timestamp: None,
        max_timestamp: None,
        chain: ChainIdentifier::from_str(CHAIN).unwrap(),
        nonce: 12345,
    }
}

fn expected(
    inputs: Vec<CallArg>,
    commands: Vec<Command>,
    payment: Vec<ObjectRef>,
    expiration: TransactionExpiration,
) -> Vec<u8> {
    bcs::encode(&TransactionData::V1(TransactionDataV1 {
        kind: TransactionKind::ProgrammableTransaction(ProgrammableTransaction {
            inputs,
            commands,
        }),
        sender: addr(SENDER),
        gas_data: GasData {
            payment,
            owner: addr(SENDER),
            price: PRICE,
            budget: BUDGET,
        },
        expiration,
    }))
    .unwrap()
}

/// Gas from the address balance: no gas object, `ValidDuring`.
fn expected_ab(inputs: Vec<CallArg>, commands: Vec<Command>) -> Vec<u8> {
    expected(inputs, commands, vec![], window())
}

fn kind_of(bytes: &[u8]) -> Vec<u8> {
    let TransactionData::V1(v1) = bcs::decode(bytes).unwrap();
    bcs::encode(&v1.kind).unwrap()
}

fn gas_of(bytes: &[u8]) -> (Vec<ObjectRef>, Vec<u8>) {
    let TransactionData::V1(v1) = bcs::decode(bytes).unwrap();
    (v1.gas_data.payment, bcs::encode(&v1.expiration).unwrap())
}

fn result(i: u16) -> Argument {
    Argument::Result(i)
}

fn pay_sui(
    coins: Vec<Proto::ObjectRef<'static>>,
    withdraw: u64,
    recipients: &[&'static str],
    amounts: &[u64],
) -> Payload<'static> {
    Payload::pay_sui(Proto::PaySui {
        input_coins: coins,
        recipients: recipients.iter().map(|r| Cow::Borrowed(*r)).collect(),
        amounts: amounts.to_vec(),
        address_balance_amount: withdraw,
    })
}

// ---------------------------------- PaySui ----------------------------------

#[test]
fn test_pay_sui_only_from_the_address_balance_redeems_exactly_the_amount() {
    let actual = built(input(pay_sui(vec![], SUI, &[ALICE], &[SUI]), true));
    let expected = expected_ab(
        vec![withdrawal(SUI), pure_address(ALICE)],
        vec![
            redeem(0),
            Command::TransferObjects(vec![result(0)], Argument::Input(1)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_sui_only_from_the_address_balance_one_coin_per_distinct_recipient() {
    // ALICE appears twice: one transfer for her, 1 + 3 SUI redeemed at once; BOB gets 2 SUI.
    let actual = built(input(
        pay_sui(
            vec![],
            6 * SUI,
            &[ALICE, BOB, ALICE],
            &[SUI, 2 * SUI, 3 * SUI],
        ),
        true,
    ));
    let expected = expected_ab(
        vec![
            withdrawal(4 * SUI),
            pure_address(ALICE),
            withdrawal(2 * SUI),
            pure_address(BOB),
        ],
        vec![
            redeem(0),
            Command::TransferObjects(vec![result(0)], Argument::Input(1)),
            redeem(2),
            Command::TransferObjects(vec![result(2)], Argument::Input(3)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_sui_only_from_the_address_balance_requires_the_exact_total() {
    for withdraw in [SUI - 1, SUI + 1, 0] {
        let err = error_of(input(pay_sui(vec![], withdraw, &[ALICE], &[SUI]), true));
        assert_eq!(
            err,
            SigningErrorType::Error_invalid_params,
            "withdraw {withdraw}"
        );
    }
}

#[test]
fn test_pay_sui_rejects_a_zero_total_and_mismatched_lists() {
    let zero = error_of(input(pay_sui(vec![], 0, &[ALICE], &[0]), true));
    assert_eq!(zero, SigningErrorType::Error_invalid_params);
    let mismatch = error_of(input(pay_sui(vec![], SUI, &[ALICE, BOB], &[SUI]), true));
    assert_eq!(mismatch, SigningErrorType::Error_invalid_params);
    let overflow = error_of(input(
        pay_sui(vec![], u64::MAX, &[ALICE, BOB], &[u64::MAX, 1]),
        true,
    ));
    assert_eq!(overflow, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_pay_sui_rejects_a_recipient_with_a_zero_amount() {
    // Total and withdrawal agree (5), but BOB would get a zero-amount `redeem_funds`.
    let err = error_of(input(pay_sui(vec![], 5, &[ALICE, BOB], &[5, 0]), true));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
    let err = error_of(input(
        pay_token(vec![], 5, TOKEN, &[ALICE, BOB], &[5, 0], None),
        true,
    ));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_pay_token_gas_reservation_only_adds_up_with_a_sui_withdrawal() {
    // A huge TOKEN amount does not collide with the SUI gas budget (different balances)...
    let huge = u64::MAX - 1;
    let ok = build(input(
        pay_token(vec![], huge, TOKEN, &[ALICE], &[huge], None),
        true,
    ));
    assert!(ok.is_ok(), "{:?}", ok.err());
    // ...but the same amount of SUI does: withdrawal + gas budget overflows u64.
    let err = error_of(input(
        pay_token(vec![], huge, "0x2::sui::SUI", &[ALICE], &[huge], None),
        true,
    ));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_pay_sui_owned_coins_plus_the_address_balance_keep_the_change_in_the_coin() {
    // 2 SUI coin + 3 SUI withdrawn, 4 SUI sent: the withdrawal is merged into the coin first.
    let actual = built(input(
        pay_sui(vec![proto_ref(COIN_1, 7)], 3 * SUI, &[ALICE], &[4 * SUI]),
        true,
    ));
    let expected = expected_ab(
        vec![
            owned(COIN_1, 7),
            withdrawal(3 * SUI),
            pure_u64(4 * SUI),
            pure_address(ALICE),
        ],
        vec![
            redeem(1),
            Command::MergeCoins(Argument::Input(0), vec![result(0)]),
            Command::SplitCoins(Argument::Input(0), vec![Argument::Input(2)]),
            Command::TransferObjects(vec![Argument::NestedResult(2, 0)], Argument::Input(3)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_sui_owned_coins_only_with_the_address_balance_paying_gas() {
    // No withdrawal: only the gas moves to the address balance.
    let actual = built(input(
        pay_sui(
            vec![proto_ref(COIN_1, 7), proto_ref(COIN_2, 9)],
            0,
            &[ALICE],
            &[SUI],
        ),
        true,
    ));
    let expected = expected_ab(
        vec![
            owned(COIN_1, 7),
            owned(COIN_2, 9),
            pure_u64(SUI),
            pure_address(ALICE),
        ],
        vec![
            Command::MergeCoins(Argument::Input(0), vec![Argument::Input(1)]),
            Command::SplitCoins(Argument::Input(0), vec![Argument::Input(2)]),
            Command::TransferObjects(vec![Argument::NestedResult(1, 0)], Argument::Input(3)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_sui_gas_coin_with_a_withdrawal_merges_it_into_the_gas_coin() {
    // Coin gas (no `valid_during`): the redeemed coin joins `GasCoin`, which is then split.
    let actual = built(input(
        pay_sui(vec![proto_ref(COIN_1, 7)], 3 * SUI, &[ALICE], &[4 * SUI]),
        false,
    ));
    let expected = expected(
        vec![withdrawal(3 * SUI), pure_u64(4 * SUI), pure_address(ALICE)],
        vec![
            redeem(0),
            Command::MergeCoins(Argument::GasCoin, vec![result(0)]),
            Command::SplitCoins(Argument::GasCoin, vec![Argument::Input(1)]),
            Command::TransferObjects(vec![Argument::NestedResult(2, 0)], Argument::Input(2)),
        ],
        vec![object_ref(COIN_1, 7)],
        TransactionExpiration::None,
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_sui_without_valid_during_and_without_a_withdrawal_is_the_unchanged_legacy_transaction()
{
    // Byte-identical to the pre-SIP-58 builder (the signed mainnet fixtures in `sui_sign.rs` pin it).
    let actual = built(input(
        pay_sui(vec![proto_ref(COIN_1, 7)], 0, &[ALICE], &[SUI]),
        false,
    ));
    let expected = expected(
        vec![pure_u64(SUI), pure_address(ALICE)],
        vec![
            Command::SplitCoins(Argument::GasCoin, vec![Argument::Input(0)]),
            Command::TransferObjects(vec![Argument::NestedResult(0, 0)], Argument::Input(1)),
        ],
        vec![object_ref(COIN_1, 7)],
        TransactionExpiration::None,
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_a_withdrawal_needs_a_gas_coin_or_a_valid_during_window() {
    // No input coins (so no gas coin) and no window: nothing to pay gas with.
    let err = error_of(input(pay_sui(vec![], SUI, &[ALICE], &[SUI]), false));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}

// --------------------------------- PayAllSui --------------------------------

fn pay_all(coins: Vec<Proto::ObjectRef<'static>>, withdraw: u64) -> Payload<'static> {
    Payload::pay_all_sui(Proto::PayAllSui {
        input_coins: coins,
        recipient: ALICE.into(),
        address_balance_amount: withdraw,
    })
}

#[test]
fn test_pay_all_sui_only_from_the_address_balance() {
    let actual = built(input(pay_all(vec![], 7 * SUI), true));
    let expected = expected_ab(
        vec![withdrawal(7 * SUI), pure_address(ALICE)],
        vec![
            redeem(0),
            Command::TransferObjects(vec![result(0)], Argument::Input(1)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_all_sui_merges_every_coin_and_the_withdrawal_into_the_first_coin() {
    let actual = built(input(
        pay_all(vec![proto_ref(COIN_1, 7), proto_ref(COIN_2, 9)], 7 * SUI),
        true,
    ));
    let expected = expected_ab(
        vec![
            owned(COIN_1, 7),
            owned(COIN_2, 9),
            withdrawal(7 * SUI),
            pure_address(ALICE),
        ],
        vec![
            Command::MergeCoins(Argument::Input(0), vec![Argument::Input(1)]),
            redeem(2),
            Command::MergeCoins(Argument::Input(0), vec![result(1)]),
            Command::TransferObjects(vec![Argument::Input(0)], Argument::Input(3)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_all_sui_gas_coin_with_a_withdrawal() {
    let actual = built(input(pay_all(vec![proto_ref(COIN_1, 7)], 7 * SUI), false));
    let expected = expected(
        vec![withdrawal(7 * SUI), pure_address(ALICE)],
        vec![
            redeem(0),
            Command::MergeCoins(Argument::GasCoin, vec![result(0)]),
            Command::TransferObjects(vec![Argument::GasCoin], Argument::Input(1)),
        ],
        vec![object_ref(COIN_1, 7)],
        TransactionExpiration::None,
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_all_sui_withdrawal_needs_a_gas_coin_or_a_valid_during_window() {
    // A withdrawal but no input coins (so no gas coin) and no window: nothing to pay gas with.
    let err = error_of(input(pay_all(vec![], SUI), false));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_pay_all_sui_needs_something_to_send() {
    let err = error_of(input(pay_all(vec![], 0), true));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}

// ------------------------------- RequestAddStake -----------------------------

fn stake(
    coins: Vec<Proto::ObjectRef<'static>>,
    withdraw: u64,
    amount: Option<u64>,
    gas: Option<Proto::ObjectRef<'static>>,
) -> Payload<'static> {
    Payload::request_add_stake(Proto::RequestAddStake {
        coins,
        amount: amount.map(|amount| Proto::Amount { amount }),
        validator: VALIDATOR.into(),
        gas,
        address_balance_amount: withdraw,
    })
}

fn move_call_stake(args: Vec<Argument>) -> Command {
    Command::move_call(
        ObjectID(addr("0x3").into_inner()),
        Identifier::from_str("sui_system").unwrap(),
        Identifier::from_str("request_add_stake_mul_coin").unwrap(),
        vec![],
        args,
    )
}

/// The `SuiSystemState` shared object (0x5), initial shared version 1, mutable.
fn shared_system_state() -> CallArg {
    CallArg::SUI_SYSTEM_MUT
}

#[test]
fn test_stake_from_the_address_balance_only() {
    let actual = built(input(stake(vec![], 2 * SUI, Some(SUI), None), true));
    let expected = expected_ab(
        vec![
            shared_system_state(),
            withdrawal(2 * SUI),
            CallArg::Pure(bcs::encode(&Some(SUI)).unwrap()),
            pure_address(VALIDATOR),
        ],
        vec![
            redeem(1),
            Command::MakeMoveVec(None, vec![result(0)]),
            move_call_stake(vec![
                Argument::Input(0),
                result(1),
                Argument::Input(2),
                Argument::Input(3),
            ]),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_stake_owned_coins_plus_the_address_balance() {
    let actual = built(input(
        stake(vec![proto_ref(COIN_1, 7)], 2 * SUI, None, None),
        true,
    ));
    let expected = expected_ab(
        vec![
            shared_system_state(),
            owned(COIN_1, 7),
            withdrawal(2 * SUI),
            CallArg::Pure(bcs::encode(&None::<u64>).unwrap()),
            pure_address(VALIDATOR),
        ],
        vec![
            redeem(2),
            Command::MakeMoveVec(None, vec![Argument::Input(1), result(0)]),
            move_call_stake(vec![
                Argument::Input(0),
                result(1),
                Argument::Input(3),
                Argument::Input(4),
            ]),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_stake_with_a_gas_coin_is_unchanged_when_nothing_is_withdrawn() {
    let gas = Some(proto_ref(COIN_2, 9));
    let actual = built(input(
        stake(vec![proto_ref(COIN_1, 7)], 0, Some(SUI), gas),
        false,
    ));
    let expected = expected(
        vec![
            shared_system_state(),
            owned(COIN_1, 7),
            CallArg::Pure(bcs::encode(&Some(SUI)).unwrap()),
            pure_address(VALIDATOR),
        ],
        vec![
            Command::MakeMoveVec(None, vec![Argument::Input(1)]),
            move_call_stake(vec![
                Argument::Input(0),
                result(0),
                Argument::Input(2),
                Argument::Input(3),
            ]),
        ],
        vec![object_ref(COIN_2, 9)],
        TransactionExpiration::None,
    );
    assert_eq!(actual, expected);
}

// ------------- arms that only change how gas is paid: same kind, new gas ------

#[test]
fn test_unstake_pay_and_transfer_object_only_swap_the_gas_payment() {
    let unstake = |gas: Option<Proto::ObjectRef<'static>>| {
        Payload::request_withdraw_stake(Proto::RequestWithdrawStake {
            staked_sui: Some(proto_ref(COIN_1, 7)),
            gas,
        })
    };
    let pay = |gas: Option<Proto::ObjectRef<'static>>| {
        Payload::pay(Proto::Pay {
            input_coins: vec![proto_ref(COIN_1, 7), proto_ref(COIN_2, 9)],
            recipients: vec![ALICE.into(), BOB.into()],
            amounts: vec![5, 6],
            gas,
            ..Proto::Pay::default()
        })
    };
    let transfer = |gas: Option<Proto::ObjectRef<'static>>| {
        Payload::transfer_object(Proto::TransferObject {
            recipient: ALICE.into(),
            object: Some(proto_ref(COIN_1, 7)),
            gas,
        })
    };
    for (name, with_gas, ab_gas) in [
        (
            "unstake",
            built(input(unstake(Some(proto_ref(GAS_COIN, 3))), false)),
            built(input(unstake(None), true)),
        ),
        (
            "pay",
            built(input(pay(Some(proto_ref(GAS_COIN, 3))), false)),
            built(input(pay(None), true)),
        ),
        (
            "transfer_object",
            built(input(transfer(Some(proto_ref(GAS_COIN, 3))), false)),
            built(input(transfer(None), true)),
        ),
    ] {
        assert_eq!(
            kind_of(&with_gas),
            kind_of(&ab_gas),
            "{name}: same commands"
        );
        let (payment, expiration) = gas_of(&ab_gas);
        assert!(payment.is_empty(), "{name}: no gas object");
        assert_eq!(expiration, bcs::encode(&window()).unwrap(), "{name}");
        let (payment, expiration) = gas_of(&with_gas);
        assert_eq!(payment.len(), 1, "{name}");
        assert_eq!(
            expiration,
            bcs::encode(&TransactionExpiration::None).unwrap()
        );
    }
}

// ---------------------------- contract / validation --------------------------

fn unstake_ab() -> Payload<'static> {
    Payload::request_withdraw_stake(Proto::RequestWithdrawStake {
        staked_sui: Some(proto_ref(COIN_1, 7)),
        gas: None,
    })
}

#[test]
fn test_gas_coin_and_valid_during_are_mutually_exclusive() {
    let payload = Payload::request_withdraw_stake(Proto::RequestWithdrawStake {
        staked_sui: Some(proto_ref(COIN_1, 7)),
        gas: Some(proto_ref(COIN_2, 9)),
    });
    assert_eq!(
        error_of(input(payload, true)),
        SigningErrorType::Error_invalid_params
    );
    // Neither a gas coin nor a window.
    assert_eq!(
        error_of(input(unstake_ab(), false)),
        SigningErrorType::Error_invalid_params
    );
}

#[test]
fn test_valid_during_window_is_validated() {
    let with = |edit: fn(&mut Proto::ValidDuring<'static>)| {
        let mut i = input(unstake_ab(), true);
        edit(i.valid_during.as_mut().unwrap());
        i
    };
    // Accepted: the current epoch alone, and the current plus the next one.
    build(with(|v| v.max_epoch = v.min_epoch)).unwrap();
    build(with(|_| {})).unwrap();
    // Rejected.
    for (name, bad) in [
        ("inverted", with(|v| v.max_epoch = v.min_epoch - 1)),
        ("three epochs", with(|v| v.max_epoch = v.min_epoch + 2)),
        ("not base58", with(|v| v.chain = "0OIl".into())),
        ("short chain id", with(|v| v.chain = "3mH".into())),
    ] {
        assert_eq!(
            error_of(bad),
            SigningErrorType::Error_invalid_params,
            "{name}"
        );
    }
    let mut no_budget = input(unstake_ab(), true);
    no_budget.gas_budget = 0;
    assert_eq!(error_of(no_budget), SigningErrorType::Error_invalid_params);
}

#[test]
fn test_withdrawal_plus_gas_budget_must_not_overflow() {
    let mut i = input(pay_all(vec![], u64::MAX), true);
    i.gas_budget = 1;
    assert_eq!(error_of(i), SigningErrorType::Error_invalid_params);
}

#[test]
fn test_valid_during_is_rejected_where_it_would_be_ignored() {
    let sign_direct = Payload::sign_direct_message(Proto::SignDirect {
        unsigned_tx_msg: "AAA=".into(),
    });
    let raw_json = Payload::raw_json("{}".into());
    for (name, payload) in [("sign_direct_message", sign_direct), ("raw_json", raw_json)] {
        assert_eq!(
            error_of(input(payload, true)),
            SigningErrorType::Error_invalid_params,
            "{name}"
        );
    }
}

#[test]
fn test_the_nonce_is_part_of_the_transaction() {
    let with = built(input(unstake_ab(), true));
    let mut other = input(unstake_ab(), true);
    other.valid_during.as_mut().unwrap().nonce += 1;
    assert_ne!(with, built(other), "the nonce is part of the transaction");
}

// ------------------------ tokens held in the address balance ------------------------

const TOKEN: &str =
    "0xdba34672e30cb065b1f93e3ab55318768fd6fef66c15942c9f7cb846e2f900e7::usdc::USDC";

/// `0xdba3...::usdc::USDC`, written by hand (not through the parser under test).
fn token_tag() -> TypeTag {
    TypeTag::Struct(Box::new(StructTag {
        address: addr("0xdba34672e30cb065b1f93e3ab55318768fd6fef66c15942c9f7cb846e2f900e7")
            .into_inner(),
        module: Identifier::from_str("usdc").unwrap(),
        name: Identifier::from_str("USDC").unwrap(),
        type_params: vec![],
    }))
}

fn token_withdrawal(amount: u64) -> CallArg {
    CallArg::FundsWithdrawal(FundsWithdrawalArg::balance_from_sender(amount, token_tag()))
}

/// `0x2::coin::redeem_funds<TOKEN>(Input(withdrawal))`
fn redeem_token(withdrawal_input: u16) -> Command {
    Command::move_call(
        ObjectID(addr("0x2").into_inner()),
        Identifier::from_str("coin").unwrap(),
        Identifier::from_str("redeem_funds").unwrap(),
        vec![token_tag()],
        vec![Argument::Input(withdrawal_input)],
    )
}

fn pay_token(
    coins: Vec<Proto::ObjectRef<'static>>,
    withdraw: u64,
    coin_type: &'static str,
    recipients: &[&'static str],
    amounts: &[u64],
    gas: Option<Proto::ObjectRef<'static>>,
) -> Payload<'static> {
    Payload::pay(Proto::Pay {
        input_coins: coins,
        recipients: recipients.iter().map(|r| Cow::Borrowed(*r)).collect(),
        amounts: amounts.to_vec(),
        gas,
        coin_type: coin_type.into(),
        address_balance_amount: withdraw,
    })
}

#[test]
fn test_pay_token_only_from_the_address_balance_redeems_exactly_the_amount() {
    let actual = built(input(
        pay_token(vec![], 7_000_000, TOKEN, &[ALICE], &[7_000_000], None),
        true,
    ));
    let expected = expected_ab(
        vec![token_withdrawal(7_000_000), pure_address(ALICE)],
        vec![
            redeem_token(0),
            Command::TransferObjects(vec![result(0)], Argument::Input(1)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_token_only_from_the_address_balance_one_coin_per_distinct_recipient() {
    let actual = built(input(
        pay_token(vec![], 6, TOKEN, &[ALICE, BOB, ALICE], &[1, 2, 3], None),
        true,
    ));
    let expected = expected_ab(
        vec![
            token_withdrawal(4),
            pure_address(ALICE),
            token_withdrawal(2),
            pure_address(BOB),
        ],
        vec![
            redeem_token(0),
            Command::TransferObjects(vec![result(0)], Argument::Input(1)),
            redeem_token(2),
            Command::TransferObjects(vec![result(2)], Argument::Input(3)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_token_owned_coins_plus_the_address_balance_keep_the_change_in_the_coin() {
    let actual = built(input(
        pay_token(vec![proto_ref(COIN_1, 7)], 3, TOKEN, &[ALICE], &[10], None),
        true,
    ));
    let expected = expected_ab(
        vec![
            owned(COIN_1, 7),
            token_withdrawal(3),
            pure_u64(10),
            pure_address(ALICE),
        ],
        vec![
            redeem_token(1),
            Command::MergeCoins(Argument::Input(0), vec![result(0)]),
            Command::SplitCoins(Argument::Input(0), vec![Argument::Input(2)]),
            Command::TransferObjects(vec![Argument::NestedResult(2, 0)], Argument::Input(3)),
        ],
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_token_withdrawal_with_a_separate_sui_gas_coin() {
    // No `valid_during`: a SUI coin pays gas, the token comes out of the address balance.
    let actual = built(input(
        pay_token(
            vec![],
            5,
            TOKEN,
            &[ALICE],
            &[5],
            Some(proto_ref(GAS_COIN, 3)),
        ),
        false,
    ));
    let expected = expected(
        vec![token_withdrawal(5), pure_address(ALICE)],
        vec![
            redeem_token(0),
            Command::TransferObjects(vec![result(0)], Argument::Input(1)),
        ],
        vec![object_ref(GAS_COIN, 3)],
        TransactionExpiration::None,
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_token_withdrawal_gas_coin_cannot_be_an_input_coin() {
    let err = error_of(input(
        pay_token(
            vec![proto_ref(COIN_1, 7)],
            5,
            TOKEN,
            &[ALICE],
            &[5],
            Some(proto_ref(COIN_1, 7)),
        ),
        false,
    ));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_pay_token_coin_type_and_withdrawal_go_together() {
    let invalid = SigningErrorType::Error_invalid_params;
    // A withdrawal needs the coin type: nothing is guessed.
    assert_eq!(
        error_of(input(pay_token(vec![], 5, "", &[ALICE], &[5], None), true)),
        invalid
    );
    // A coin type without a withdrawal is a mistake, not a no-op.
    assert_eq!(
        error_of(input(
            pay_token(vec![proto_ref(COIN_1, 7)], 0, TOKEN, &[ALICE], &[5], None),
            true
        )),
        invalid
    );
}

#[test]
fn test_pay_token_rejects_a_malformed_or_non_coin_type() {
    for coin_type in [
        "usdc",
        "0x2::coin::Coin<0x2::sui::SUI",
        "u64",
        "vector<u8>",
        "0x2::sui",
        "",
    ] {
        let err = error_of(input(
            pay_token(vec![], 5, coin_type, &[ALICE], &[5], None),
            true,
        ));
        assert_eq!(err, SigningErrorType::Error_invalid_params, "{coin_type:?}");
    }
}

#[test]
fn test_pay_token_only_from_the_address_balance_requires_the_exact_total() {
    for withdraw in [4, 6] {
        let err = error_of(input(
            pay_token(vec![], withdraw, TOKEN, &[ALICE], &[5], None),
            true,
        ));
        assert_eq!(
            err,
            SigningErrorType::Error_invalid_params,
            "withdraw {withdraw}"
        );
    }
}

#[test]
fn test_pay_without_a_token_withdrawal_is_the_unchanged_legacy_transaction() {
    // Same bytes as before SIP-58: a token coin, a SUI gas coin, no window, no withdrawal.
    let actual = built(input(
        pay_token(
            vec![proto_ref(COIN_1, 7), proto_ref(COIN_2, 9)],
            0,
            "",
            &[ALICE],
            &[5],
            Some(proto_ref(GAS_COIN, 3)),
        ),
        false,
    ));
    let expected = expected(
        vec![
            owned(COIN_1, 7),
            owned(COIN_2, 9),
            pure_u64(5),
            pure_address(ALICE),
        ],
        vec![
            Command::MergeCoins(Argument::Input(0), vec![Argument::Input(1)]),
            Command::SplitCoins(Argument::Input(0), vec![Argument::Input(2)]),
            Command::TransferObjects(vec![Argument::NestedResult(1, 0)], Argument::Input(3)),
        ],
        vec![object_ref(GAS_COIN, 3)],
        TransactionExpiration::None,
    );
    assert_eq!(actual, expected);
}

#[test]
fn test_pay_sui_and_pay_all_sui_without_any_coin_or_withdrawal_are_rejected() {
    // The legacy path (no `valid_during`, nothing withdrawn) still needs a coin to spend.
    let pay = error_of(input(pay_sui(vec![], 0, &[ALICE], &[SUI]), false));
    assert_eq!(pay, SigningErrorType::Error_invalid_params);
    let pay_all = error_of(input(pay_all(vec![], 0), false));
    assert_eq!(pay_all, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_a_signing_input_without_a_transaction_payload_is_rejected() {
    let err = error_of(input(Payload::None, true));
    assert_eq!(err, SigningErrorType::Error_invalid_params);
}
