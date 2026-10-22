// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! SIP-58: signing through `AnySigner` with gas paid from the address balance and funds withdrawn
//! from it. These pin the bytes the Android (JNI) and iOS bindings must reproduce for the same input.
//! What the transactions contain is asserted structurally in `tw_sui/tests/address_balance_builder.rs`;
//! the same builders were executed on Sui devnet (see the tsdk `sui-address-balance.devnet.test.ts`).

use crate::chains::sui::test_cases::{PRIVATE_KEY_54E80D76, SENDER_54E80D76};
use std::str::FromStr;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as TransactionType};
use tw_sui::address::SuiAddress;
use tw_sui::transaction::sui_types::{CallArg, Reservation, WithdrawFrom};
use tw_sui::transaction::transaction_data::{TransactionData, TransactionKind};

const MAINNET_CHAIN: &str = "4btiuiMPvEENsttpZC7CZ53DruC3MAgfznDbASZ7DR6S";
const RECIPIENT: &str = "0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a";
const TOKEN: &str =
    "0xdba34672e30cb065b1f93e3ab55318768fd6fef66c15942c9f7cb846e2f900e7::usdc::USDC";
const VALIDATOR: &str = "0x61953ea72709eed72f4441dd944eec49a11b4acabfc8e04015e89c63be81b6ab";

fn input(payload: TransactionType<'static>) -> Proto::SigningInput<'static> {
    Proto::SigningInput {
        transaction_payload: payload,
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        gas_budget: 5_000_000,
        reference_gas_price: 100,
        valid_during: Some(Proto::ValidDuring {
            min_epoch: 1288,
            max_epoch: 1289,
            chain: MAINNET_CHAIN.into(),
            nonce: 12345,
        }),
        ..Proto::SigningInput::default()
    }
}

fn sign_and_check(
    payload: TransactionType<'static>,
    expected_unsigned_tx: &str,
    expected_signature: &str,
) {
    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input(payload));
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    assert_eq!(output.unsigned_tx, expected_unsigned_tx);
    assert_eq!(output.signature, expected_signature);
}

#[test]
fn test_sui_sign_pay_sui_from_address_balance() {
    let pay_sui = Proto::PaySui {
        recipients: vec![RECIPIENT.into()],
        amounts: vec![1_000_000_000],
        address_balance_amount: 1_000_000_000,
        ..Proto::PaySui::default()
    };
    sign_and_check(TransactionType::pay_sui(pay_sui), "AAACAgAAypo7AAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACCnF1q91e2S6+OtOQ2zZsanBkeM31F83mz5hjAGXNo3egIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEAAAEBAgAAAQEAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rJkAAAAAAAAAEBLTAAAAAAAAgEIBQAAAAAAAAEJBQAAAAAAAAAAIDWDSorBfKSPsUrI+ZwXyYdH6V3QcpSuQaRrOCJGpEmbOTAAAA==", "AL0Td0eKAdFam0WCkSO240Q0Mc5evlwFj5Sr4D8w1TGxtEHoJ2hzWcsytyIIgzhdFjyUgEtPoCHQvXOOCkq5/Q2F69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_pay_token_from_address_balance() {
    // A token (not SUI) held only in the address balance: redeemed with `0x2::coin::redeem_funds<T>`,
    // gas still paid from the SUI address balance.
    let pay = Proto::Pay {
        recipients: vec![RECIPIENT.into()],
        amounts: vec![7_000_000],
        coin_type: TOKEN.into(),
        address_balance_amount: 7_000_000,
        ..Proto::Pay::default()
    };
    sign_and_check(TransactionType::pay(pay), "AAACAgDAz2oAAAAAAAAH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAAAIKcXWr3V7ZLr4605DbNmxqcGR4zfUXzebPmGMAZc2jd6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgRjb2luDHJlZGVlbV9mdW5kcwEH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAEBAAABAQIAAAEBAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayZAAAAAAAAABAS0wAAAAAAAIBCAUAAAAAAAABCQUAAAAAAAAAACA1g0qKwXykj7FKyPmcF8mHR+ld0HKUrkGkazgiRqRJmzkwAAA=", "AK5moGv2+NcANkUVDvQGsjNbUwYTP++hpPnJlnUgDhD7dfHkHOsv6SHj8Cwoh4ZbTJSQ8i8IYTISYaANmdQ7tQqF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_pay_all_sui_from_address_balance() {
    let pay_all = Proto::PayAllSui {
        recipient: RECIPIENT.into(),
        address_balance_amount: 2_000_000_000,
        ..Proto::PayAllSui::default()
    };
    sign_and_check(TransactionType::pay_all_sui(pay_all), "AAACAgAAlDV3AAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACCnF1q91e2S6+OtOQ2zZsanBkeM31F83mz5hjAGXNo3egIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEAAAEBAgAAAQEAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rJkAAAAAAAAAEBLTAAAAAAAAgEIBQAAAAAAAAEJBQAAAAAAAAAAIDWDSorBfKSPsUrI+ZwXyYdH6V3QcpSuQaRrOCJGpEmbOTAAAA==", "AKwcuOoCM601XIX4mEH4lfy8FqI7XqOEswhP/hbJkiKB1P4hATcV9Tbx4aZE3HScyFPBRCjEH04Dg201O5exywCF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_add_stake_from_address_balance() {
    let stake = Proto::RequestAddStake {
        amount: Some(Proto::Amount {
            amount: 1_000_000_000,
        }),
        validator: VALIDATOR.into(),
        address_balance_amount: 1_000_000_000,
        ..Proto::RequestAddStake::default()
    };
    sign_and_check(TransactionType::request_add_stake(stake), "AAAEAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQIAAMqaOwAAAAAABwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAAAJAQDKmjsAAAAAACBhlT6nJwnu1y9EQd2UTuxJoRtKyr/I4EAV6JxjvoG2qwMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEBAAUAAQIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwpzdWlfc3lzdGVtGnJlcXVlc3RfYWRkX3N0YWtlX211bF9jb2luAAQBAAACAQABAgABAwBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsmQAAAAAAAAAQEtMAAAAAAACAQgFAAAAAAAAAQkFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZs5MAAA", "AFbsYoHh3yjNs479Y42OEpp+0DYKcRQTxHcJk584SCZv6WAmAk2vTzGdu+/sKR3KP18MkYSDR2tigQfhuRxQgwmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_signer_address_is_the_withdrawal_owner() {
    // The funds come out of the transaction sender's address balance: it has to be the key's address,
    // it also owns the gas, and the withdrawal reads from the sender.
    let pay_sui = Proto::PaySui {
        recipients: vec![RECIPIENT.into()],
        amounts: vec![1],
        address_balance_amount: 1,
        ..Proto::PaySui::default()
    };
    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input(TransactionType::pay_sui(pay_sui)));
    let raw =
        tw_encoding::base64::decode(&output.unsigned_tx, tw_encoding::base64::STANDARD).unwrap();
    let TransactionData::V1(tx) = tw_encoding::bcs::decode(&raw).unwrap();

    let sender = SuiAddress::from_str(SENDER_54E80D76).unwrap();
    assert_eq!(tx.sender, sender);
    assert_eq!(tx.gas_data.owner, sender);
    assert!(
        tx.gas_data.payment.is_empty(),
        "gas comes from the address balance"
    );
    let TransactionKind::ProgrammableTransaction(ptb) = tx.kind;
    let withdrawals: Vec<_> = ptb
        .inputs
        .iter()
        .filter_map(|arg| match arg {
            CallArg::FundsWithdrawal(w) => Some(w),
            _ => None,
        })
        .collect();
    assert_eq!(withdrawals.len(), 1);
    assert!(matches!(withdrawals[0].withdraw_from, WithdrawFrom::Sender));
    assert!(matches!(
        withdrawals[0].reservation,
        Reservation::MaxAmountU64(1)
    ));
}
