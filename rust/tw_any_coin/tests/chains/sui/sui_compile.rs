// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::sui::test_cases::{transfer_d4ay9tdb, PRIVATE_KEY_54E80D76};
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_misc::traits::ToBytesVec;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_sui_compile() {
    let input = transfer_d4ay9tdb::sui_transfer_input();

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Sui, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data_hash.to_hex(),
        transfer_d4ay9tdb::TX_HASH
    );

    // Step 3: Compile transaction info

    // Simulate external signing.
    let key_pair = ed25519::sha512::KeyPair::try_from(PRIVATE_KEY_54E80D76).unwrap();
    let public_key_bytes = key_pair.public().to_vec();
    let signature_bytes = key_pair
        .sign(preimage_output.data_hash.to_vec())
        .unwrap()
        .to_vec();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Sui,
        &input,
        vec![signature_bytes],
        vec![public_key_bytes],
    );

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
    assert_eq!(output.unsigned_tx, transfer_d4ay9tdb::UNSIGNED_TX);
    assert_eq!(output.signature, transfer_d4ay9tdb::SIGNATURE);
}
