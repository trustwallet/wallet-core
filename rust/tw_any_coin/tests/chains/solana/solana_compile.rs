// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58::{self, Alphabet};
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto::{self, mod_SigningInput::OneOftransaction_type as TransactionType};

#[test]
fn test_solana_compile_transfer() {
    let transfer = Proto::Transfer {
        recipient: "3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe".into(),
        value: 1000,
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        sender: "sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH".into(),
        recent_blockhash: "TPJFTN4CjBn12HiBfAbGUhpD9zGvRSm2RcheFRA4Fyv".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "010001030d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c024c255a8bc3e8496217a2cd2a1894b9b9dcace04fcd9c0d599acdaaea40a1b61000000000000000000000000000000000000000000000000000000000000000006c25012cc11a599a45b3b2f7f8a7c65b0547fa0bb67170d7a0cd1eda4e2c9e501020200010c02000000e803000000000000"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature = "a8c610697087eaf8a34b3facbe06f8e9bb9603bb03270dad021ffcd2fc37b6e9efcdcb78b227401f000eb9231c67685240890962e44a17fd27fc2ff7b971df03".decode_hex().unwrap();
    let public_key = "0d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0"
        .decode_hex()
        .unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::Solana, &input, vec![signature], vec![public_key]);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "5bWxVCP5fuzkKSGby9hnsLranszQJR2evJGTfBrpDQ4rJceW1WxKNrWqVPBsN2QCAGmE6W7VaYkyWjv39HhGrr1Ne2QSUuHZdyyn7hK4pxzLPMgPG8fY1XvXdppWMaKMLmhriLkckzGKJMaE3pWBRFBKzigXY28714uUNndb7S9hVakxa59hrLph39CMgAkcj6b8KYvJEkb1YdYytHSZNGi4kVVTNqiicNgPdf1gmG6qz9zVtnqj9JtaD2efdS8qxsKnvNWSgb8XxbT6dwyp7msUUi7d27cYaPTpK");
    assert_eq!(output.unsigned_tx, "87PYr2vKPjNPfwNmqTvhgkThhohTqFNKYJgCHcrUCeayX6daQs9AFvMA698MG9TknbSnUxNXaNaReatkevLDgiTG5FqcBgVHG5PLPrq3PCdKPLjAN9RMQJXM5i6KaVMDzGJGMfgSFMS4ecEjqumZX4nux9rhG4jpYaQbe5sgyYUetwMmemoNiCgW2qCFsGnTYR9rWSU7S9zF");
}

#[test]
fn test_solana_compile_create_nonce_account() {
    let nonce_account = "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ";
    let sender = "sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH";

    let create_nonce_account = Proto::CreateNonceAccount {
        nonce_account: nonce_account.into(),
        rent: 10000000,
        ..Proto::CreateNonceAccount::default()
    };
    let input = Proto::SigningInput {
        sender: sender.into(),
        recent_blockhash: "mFmK2xFMhzJJaUN5cctfdCizE9dtgcSASSEDh1Yzmat".into(),
        transaction_type: TransactionType::create_nonce_account(create_nonce_account),
        tx_encoding: Proto::Encoding::Base64,
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "020003050d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c057f6ed937bb447a6700c9684d2e182b1a6661838a86cca7d0aac18be2e098b2106a7d517192c568ee08a845f73d29788cf035c3145b21ab344d8062ea940000006a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a0000000000000000000000000000000000000000000000000000000000000000000000000b563fd13b46e844f12f54fa8a0e78c44d95dbae4953368b7135f1e0de111cb502040200013400000000809698000000000050000000000000000000000000000000000000000000000000000000000000000000000000000000040301020324060000000d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let sender_signature = "83985a7cb1ba60a11c361a49476f10316d70032d5e2b4d882250d4de1a841f3dbaf9e68b036b067ec4f56c04032672229777ad101e9d3d51d6d917994b767e0e".decode_hex().unwrap();
    let nonce_signature = "24708efdba7e33259607597396b665baea3fc51d71e19a963cccad4bbdd2770e42e4831d1cc037afd02065a09e8c2b5e04f63da20f200e6aff689ad3b2bf7f0a".decode_hex().unwrap();
    let sender_pubkey = base58::decode(sender, Alphabet::BITCOIN).unwrap();
    let nonce_pubkey = base58::decode(nonce_account, Alphabet::BITCOIN).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Solana,
        &input,
        vec![sender_signature, nonce_signature],
        vec![sender_pubkey, nonce_pubkey],
    );

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "AoOYWnyxumChHDYaSUdvEDFtcAMtXitNiCJQ1N4ahB89uvnmiwNrBn7E9WwEAyZyIpd3rRAenT1R1tkXmUt2fg4kcI79un4zJZYHWXOWtmW66j/FHXHhmpY8zK1LvdJ3DkLkgx0cwDev0CBloJ6MK14E9j2iDyAOav9omtOyv38KAgADBQ0ESmLQpN/loDehW1n6TU0NOrgRA6LBCm2gik0FhhHAV/btk3u0R6ZwDJaE0uGCsaZmGDiobMp9CqwYvi4JiyEGp9UXGSxWjuCKhF9z0peIzwNcMUWyGrNE2AYuqUAAAAan1RcZLFxRIYzJTD1K8X9Y2u4Im6H9ROPb2YoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAALVj/RO0boRPEvVPqKDnjETZXbrklTNotxNfHg3hEctQIEAgABNAAAAACAlpgAAAAAAFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAwECAyQGAAAADQRKYtCk3+WgN6FbWfpNTQ06uBEDosEKbaCKTQWGEcA=");
    assert_eq!(output.unsigned_tx, "AgADBQ0ESmLQpN/loDehW1n6TU0NOrgRA6LBCm2gik0FhhHAV/btk3u0R6ZwDJaE0uGCsaZmGDiobMp9CqwYvi4JiyEGp9UXGSxWjuCKhF9z0peIzwNcMUWyGrNE2AYuqUAAAAan1RcZLFxRIYzJTD1K8X9Y2u4Im6H9ROPb2YoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAALVj/RO0boRPEvVPqKDnjETZXbrklTNotxNfHg3hEctQIEAgABNAAAAACAlpgAAAAAAFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAwECAyQGAAAADQRKYtCk3+WgN6FbWfpNTQ06uBEDosEKbaCKTQWGEcA=");
}
