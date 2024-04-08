// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_encoding::hex::DecodeHex;
use tw_encoding::{base64, bcs};
use tw_sui::address::SuiAddress;
use tw_sui::transaction::command::{Argument, Command};
use tw_sui::transaction::programmable_transaction::ProgrammableTransaction;
use tw_sui::transaction::sui_types::{
    CallArg, GasData, ObjectDigest, ObjectID, SequenceNumber, TransactionExpiration,
};
use tw_sui::transaction::transaction_data::{TransactionData, TransactionDataV1, TransactionKind};

#[test]
fn test_decode_transfer_tx() {
    let programmable = ProgrammableTransaction {
        inputs: vec![
            CallArg::Pure("1027000000000000".decode_hex().unwrap()),
            CallArg::Pure(
                "259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015"
                    .decode_hex()
                    .unwrap(),
            ),
        ],
        commands: vec![
            Command::SplitCoins(Argument::GasCoin, vec![Argument::Input(0)]),
            Command::TransferObjects(vec![Argument::NestedResult(0, 0)], Argument::Input(1)),
        ],
    };

    let v1 = TransactionDataV1 {
        kind: TransactionKind::ProgrammableTransaction(programmable),
        sender: SuiAddress::from_str(
            "0xd575ad7f18e948462a5cf698f564ef394a752a71fec62493af8a055c012c0d50",
        )
        .unwrap(),
        gas_data: GasData {
            payment: vec![(
                ObjectID(
                    SuiAddress::from_str(
                        "0x06f2c2c8c1d8964df1019d6616e9705719bebabd931da2755cb948ceb7e68964",
                    )
                    .unwrap()
                    .into_inner(),
                ),
                SequenceNumber(748),
                ObjectDigest::from_str("7UoYeVzREVT17ZyYbRTsKzRCec5xJWm6FMh8AKaDPdDx").unwrap(),
            )],
            owner: SuiAddress::from_str(
                "0xd575ad7f18e948462a5cf698f564ef394a752a71fec62493af8a055c012c0d50",
            )
            .unwrap(),
            price: 1,
            budget: 2000,
        },
        expiration: TransactionExpiration::None,
    };
    let data = TransactionData::V1(v1);

    let is_url = false;
    let bytes = base64::encode(&bcs::encode(&data).unwrap(), is_url);
    // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
    assert_eq!(bytes, "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA");
}
