// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::sui::object_ref;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as TransactionType};

pub(super) const PRIVATE_KEY_54E80D76: &str =
    "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff";
pub(super) const SENDER_54E80D76: &str =
    "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2";

/// Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
pub(super) mod transfer_d4ay9tdb {
    use super::*;

    pub const UNSIGNED_TX: &str = "AAAEAAjoAwAAAAAAAAAIUMMAAAAAAAAAIKcXWr3V7ZLr4605DbNmxqcGR4zfUXzebPmGMAZc2jd6ACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgMCAAIBAAABAQABAQMAAAAAAQIAAQEDAAABAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFeHEaBQAAAAAgGGuNnxrqusosgjP3gQ3jBjnhapGNBlcU0yTaupXpa0BU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwMYtAAAAAAAA";
    pub const SIGNATURE: &str = "AEh44B7iGArEHF1wOLAQJMLNgGnaIwn3gKPC92vtDJqITDETAM5z9plaxio1xomt6/cZReQ5FZaQsMC6l7E0BwmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==";
    pub const TX_HASH: &str = "4c171457873befef70077461909ae40ac67bdad476b832b9c09b589cd698578f";

    pub fn sui_transfer_input() -> Proto::SigningInput<'static> {
        let pay_sui = Proto::PaySui {
            input_coins: vec![object_ref(
                "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
                85619064,
                "2eKuWbZSVfpFVfg8FXY9wP6W5AFXnTchSoUdp7obyYZ5",
            )],
            recipients: vec![
                "0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a".into(),
                // Send some amount to self.
                SENDER_54E80D76.into(),
            ],
            amounts: vec![1000, 50_000],
        };

        Proto::SigningInput {
            transaction_payload: TransactionType::pay_sui(pay_sui),
            // 0.003 SUI
            gas_budget: 3000000,
            reference_gas_price: 750,
            ..Proto::SigningInput::default()
        }
    }
}
