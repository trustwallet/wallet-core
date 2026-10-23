// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! SIP-58: signing through `AnySigner` with gas paid from the address balance and funds withdrawn
//! from it. These pin the bytes the Android (JNI) and iOS bindings must reproduce for the same input.
//! What the transactions contain is asserted structurally in `tw_sui/tests/address_balance_builder.rs`;
//! the same builders were executed on Sui devnet (see the tsdk `sui-address-balance.devnet.test.ts`).

use crate::chains::sui::test_cases::{PRIVATE_KEY_54E80D76, SENDER_54E80D76};
use std::str::FromStr;
use tw_any_coin::test_utils::sign_utils::{AnySignerHelper, CompilerHelper};
use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
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

const WALLET_A: &str = "0x836df78a1ea9a9d9ca8f2ae92fc6799c72f63743d655bcfb5af90f70c599caab";
const WALLET_B: &str = "0x87523b4864c9858aea9f6f7692ba81fb4d0692a0bf718c76869add391f69ecfa";
const SOURCE: &str = "0x3b5bfe5427c241c78f45da9fef99a36a20f903658670b0eba424901ef76e1f8b";
const CITADEL_VALIDATOR: &str =
    "0x4f9791d5c689306862b4eb9a25914c5433b7dfd5cb4827b461f7dfc813f28a7c";

/// Rebuilds a transaction that was executed on Sui mainnet and checks that WalletCore produces the
/// very same bytes: the unsigned transaction must equal the on-chain `transactionBcs`, the on-chain
/// signature must verify against our signing hash (compile fails otherwise), and the transaction
/// digest must equal the one the chain reports.
fn assert_matches_mainnet(
    signer: &str,
    window: Option<(u64, u32)>,
    gas_budget: u64,
    payload: TransactionType<'static>,
    on_chain_tx: &str,
    on_chain_signature: &str,
    on_chain_digest: &str,
) {
    let mut input = input(payload);
    input.private_key = Default::default();
    input.signer = signer.into();
    input.gas_budget = gas_budget;
    match window {
        Some((min_epoch, nonce)) => {
            let valid_during = input.valid_during.as_mut().unwrap();
            valid_during.min_epoch = min_epoch;
            valid_during.max_epoch = min_epoch + 1;
            valid_during.nonce = nonce;
        },
        None => input.valid_during = None,
    }

    // On-chain signature: flag (1) | signature (64) | public key (32).
    let raw =
        tw_encoding::base64::decode(on_chain_signature, tw_encoding::base64::STANDARD).unwrap();
    let (signature, public_key) = (raw[1..65].to_vec(), raw[65..].to_vec());

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::Sui, &input, vec![signature], vec![public_key]);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    assert_eq!(output.unsigned_tx, on_chain_tx);
    assert_eq!(output.signature, on_chain_signature);
    assert_eq!(
        TransactionUtilHelper::calc_tx_hash(CoinType::Sui, on_chain_tx),
        on_chain_digest
    );
}

#[test]
fn test_sui_mainnet_pay_sui_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/Ffiza1ksFJ885TPZnjssvYdk6YgJivAy7TpzSgZVUJn2
    let pay_sui = Proto::PaySui {
        recipients: vec![WALLET_B.into()],
        amounts: vec![100_000_000],
        address_balance_amount: 100_000_000,
        ..Proto::PaySui::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_288, 274159711)),
        2_091_000,
        TransactionType::pay_sui(pay_sui),
        "AAACAgAA4fUFAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACCHUjtIZMmFiuqfb3aSuoH7TQaSoL9xjHaGmt05H2ns+gIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEAAAEBAgAAAQEAg233ih6pqdnKjyrpL8Z5nHL2N0PWVbz7WvkPcMWZyqsAg233ih6pqdnKjyrpL8Z5nHL2N0PWVbz7WvkPcMWZyqtkAAAAAAAAAPjnHwAAAAAAAgEIBQAAAAAAAAEJBQAAAAAAAAAAIDWDSorBfKSPsUrI+ZwXyYdH6V3QcpSuQaRrOCJGpEmbX1hXEA==",
        "AMLBqbKol5X2jh0ZVUYiOGfZBYJPRVXLU/H3rOw50APXcFMJQemvvBS7nxqgEiW85ebdZWAq3PyEMRdthCtMZwTGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "Ffiza1ksFJ885TPZnjssvYdk6YgJivAy7TpzSgZVUJn2",
    );
}

#[test]
fn test_sui_mainnet_add_stake_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/cTLRHRgKeFd5DSaNHvgFgGQm5gUnsJWGoLxyGWwh6rU
    let stake = Proto::RequestAddStake {
        amount: Some(Proto::Amount {
            amount: 1_500_000_000,
        }),
        validator: CITADEL_VALIDATOR.into(),
        address_balance_amount: 1_500_000_000,
        ..Proto::RequestAddStake::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_288, 289905214)),
        23_000_000,
        TransactionType::request_add_stake(stake),
        "AAAEAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQIAAC9oWQAAAAAABwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAAAJAQAvaFkAAAAAACBPl5HVxokwaGK065olkUxUM7ff1ctIJ7Rh99/IE/KKfAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEBAAUAAQIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwpzdWlfc3lzdGVtGnJlcXVlc3RfYWRkX3N0YWtlX211bF9jb2luAAQBAAACAQABAgABAwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAwPNeAQAAAAACAQgFAAAAAAAAAQkFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZs+mkcR",
        "APPmNqI0Z/dubZZCkojBr3Qu38ycg6g/JkCoWeqXvxp8hs1+bzA+oVUGeMQHVAnE4O28RJcpAwIQ77a5Ug6uxwnGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "cTLRHRgKeFd5DSaNHvgFgGQm5gUnsJWGoLxyGWwh6rU",
    );
}

#[test]
fn test_sui_mainnet_pay_token_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/363oW4VnTAsx1TdbRfi3xPBwWU5EieVoDDS7iZt6emAC
    let pay = Proto::Pay {
        recipients: vec![SOURCE.into()],
        amounts: vec![100_000],
        coin_type: TOKEN.into(),
        address_balance_amount: 100_000,
        ..Proto::Pay::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_288, 4_276_538_772)),
        5_000_000,
        TransactionType::pay(pay),
        "AAACAgCghgEAAAAAAAAH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAAAIDtb/lQnwkHHj0Xan++Zo2og+QNlhnCw66QkkB73bh+LAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgRjb2luDHJlZGVlbV9mdW5kcwEH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAEBAAABAQIAAAEBAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrZAAAAAAAAABAS0wAAAAAAAIBCAUAAAAAAAABCQUAAAAAAAAAACA1g0qKwXykj7FKyPmcF8mHR+ld0HKUrkGkazgiRqRJm5TN5v4=",
        "ANxbQ1dQz8R4UP1CQ1XC27fZtOO/uPF6fZBkpiCpLIcDIxy5pGvOVtvyV4QJGy2qM8q/4wj8WKa+0p7wAdNJsQfGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "363oW4VnTAsx1TdbRfi3xPBwWU5EieVoDDS7iZt6emAC",
    );
}

#[test]
fn test_sui_mainnet_pay_all_sui_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/7WD3PNRutXHxtXPrrSvR5qP2NpaHznu2vsjNE3eZwWjE
    let pay_all = Proto::PayAllSui {
        recipient: SOURCE.into(),
        address_balance_amount: 895_338_271,
        ..Proto::PayAllSui::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_288, 3_891_899_009)),
        2_091_000,
        TransactionType::pay_all_sui(pay_all),
        "AAACAgAfx101AAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEAAAEBAgAAAQEAg233ih6pqdnKjyrpL8Z5nHL2N0PWVbz7WvkPcMWZyqsAg233ih6pqdnKjyrpL8Z5nHL2N0PWVbz7WvkPcMWZyqtkAAAAAAAAAPjnHwAAAAAAAgEIBQAAAAAAAAEJBQAAAAAAAAAAIDWDSorBfKSPsUrI+ZwXyYdH6V3QcpSuQaRrOCJGpEmbgar55w==",
        "AHAlZxDGgmfwvgwmR71JZTFRCydOX7BEP5Tmpk94Ko9MxAPPI9G9pvUp/cvu7iSmgLuBfRLeX9GUVjXr6eycsQzGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "7WD3PNRutXHxtXPrrSvR5qP2NpaHznu2vsjNE3eZwWjE",
    );
}

#[test]
fn test_sui_mainnet_pay_all_sui_coin_plus_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/9KnM2etjKkXsUKUyGcoo6hBaCWmuqiWNxH7mEbgKd7FA
    let pay_all = Proto::PayAllSui {
        input_coins: vec![Proto::ObjectRef {
            object_id: "0x8dc7d638f5f52beecaba3e9cd0ebe404c26333b187a49f1424cda8f459825dfb".into(),
            version: 1_058_222_906,
            object_digest: "8TU818KumrXSsbjaZGzrDcRKRKLztGtsqUY8D2sZwXX".into(),
        }],
        recipient: SOURCE.into(),
        address_balance_amount: 8_116_142,
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_288, 2_177_643_431)),
        3_000_000,
        TransactionType::pay_all_sui(pay_all),
        "AAADAQCNx9Y49fUr7sq6PpzQ6+QEwmMzsYeknxQkzaj0WYJd+zozEz8AAAAAIAHo/h/Okee7iuwQyxShO/KZlop3wIH+DdSqQBp3iUXCAgCu13sAAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEBAAMBAAABAgAAAQEBAAABAgCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAwMYtAAAAAAACAQgFAAAAAAAAAQkFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZunM8yB",
        "ALQYvXWnfoqihBRk35lGkaVeWmxu+EV6XLEV+byzHLDi0jf36pttX1NwOF6JsLTL66mLxbjMmYiwpq65oIpcJwLGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "9KnM2etjKkXsUKUyGcoo6hBaCWmuqiWNxH7mEbgKd7FA",
    );
}

#[test]
fn test_sui_mainnet_request_withdraw_stake_gas_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/DU8AAP3NGHwUtbTTuc4BUUtMMw5FZVDpSCbd4hu6Waak
    let unstake = Proto::RequestWithdrawStake {
        staked_sui: Some(Proto::ObjectRef {
            object_id: "0x889337e1e9a88fd1d4f174ed06bb08ad7003ce1fa6f0d4928e8bf4b10889fb9a".into(),
            version: 1_058_210_595,
            object_digest: "9t9G6TVcQe4gmxqKoc5bKpx5NUjmknFyQHFE9Lrr8zwA".into(),
        }),
        ..Proto::RequestWithdrawStake::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_288, 3_241_074_455)),
        5_000_000,
        TransactionType::request_withdraw_stake(unstake),
        "AAACAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQEAiJM34emoj9HU8XTtBrsIrXADzh+m8NSSjov0sQiJ+5ojAxM/AAAAACCD93ThHQJe6tzVtgTOvyXZlAAJNB1V2jSoQc0UQh8P0QEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMKc3VpX3N5c3RlbRZyZXF1ZXN0X3dpdGhkcmF3X3N0YWtlAAIBAAABAQCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAACAQgFAAAAAAAAAQkFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZsX3y7B",
        "AIxfYvziLmuD2uFFlKLtw3zWZCL627CMXZ+wfdY66nFvB0aP4x9TLQCwDWmEALfDtNCn6Di7tQrEX6kL8ocJQA7GMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "DU8AAP3NGHwUtbTTuc4BUUtMMw5FZVDpSCbd4hu6Waak",
    );
}

#[test]
fn test_sui_mainnet_pay_sui_coin_plus_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/4rpxFcVUb4iXCnRRKp7fjAYcEjoRt7kXFYLxT9B2L23p
    let pay_sui = Proto::PaySui {
        input_coins: vec![
            Proto::ObjectRef {
                object_id: "0xa67bd63299bb32d365006f0d167e5b70ad376d7a869fe62504cd84fc023c9496"
                    .into(),
                version: 1,
                object_digest: "8Ftq4t1xoAcUAmtDubQCCrbWKhv929KTnsUg1xEKeXrJ".into(),
            },
            Proto::ObjectRef {
                object_id: "0x3d65fd1d7ee57f956cd4d4dadf7402a2e71f3f7daab061de63abd4542519acf9"
                    .into(),
                version: 995_462_529,
                object_digest: "CXHLq4nbqkMHcLFDLoPVdhZoWMRUsiL7WfuaC9boqsbF".into(),
            },
        ],
        recipients: vec![SOURCE.into()],
        amounts: vec![700_000_000],
        address_balance_amount: 200_000_000,
    };
    assert_matches_mainnet(
        WALLET_B,
        Some((1_288, 1_291_719_829)),
        1_103_198,
        TransactionType::pay_sui(pay_sui),
        "AAAFAQCme9Yymbsy02UAbw0WfltwrTdteoaf5iUEzYT8AjyUlgEAAAAAAAAAIGvSivHuS3P0mM4UZk8f5bGxunHDbg+9ATEn+fikRq4rAQA9Zf0dfuV/lWzU1NrfdAKi5x8/faqwYd5jq9RUJRms+YGNVTsAAAAAIKsydVG65XVj5G/7CziPCkRrG/BVHnOIXciVW6iBdSbqAgAAwusLAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAAAgAJ7kpAAAAAAAgO1v+VCfCQcePRdqf75mjaiD5A2WGcLDrpCSQHvduH4sFAwEAAAEBAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQECAAMBAAABAgEAAgEAAAEBAwABAQMDAAAAAQQAh1I7SGTJhYrqn292krqB+00GkqC/cYx2hprdOR9p7PoAh1I7SGTJhYrqn292krqB+00GkqC/cYx2hprdOR9p7PpkAAAAAAAAAF7VEAAAAAAAAgEIBQAAAAAAAAEJBQAAAAAAAAAAIDWDSorBfKSPsUrI+ZwXyYdH6V3QcpSuQaRrOCJGpEmblRT+TA==",
        "ANl0GzsNwvg+xIEviKK/ruBpm+xOtaodSkps7ylMJg8gI+wPYN4VMXX87dJ0uwUQpoEV0PtigK6Sl8y8OWMb3wy22xBlOF4TBNET8OCIFtfPwRBB0jBbC9r/CjKxCX5nOA==",
        "4rpxFcVUb4iXCnRRKp7fjAYcEjoRt7kXFYLxT9B2L23p",
    );
}

#[test]
fn test_sui_mainnet_pay_all_sui_empty_coin_plus_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/8BUuA4rpqiQUiVZkonD8KTx5euPwgpdCv1meEbPEoXGZ
    let pay_all = Proto::PayAllSui {
        input_coins: vec![Proto::ObjectRef {
            object_id: "0xa67bd63299bb32d365006f0d167e5b70ad376d7a869fe62504cd84fc023c9496".into(),
            version: 995_462_530,
            object_digest: "H3ojzARkpsjLPeisdrnZX9owVSMUDGDPc2rCAuA1R1ev".into(),
        }],
        recipient: SOURCE.into(),
        address_balance_amount: 198_796_703,
    };
    assert_matches_mainnet(
        WALLET_B,
        Some((1_288, 1_953_034_527)),
        1_103_099,
        TransactionType::pay_all_sui(pay_all),
        "AAADAQCme9Yymbsy02UAbw0WfltwrTdteoaf5iUEzYT8AjyUloKNVTsAAAAAIO5zALfJnyc1I5rGnsoI8thKU/d9KxXzBK7fckZ+niN3AgCfZdkLAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEBAAMBAAABAgAAAQEBAAABAgCHUjtIZMmFiuqfb3aSuoH7TQaSoL9xjHaGmt05H2ns+gCHUjtIZMmFiuqfb3aSuoH7TQaSoL9xjHaGmt05H2ns+mQAAAAAAAAA+9QQAAAAAAACAQgFAAAAAAAAAQkFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZsf8Wh0",
        "AGTZ+ixeIVIuMsGONB0nWMs9dNpVunPiywQK96icr1nuWipccam1lVJ0EhUxndKw2cFdEJmFB3zuKu1GjgallQK22xBlOF4TBNET8OCIFtfPwRBB0jBbC9r/CjKxCX5nOA==",
        "8BUuA4rpqiQUiVZkonD8KTx5euPwgpdCv1meEbPEoXGZ",
    );
}

#[test]
fn test_sui_mainnet_pay_sui_gas_coin_plus_withdrawal() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/6qB8xFseugVfxPrTMAXoMxWG5Phearr4hZtEXZFSBJbU
    let pay_sui = Proto::PaySui {
        input_coins: vec![Proto::ObjectRef {
            object_id: "0x039477668ecae25f3ec40f26a03ff9e8e9263adc5bf97e208a09f017cded3ada".into(),
            version: 1_058_323_180,
            object_digest: "71BhuNoCgNqNbThnHoEr4SaUSFZuvYkdsDGdXcmq4zVV".into(),
        }],
        recipients: vec![SOURCE.into()],
        amounts: vec![150_000_000],
        address_balance_amount: 100_000_000,
    };
    assert_matches_mainnet(
        WALLET_A,
        None,
        5_000_000,
        TransactionType::pay_sui(pay_sui),
        "AAADAgAA4fUFAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAAAiA0fAIAAAAAAAgO1v+VCfCQcePRdqf75mjaiD5A2WGcLDrpCSQHvduH4sEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACBGNvaW4McmVkZWVtX2Z1bmRzAQcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgNzdWkDU1VJAAEBAAADAAECAAACAAEBAQABAQMCAAAAAQIAg233ih6pqdnKjyrpL8Z5nHL2N0PWVbz7WvkPcMWZyqsBA5R3Zo7K4l8+xA8moD/56OkmOtxb+X4gignwF83tOtrsuhQ/AAAAACBZMjnawjkd4fRspwRimjCRr/8JYCiKOm0yNuhqYBZsMINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrZAAAAAAAAABAS0wAAAAAAAA=",
        "AINrrjFLufgc7SW5pkgRmzGuM8QSPbIT6tFyIsS8GnsMKCpImSIq6QiUazSguyiaIEgIiUYdRWSnR7vSgut4gQjGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "6qB8xFseugVfxPrTMAXoMxWG5Phearr4hZtEXZFSBJbU",
    );
}

#[test]
fn test_sui_mainnet_pay_all_sui_gas_coin_plus_withdrawal() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/4FhcHniYSdWcfwWDtEsWNKeo4DfsRAQtmwwA9DrzARuH
    let pay_all = Proto::PayAllSui {
        input_coins: vec![Proto::ObjectRef {
            object_id: "0x039477668ecae25f3ec40f26a03ff9e8e9263adc5bf97e208a09f017cded3ada".into(),
            version: 1_058_323_181,
            object_digest: "5nRWoSraS2u9F98r8LTXYBDpYHESEHW871Do7u4zTqNM".into(),
        }],
        recipient: SOURCE.into(),
        address_balance_amount: 50_000_000,
    };
    assert_matches_mainnet(
        WALLET_A,
        None,
        5_000_000,
        TransactionType::pay_all_sui(pay_all),
        "AAACAgCA8PoCAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQEAAAMAAQIAAAEBAAEBAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrAQOUd2aOyuJfPsQPJqA/+ejpJjrcW/l+IIoJ8BfN7Tra7boUPwAAAAAgRxGaGkX4d6DoF9SXFGIQL2ZWqlustJtgb5J7hgyNdxaDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAAA",
        "AIDZQyF4aYNoD8WVLBwk99XxQNtSIWd+jknsT79eRO8qqikPJ/B9q+5c7QRJhPPqnB+HrM9jexFOQINZpza/yATGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "4FhcHniYSdWcfwWDtEsWNKeo4DfsRAQtmwwA9DrzARuH",
    );
}

#[test]
fn test_sui_mainnet_add_stake_coin_plus_withdrawal() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/HX5kR5u1j1JBmtX4LkeJ8AABZ7zL9qDkPVspSuQChgZ5
    let stake = Proto::RequestAddStake {
        coins: vec![Proto::ObjectRef {
            object_id: "0x05adab7efdfc3413912cf73c27e03456df8b874531cf0e68654f0352b0c0d656".into(),
            version: 1_058_323_181,
            object_digest: "5yRySVDY9VShST3VjLqNArkbpL7ne1UWKRYQLeDdfUWv".into(),
        }],
        amount: Some(Proto::Amount {
            amount: 1_000_000_000,
        }),
        validator: CITADEL_VALIDATOR.into(),
        address_balance_amount: 300_000_000,
        ..Proto::RequestAddStake::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_289, 1_801_762_844)),
        5_000_000,
        TransactionType::request_add_stake(stake),
        "AAAFAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQEABa2rfv38NBORLPc8J+A0Vt+Lh0Uxzw5oZU8DUrDA1lbtuhQ/AAAAACBJ44KbL2YPScvF6TTRFm+DeKAS9ZVBMc5H1xU01BMxAwIAAKPhEQAAAAAABwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAAAJAQDKmjsAAAAAACBPl5HVxokwaGK065olkUxUM7ff1ctIJ7Rh99/IE/KKfAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBBwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAQECAAUAAgEBAAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwpzdWlfc3lzdGVtGnJlcXVlc3RfYWRkX3N0YWtlX211bF9jb2luAAQBAAACAQABAwABBACDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAACAQkFAAAAAAAAAQoFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZscuGRr",
        "AHT7BMtC5lmnX9R5GS90eBjvH31YRzsFTkmweklMgFB3ZOBdvqTxNX/XYULmvAN69+LufseFQW2/R9o6+/VdMQvGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "HX5kR5u1j1JBmtX4LkeJ8AABZ7zL9qDkPVspSuQChgZ5",
    );
}

#[test]
fn test_sui_mainnet_pay_token_coin_plus_withdrawal() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/B2kSBsF9RFBskBCYi4Kuth3ukbSjNdRGZi6h62oAcjVq
    let pay = Proto::Pay {
        input_coins: vec![Proto::ObjectRef {
            object_id: "0x16bd807e15e28bead930364e3a8cbe6dac3fdd247c282b20ac98412177f9ca0e".into(),
            version: 1_058_323_182,
            object_digest: "sdUjBTN1ffVPZv3GxTV82s46Cyq2DrhAM2vT7yHYcqG".into(),
        }],
        recipients: vec![SOURCE.into()],
        amounts: vec![200_000],
        coin_type: TOKEN.into(),
        address_balance_amount: 150_000,
        ..Proto::Pay::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_289, 4_161_091_766)),
        5_000_000,
        TransactionType::pay(pay),
        "AAAEAQAWvYB+FeKL6tkwNk46jL5trD/dJHwoKyCsmEEhd/nKDu66FD8AAAAAIAz4SUx41dtg7yUhR7LCtC1rDNj+pfgS2jSJi7NSK4szAgDwSQIAAAAAAAAH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAAACEANAwAAAAAAACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIEY29pbgxyZWRlZW1fZnVuZHMBB9ujRnLjDLBlsfk+OrVTGHaP1v72bBWULJ98uEbi+QDnBHVzZGMEVVNEQwABAQEAAwEAAAECAAACAQAAAQECAAEBAwIAAAABAwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAACAQkFAAAAAAAAAQoFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZu2OAX4",
        "ABHmCIZtIpr6Y2A/PrDvo157C3WsBRxeJG43Xmfbi88pSlN68EfuTHaLkbjdOu3hXKv2EmXOnNFldc8isS/VRAbGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "B2kSBsF9RFBskBCYi4Kuth3ukbSjNdRGZi6h62oAcjVq",
    );
}

#[test]
fn test_sui_mainnet_transfer_object_gas_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/DxYLYcUTPtvjgjXG3UUSNMLzEstqCPS1mVTPbUFJAa9W
    let transfer = Proto::TransferObject {
        object: Some(Proto::ObjectRef {
            object_id: "0x16bd807e15e28bead930364e3a8cbe6dac3fdd247c282b20ac98412177f9ca0e".into(),
            version: 1_058_323_183,
            object_digest: "C53XUGPKCAYZQGF99LoeSzJgShjzSxUxZNnCmKSuFBCC".into(),
        }),
        recipient: SOURCE.into(),
        ..Proto::TransferObject::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_289, 3_417_520_445)),
        5_000_000,
        TransactionType::transfer_object(transfer),
        "AAACACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwEAFr2AfhXii+rZMDZOOoy+baw/3SR8KCsgrJhBIXf5yg7vuhQ/AAAAACCkebwew+3Z25wXvbBVnR7gvVVMb/SnVfxTkQe9GsD3gQEBAQEBAAEAAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrZAAAAAAAAABAS0wAAAAAAAIBCQUAAAAAAAABCgUAAAAAAAAAACA1g0qKwXykj7FKyPmcF8mHR+ld0HKUrkGkazgiRqRJmz05s8s=",
        "AAn+z72BYMTJ8BQHe5+0LLnR9JuIzyOY/Xo3Pa8CVCD4jtWPsUgkskUcQH2iXgkc1l7EopMJsmaT1N525Z/IigTGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "DxYLYcUTPtvjgjXG3UUSNMLzEstqCPS1mVTPbUFJAa9W",
    );
}

#[test]
fn test_sui_mainnet_pay_sui_to_distinct_recipients_from_address_balance() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/Csuuizh2CZCZBH4XWUZLwd6BbWZaMCfRcu4arcf3xWqE
    // SOURCE appears twice (10 + 30 MIST-millions): it gets one coin of 40, B one coin of 20.
    let pay_sui = Proto::PaySui {
        recipients: vec![SOURCE.into(), WALLET_B.into(), SOURCE.into()],
        amounts: vec![10_000_000, 20_000_000, 30_000_000],
        address_balance_amount: 60_000_000,
        ..Proto::PaySui::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_289, 3_747_188_978)),
        5_000_000,
        TransactionType::pay_sui(pay_sui),
        "AAAEAgAAWmICAAAAAAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQAAACA7W/5UJ8JBx49F2p/vmaNqIPkDZYZwsOukJJAe924fiwIAAC0xAQAAAAAABwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACA3N1aQNTVUkAAAAgh1I7SGTJhYrqn292krqB+00GkqC/cYx2hprdOR9p7PoEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACBGNvaW4McmVkZWVtX2Z1bmRzAQcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgNzdWkDU1VJAAEBAAABAQIAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgRjb2luDHJlZGVlbV9mdW5kcwEHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIDc3VpA1NVSQABAQIAAQECAgABAwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAACAQkFAAAAAAAAAQoFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZvykFnf",
        "AOQfTW6IfCc5hlWjNnPNNiDyhsKHAz1GaUo0AnoAK/jGwYFT/u287hD8SJRqG2fnBcR/dNVznilmAhZgOuDXDgHGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "Csuuizh2CZCZBH4XWUZLwd6BbWZaMCfRcu4arcf3xWqE",
    );
}

#[test]
fn test_sui_mainnet_request_withdraw_stake_after_a_stake_funded_by_coin_plus_withdrawal() {
    // Unstakes the stake of `HX5kR5u1…`. Successfully broadcasted: https://suiscan.xyz/mainnet/tx/3QEpX3n11x6MetxmVVejPzzMJc31q1AHBMMqMWjvHP6W
    let unstake = Proto::RequestWithdrawStake {
        staked_sui: Some(Proto::ObjectRef {
            object_id: "0xfeda0cc05846973059c51f33530ba7ffaa03fb794ddfcc5257a87a33d4d922d9".into(),
            version: 1_059_082_490,
            object_digest: "Z7gWxKPsTHXPBDjgu6fie73FtATwPktT4Un6LHSFv5p".into(),
        }),
        ..Proto::RequestWithdrawStake::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_289, 811_073_433)),
        5_000_000,
        TransactionType::request_withdraw_stake(unstake),
        "AAACAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQEA/toMwFhGlzBZxR8zUwun/6oD+3lN38xSV6h6M9TZItn6UCA/AAAAACAIOiP2i9ARJYR65tG8/8YcuSiGBWbXNvFVTwwkp5+KiwEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMKc3VpX3N5c3RlbRZyZXF1ZXN0X3dpdGhkcmF3X3N0YWtlAAIBAAABAQCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKqwCDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAACAQkFAAAAAAAAAQoFAAAAAAAAAAAgNYNKisF8pI+xSsj5nBfJh0fpXdBylK5BpGs4IkakSZuZ/1cw",
        "APoeDQbbCa3lq24DiZKEz0Vak5y8pCUcRPv2ybuio4C1AB0cRqvAI2Or2COSLpUJDPhU5EjRQHzqYQAOX7cMKA/GMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "3QEpX3n11x6MetxmVVejPzzMJc31q1AHBMMqMWjvHP6W",
    );
}

#[test]
fn test_sui_mainnet_pay_token_withdrawal_with_a_separate_sui_gas_coin() {
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/FqxjtLe1CT2qrb2byeY7QSxudDctmaUSohwrm5epaZmP
    let pay = Proto::Pay {
        recipients: vec![SOURCE.into()],
        amounts: vec![10_000],
        gas: Some(Proto::ObjectRef {
            object_id: "0xa7d7f33ab25db267c68bdea246b4f6c6e29e22a2f04784e106758c43811f5e09".into(),
            version: 1_059_085_462,
            object_digest: "4epsMTs5Swjtio8KiDVEZQMc7uccMREFmuVoqoevsQ2U".into(),
        }),
        coin_type: TOKEN.into(),
        address_balance_amount: 10_000,
        ..Proto::Pay::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        None,
        5_000_000,
        TransactionType::pay(pay),
        "AAACAgAQJwAAAAAAAAAH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAAAIDtb/lQnwkHHj0Xan++Zo2og+QNlhnCw66QkkB73bh+LAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgRjb2luDHJlZGVlbV9mdW5kcwEH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAEBAAABAQIAAAEBAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrAafX8zqyXbJnxoveoka09sbiniKi8EeE4QZ1jEOBH14JllwgPwAAAAAgNkOs53W2uZW8ZwbkC/tw0bD5P6qxJEIR9VesqZNK7FGDbfeKHqmp2cqPKukvxnmccvY3Q9ZVvPta+Q9wxZnKq2QAAAAAAAAAQEtMAAAAAAAA",
        "ADsx2M6aukcWzUZSLRAgsBlfqG18d3yX6NFpT5FhD1qaeZKub6gMLC4rxXbCT8j5PO8/sVRmsASvR0qERrcqIwjGMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "FqxjtLe1CT2qrb2byeY7QSxudDctmaUSohwrm5epaZmP",
    );
}

#[test]
fn test_sui_mainnet_pay_token_to_distinct_recipients_from_address_balance() {
    // SOURCE appears twice (5000 + 7000): it gets one coin of 12000, B one coin of 6000.
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/7LtTy1D7kCTYotT9k3jh7NKwjYg5TXzbPm2S851MhaVd
    let pay = Proto::Pay {
        recipients: vec![SOURCE.into(), WALLET_B.into(), SOURCE.into()],
        amounts: vec![5_000, 6_000, 7_000],
        coin_type: TOKEN.into(),
        address_balance_amount: 18_000,
        ..Proto::Pay::default()
    };
    assert_matches_mainnet(
        WALLET_A,
        Some((1_289, 3517279276)),
        5_000_000,
        TransactionType::pay(pay),
        "AAAEAgDgLgAAAAAAAAAH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAAAIDtb/lQnwkHHj0Xan++Zo2og+QNlhnCw66QkkB73bh+LAgBwFwAAAAAAAAAH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAAAIIdSO0hkyYWK6p9vdpK6gftNBpKgv3GMdoaa3Tkfaez6BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgRjb2luDHJlZGVlbV9mdW5kcwEH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAEBAAABAQIAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgRjb2luDHJlZGVlbV9mdW5kcwEH26NGcuMMsGWx+T46tVMYdo/W/vZsFZQsn3y4RuL5AOcEdXNkYwRVU0RDAAEBAgABAQICAAEDAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrAINt94oeqanZyo8q6S/GeZxy9jdD1lW8+1r5D3DFmcqrZAAAAAAAAABAS0wAAAAAAAIBCQUAAAAAAAABCgUAAAAAAAAAACA1g0qKwXykj7FKyPmcF8mHR+ld0HKUrkGkazgiRqRJmyxspdE=",
        "ALXsVNILJgLreFbZncuMskuffHgvl2KCW2ePN//FtAtA/CSXkzQdd275UEo+WmiGJJmzOWdhIZPd6G8/iO391Q3GMsvIec57ahzKczjOO6K/enVleZuBPnMuZ2oPr6gHNQ==",
        "7LtTy1D7kCTYotT9k3jh7NKwjYg5TXzbPm2S851MhaVd",
    );
}
