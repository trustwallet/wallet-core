// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base64;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as TransactionType};

#[test]
fn test_sui_sign_direct_transfer() {
    let unsigned_tx = "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA";
    let direct = Proto::SignDirect {
        unsigned_tx_msg: unsigned_tx.into(),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::sign_direct_message(direct),
        private_key: "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"
            .decode_hex()
            .unwrap()
            .into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.unsigned_tx, unsigned_tx);
    // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
    assert_eq!(output.signature, "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}
