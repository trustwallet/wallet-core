// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::{coin_dispatcher, utxo_dispatcher};
use tw_encoding::hex::ToHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto::SigningError;
use tw_proto::{deserialize, serialize};

pub struct Expected {
    /// Hex encoded PSBT.
    pub psbt: &'static str,
    pub encoded: &'static str,
    pub txid: &'static str,
    pub vsize: u64,
    pub weight: u64,
    pub fee: i64,
}

pub struct BitcoinPsbtSignHelper<'a> {
    input: &'a Proto::PsbtSigningInput<'a>,
    coin_type: Option<CoinType>,
}

impl<'a> BitcoinPsbtSignHelper<'a> {
    pub fn new(input: &'a Proto::PsbtSigningInput<'a>) -> Self {
        BitcoinPsbtSignHelper {
            input,
            coin_type: None,
        }
    }

    pub fn coin(mut self, coin_type: CoinType) -> Self {
        self.coin_type = Some(coin_type);
        self
    }

    #[track_caller]
    pub fn sign_psbt(self, expected: Expected) {
        let coin_type = self
            .coin_type
            .expect("'BitcoinSignHelper::coin_type' is not set");

        let input = serialize(self.input).unwrap();

        // TODO call `tw_bitcoin_sign_psbt` when all tests are moved to another crate.
        let (ctx, _entry) = coin_dispatcher(coin_type).expect("Unknown CoinType");
        let output_bytes = utxo_dispatcher(coin_type)
            .expect("CoinType is not UTXO, i.e `utxo_dispatcher` failed")
            .sign_psbt(&ctx, &input)
            .unwrap();

        let output: Proto::PsbtSigningOutput = deserialize(&output_bytes).unwrap();

        assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
        assert_eq!(output.psbt.to_hex(), expected.psbt, "Wrong PSBT hex");
        assert_eq!(
            output.encoded.to_hex(),
            expected.encoded,
            "Wrong encoded signed transaction"
        );
        assert_eq!(output.txid.to_hex(), expected.txid, "Wrong txid");
        assert_eq!(output.vsize, expected.vsize, "Wrong vsize");
        assert_eq!(output.weight, expected.weight, "Wrong weight");
        assert_eq!(output.fee, expected.fee, "Wrong fee");
    }
}
