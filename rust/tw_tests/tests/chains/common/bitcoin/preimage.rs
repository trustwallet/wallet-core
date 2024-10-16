// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::PreImageHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_memory::Data;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto::SigningError;

struct ExpectedSighash {
    pubkey: Data,
    sighash: Data,
    taproot_tweak: Option<Data>,
    signing_method: Proto::mod_PreSigningOutput::SigningMethod,
}

pub struct BitcoinPreImageHelper<'a> {
    input: &'a Proto::SigningInput<'a>,
    coin_type: Option<CoinType>,
    expected_sighashes: Vec<ExpectedSighash>,
}

impl<'a> BitcoinPreImageHelper<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
        BitcoinPreImageHelper {
            input,
            coin_type: None,
            expected_sighashes: Vec::default(),
        }
    }

    pub fn coin(mut self, coin_type: CoinType) -> Self {
        self.coin_type = Some(coin_type);
        self
    }

    pub fn add_expected_legacy(mut self, pubkey: Data, sighash: Data) -> Self {
        self.expected_sighashes.push(ExpectedSighash {
            pubkey: pubkey.to_vec(),
            sighash: sighash.to_vec(),
            taproot_tweak: None,
            signing_method: Proto::mod_PreSigningOutput::SigningMethod::Legacy,
        });
        self
    }

    pub fn add_expected_segwit(mut self, pubkey: Data, sighash: Data) -> Self {
        self.expected_sighashes.push(ExpectedSighash {
            pubkey: pubkey.to_vec(),
            sighash: sighash.to_vec(),
            taproot_tweak: None,
            signing_method: Proto::mod_PreSigningOutput::SigningMethod::Segwit,
        });
        self
    }

    pub fn add_expected_taproot(
        mut self,
        pubkey: Data,
        sighash: Data,
        taproot_tweak: Option<Data>,
    ) -> Self {
        self.expected_sighashes.push(ExpectedSighash {
            pubkey: pubkey.to_vec(),
            sighash: sighash.to_vec(),
            taproot_tweak,
            signing_method: Proto::mod_PreSigningOutput::SigningMethod::Taproot,
        });
        self
    }
}

impl<'a> BitcoinPreImageHelper<'a> {
    pub fn pre_image(self) {
        let coin_type = self
            .coin_type
            .expect("'BitcoinPreImageHelper::coin_type' is not set");
        let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
        let preimage_output = pre_imager.pre_image_hashes(coin_type, self.input);

        assert_eq!(
            preimage_output.error,
            SigningError::OK,
            "{}",
            preimage_output.error_message
        );

        assert_eq!(
            preimage_output.sighashes.len(),
            self.expected_sighashes.len(),
            "Wrong number of sighashes",
        );

        for (i, (actual_sighash, expected_sighash)) in preimage_output
            .sighashes
            .into_iter()
            .zip(self.expected_sighashes)
            .enumerate()
        {
            assert_eq!(
                actual_sighash.public_key.to_hex(),
                expected_sighash.pubkey.to_hex(),
                "Wrong public key at {}",
                i
            );
            assert_eq!(
                actual_sighash.sighash.to_hex(),
                expected_sighash.sighash.to_hex(),
                "Wrong sighash at {}",
                i
            );
            let actual_merkle_tree = actual_sighash.tweak.map(|tweak| tweak.merkle_root.to_hex());
            let expected_merkle_tree = expected_sighash.taproot_tweak.map(|root| root.to_hex());
            assert_eq!(
                actual_merkle_tree, expected_merkle_tree,
                "Wrong taproot merkle tree at {}",
                i
            );
            assert_eq!(
                actual_sighash.signing_method, expected_sighash.signing_method,
                "Wrong signing method at {}",
                i
            );
        }
    }
}
