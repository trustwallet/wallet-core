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
    let sender_pubkey = base58::decode(sender, Alphabet::Bitcoin).unwrap();
    let nonce_pubkey = base58::decode(nonce_account, Alphabet::Bitcoin).unwrap();

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

#[test]
fn test_solana_compile_withdraw_nonce_account() {
    let sender = "sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH";

    let withdraw_nonce = Proto::WithdrawNonceAccount {
        nonce_account: "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ".into(),
        recipient: "3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe".into(),
        value: 10000000,
    };
    let input = Proto::SigningInput {
        sender: sender.into(),
        recent_blockhash: "5ccb7sRth3CP8fghmarFycr6VQX3NcfyDJsMFtmdkdU8".into(),
        transaction_type: TransactionType::withdraw_nonce_account(withdraw_nonce),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "010003060d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c057f6ed937bb447a6700c9684d2e182b1a6661838a86cca7d0aac18be2e098b2124c255a8bc3e8496217a2cd2a1894b9b9dcace04fcd9c0d599acdaaea40a1b6106a7d517192c568ee08a845f73d29788cf035c3145b21ab344d8062ea940000006a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a000000000000000000000000000000000000000000000000000000000000000000000000448e50d73f42e3163f5926922aadd2bca6bdd91f97b3eb7b750e2cecfd810f6d01050501020304000c050000008096980000000000"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature = "1212e67f30fc9f17a440ef446c3e8638676b8253d3dc4517e297a777c011f820509c5d102e5648827e2a6a4b6274943c9209f44d2dee31277eaa6c54afd0410c".decode_hex().unwrap();
    let pubkey = base58::decode(sender, Alphabet::Bitcoin).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::Solana, &input, vec![signature], vec![pubkey]);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "7gdEdDymvtfPfVgVvCTPzafmZc1Z8Zu4uXgJDLm8KGpLyPHysxFGjtFzimZDmGtNhQCh22Ygv3ZtPZmSbANbafikR3S1tvujatHW9gMo35jveq7TxwcGoNSqc7tnH85hkEZwnDryVaiKRvtCeH3dgFE9YqPHxiBuZT5eChHJvVNb9iTTdMsJXMusRtzeRV45CvrLKUvsAH7SSWHYW6bGow5TbEJie4buuz2rnbeVG5cxaZ6vyG2nJWHNuDPWZJTRi1MFEwHoxst3a5jQPv9UrG9rNZFCw4uZizVcG6HEqHWgQBu8gVpYpzFCX5SrhjGPZpbK3YmHhUEMEpJx3Fn7jX7Kt4t3hhhrieXppoqKNuqjeNVjfEf3Q8dJRfuVMLdXYbmitCVTPQzYKWBR6ERqWLYoAVqjoAS2pRUw1nrqi1HR");
}

#[test]
fn test_solana_compile_create_and_transfer_token() {
    let sender = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V";

    let create_transfer_token = Proto::CreateAndTransferToken {
        recipient_main_address: "3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei".into(),
        token_mint_address: "4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU".into(),
        recipient_token_address: "BwTAyrCEdkjNyGSGVNSSGh6phn8KQaLN638Evj7PVQfJ".into(),
        sender_token_address: "5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf".into(),
        amount: 4000,
        decimals: 6,
        ..Proto::CreateAndTransferToken::default()
    };
    let input = Proto::SigningInput {
        sender: sender.into(),
        recent_blockhash: "AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "0100060994c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685aa287d36838b4ef65c2c460d1a52498453c259cd6a35ca91064aaead28187ca69485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551b5fd4eacd82d65e302202544558bb05c2698f88852a7925c5c0ee5ea8711ddb3fe1262150283eee811633b442cb3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea7000000000000000000000000000000000000000000000000000000000000000006ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857eff00a906a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a000000008c97258f4e2489f1bb3d1029148e0d830b5a1399daff1084048e7bd8dbe9f8598fb6d19edbbae20ebbc767fba1da4d4b40a4b97479fe526a82325cba7cee506802080700010304050607000604020401000a0ca00f00000000000006"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature = "28d04f30288e51c257648b8c955c661228d1e13632ee2fa366d3932e359a0d6044c66074e89d8542d2a60675679bd28a1b6eb80c273ed60a8f1cfe4fdc736b02".decode_hex().unwrap();
    let pubkey = base58::decode(sender, Alphabet::Bitcoin).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::Solana, &input, vec![signature], vec![pubkey]);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "2qkvFTcMk9kPaHtd7idJ1gJc4zTkuYDUJsC67kXvHjv3zwEyUx92QyhhSeBjL6h3Zaisj2nvTWid2UD1N9hbg9Ty7vSHLc7mcFVvy3yJmN9tz99iLKsf15rEeKUk3crXWLtKZEpcXJurN7vrxKwjQJnVob2RjyxwVfho1oNZ72BHvqToRM1W2KbcYhxK4d9zB4QY5tR2dzgCHWqAjf9Yov3y9mPBYCQBtw2GewrVMDbg5TK81E9BaWer3BcEafc3NCnRfcFEp7ZUXsGAgJYx32uUoJPP8ByTqBsp2JWgHyZhoz1WUUYRqWKZthzotErVetjSik4h5GcXk9Nb6kzqEf4nKEZ22eyrP5dv3eZMuGUUpMYUT9uF16T72s4TTwqiWDPFkidD33tACx74JKGoDraHEvEeAPrv6iUmC675kMuAV4EtVspVc5SnKXgRWRxb4dcH3k7K4ckjSxYZwg8UhTXUgPxA936jBr2HeQuPLmNVn2muA1HfL2DnyrobUP9vHpbL3HHgM2fckeXy8LAcjnoE9TTaAKX32wo5xoMj9wJmmtcU6YbXN4KgZ");
}

#[test]
fn test_solana_compile_advance_nonce_account() {
    let sender = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V";

    let advance_nonce = Proto::AdvanceNonceAccount {
        nonce_account: "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ".into(),
    };
    let input = Proto::SigningInput {
        sender: sender.into(),
        recent_blockhash: "4KQLRUfd7GEVXxAeDqwtuGTdwKd9zMfPycyAG3wJsdck".into(),
        transaction_type: TransactionType::advance_nonce_account(advance_nonce),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "0100020494c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a57f6ed937bb447a6700c9684d2e182b1a6661838a86cca7d0aac18be2e098b2106a7d517192c568ee08a845f73d29788cf035c3145b21ab344d8062ea940000000000000000000000000000000000000000000000000000000000000000000003149e670959884ea98bb33bca21c9505f1fc17b1d51ca59555a5d58c93f0f9c90103030102000404000000"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature = "54772ff2be7cce175e58dde0152437382ca71f932b62497ccac624f202bbf8fb8247a8c80ba7dd5412d8316c8ab25caf48739cc677c15b97cbbbc005b3a56006".decode_hex().unwrap();
    let pubkey = base58::decode(sender, Alphabet::Bitcoin).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::Solana, &input, vec![signature], vec![pubkey]);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "7YPgNzjCnUd2zBb6ZC6bf1YaoLjhJPHixLUdTjqMjq1YdzADJCx2wsTTBFFrqDKSHXEL6ntRq8NVJTQMGzGH5AQRKwtKtutehxesmtzkZCPY9ADZ4ijFyveLmTt7kjZXX7ZWVoUmKAqiaYsPTex728uMBSRJpV4zRw2yKGdQRHTKy2QFEb9acwLjmrbEgoyzPCarxjPhw21QZnNcy8RiYJB2mzZ9nvhrD5d2jB5TtdiroQPgTSdKFzkNEd7hJUKpqUppjDFcNHGK73FE9pCP2dKxCLH8Wfaez8bLtopjmWun9cbikxo7LZsarYzMXvxwZmerRd1");
}

#[test]
fn test_solana_compile_create_and_transfer_token_with_external_fee_payer() {
    let sender = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V";
    let fee_payer = "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ";

    let create_transfer_token = Proto::CreateAndTransferToken {
        recipient_main_address: "E54BymfdhXQtzDSGtgiJayauEMdB2gJjPqoDjzfbCXwj".into(),
        token_mint_address: "4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU".into(),
        recipient_token_address: "Ay7G7Yb7ZCebCQdG39FxD1nFNDtqFWJCBgfd5Ek7DDcS".into(),
        sender_token_address: "5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf".into(),
        // 0.004
        amount: 4000,
        decimals: 6,
        ..Proto::CreateAndTransferToken::default()
    };
    let input = Proto::SigningInput {
        sender: sender.into(),
        recent_blockhash: "EsnN3ksLV6RLBW7qqgrvm2diwVJNTzL9QCuzm6tqWsU8".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        fee_payer: fee_payer.into(),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "0200060acb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b5794c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a9418c9576a9c00c6bd8fc223f471573f7172488de10aa84dbf63c53a20bae717485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551b5fd4eacd82d65e30220c231dc02f482980f7d9915c1ecf53374091d38c060b49487f9c5d932e077ed763b442cb3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea7000000000000000000000000000000000000000000000000000000000000000006ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857eff00a906a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a000000008c97258f4e2489f1bb3d1029148e0d830b5a1399daff1084048e7bd8dbe9f859ce2a4331bce3670e6ea8bedff5908c6d91f833a31a7fdeac16978c261a1801d502090700020405060708000704030502010a0ca00f00000000000006"
    );
    let actual_signers: Vec<_> = preimage_output
        .signers
        .iter()
        .map(|signer| String::from_utf8(signer.to_vec()).unwrap())
        .collect();
    assert_eq!(
        actual_signers,
        vec![fee_payer.to_string(), sender.to_string()]
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature1 = "0567ccb81d1c52d87e6491d26a7f9e93f346a69a6f0318ab2f1d8fba210d0b034343730c7ed5d3a96a37e2eb4e97cb813eec9b1fab82b538f9cf7dd71383cd03".decode_hex().unwrap();
    let signature2 = "8aef8b94f8448e583e51b54c4935b1911bb98ebc3053a46859865451584b11e83fe1a50734afc933b973142b73a9b4892509798b2a3a6dcd851c0809704d4b0e".decode_hex().unwrap();
    let fee_payer_pubkey = base58::decode(fee_payer, Alphabet::Bitcoin).unwrap();
    let sender_pubkey = base58::decode(sender, Alphabet::Bitcoin).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Solana,
        &input,
        vec![signature1, signature2],
        vec![fee_payer_pubkey, sender_pubkey],
    );

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "5sxFkQYd2FvqRU64N79A6xjJKNkgUsEEg2wKgai2NiK7A7hF3q5GYEbjQsYBG9S2MejwTENbwHzvypaa3D3cEkxvVTg19aJFWdCtXQiz42QF5fN2MuAb6eJR4KHFnzCtxxnYGtN9swZ5B5cMSPCffCRZeUTe3kooRmbTYPvSaemU6reVSM7X2beoFKPd2svrLFa8XnvhBwL9EiFWQ9WhHB2cDV7KozCnJAW9kdNDR4RbfFQxboANGo3ZGE5ddcZ6YdomATKze1TtHj2qzJEJRwxsRr3iM3iNFb4Eav5Q2n71KUriRf73mo44GQUPbQ2LvpZKf4V6M2PzxJwzBo7FiFZurPmsanT3U5efEsKnnueddbiLHedc8JXc1d3Z53sFxVGJpsGA8RR6thse9wUvaEWqXVtPbNA6NMao9DFGD6Dudza9pJXSobPc7mDHZmVmookf5vi6Lb9Y1Q4EgcEPQmbaDnKGGB6uGfZe629i3iKXRzAd2dB7mKfffhDadZ8S1eYGT3dhddV3ExRxcqDP9BAGQT3rkRw1JpeSSi7ziYMQ3vn4t3okdgQSq6rrpbPDUNG8tLSHFMAq3ydnh4Cb4ECKkYoz9SFAnXACUu4mWETxijuKMK9kHrTqPGk9weHTzobzCC8q8fcPWV3TcyUyMxsbVxh5q1p5h5tWfD9td5TZJ2HEUbTop2dA53ZF");
}

#[test]
fn test_solana_compile_token_transfer_with_external_fee_payer() {
    let sender = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V";
    let fee_payer = "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ";

    let token_transfer = Proto::TokenTransfer {
        token_mint_address: "4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU".into(),
        sender_token_address: "5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf".into(),
        recipient_token_address: "AZapcpAZtEL1gQuC87F2L1hSfAZnAvNy1hHtJ8DJzySN".into(),
        // 0.004
        amount: 4000,
        decimals: 6,
        ..Proto::TokenTransfer::default()
    };
    let input = Proto::SigningInput {
        sender: sender.into(),
        recent_blockhash: "GwB5uixiTQUG2Pvo6fWAaNQmz41Jt4WMEPD7b83wvHkX".into(),
        transaction_type: TransactionType::token_transfer_transaction(token_transfer),
        fee_payer: fee_payer.into(),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "02000206cb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b5794c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551b5fd4eacd82d65e302208e12027e9261a6a276b5ff00ddecfda567ff3ae510a5b47045086ad1d50cab573b442cb3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea706ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857eff00a9ecc01200a43c87ad04ab8b382c0934a54e585e7dcd9cdef6d1cacd52718981c4010504020403010a0ca00f00000000000006"
    );
    let actual_signers: Vec<_> = preimage_output
        .signers
        .iter()
        .map(|signer| String::from_utf8(signer.to_vec()).unwrap())
        .collect();
    assert_eq!(
        actual_signers,
        vec![fee_payer.to_string(), sender.to_string()]
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature1 = "42eb5f9c2203aaaa1f58c38161da982fd5a855562fe75f12808fdec3bb742f3a688cf806f55588b4c533cd7e19bacc468497a8f19ad86fe06fe4dbb67a526c08".decode_hex().unwrap();
    let signature2 = "54eac822f984d4a23620546059890652d0540d7a73e6ca450d34ae046e7d0fd25d9bd6fa041a82f5573469196b2c46088997b6de97a8d20e28ba46b07333ec02".decode_hex().unwrap();
    let fee_payer_pubkey = base58::decode(fee_payer, Alphabet::Bitcoin).unwrap();
    let sender_pubkey = base58::decode(sender, Alphabet::Bitcoin).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Solana,
        &input,
        vec![signature1, signature2],
        vec![fee_payer_pubkey, sender_pubkey],
    );

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "qjgNVBmoPDHNTN2ENQfxNVE57jWXpqdmu5GQX4msA7iK8ZRAnKpvbusQagv8CZGyNYti23p9jBsjTSx75ZU26UW5vgC8D88pusW8W5dp1ERo5DSfurMSYJ6afgQHdcuzn7exb8znSm6uV4y1cWgBRcuAGdg3wRpVhP8HEB1EeKgzjYVWvMSy6yR7qVrSL6BxHG6eiAMyahLFbEt4qBqLEdxxY7Dt4DyydVYmG2ZVtheaMHD3ACwCjpyPLXj399wxSgGXQQFGtzEJQw9awVezmJ4wZk6W4dDpXQvdKYaqUvwTwRZsQB5o2iekPWZXR9xvHiMLjMVBPzYgcU14ZSaCbqSNVv2pAJxP1sMvxZMNMzZPttPxCsDDGq9biC7exXwzesXSnZ3rsgEYeZtkUiBHAxR4rYqBpA6VzLs1bPx8MPTvr9mhNi2ezMBbg2nEfHV6Fz7H7rEY2g3jDtRz35Vmgits8s9RKi3kb73WtGUieRiXjiqkNhpvKkST1oEYRQ9");
}

#[test]
fn test_solana_compile_transfer_with_zero_signature() {
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
    let public_key = "0d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0"
        .decode_hex()
        .unwrap();
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();

    // Compile with zero signature
    let signature = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        .decode_hex()
        .unwrap();
    let output = compiler.compile(CoinType::Solana, &input, vec![signature], vec![public_key]);
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "3md7BBV9wFjYGnMWcMNyAZcjca2HGfXWZkrU8vvho66z2sJMZFcx6HZdBiAddjo2kzgBv3uZoac3domBRjJJSXkbBvokxPZ8Lfa4BipQ5HdcJ2Fij2h7NHywnKFjDrgqfZ3YfUTEFreBy3MebYxQvgCaVhxGkxDwYc3Pvsoz4yQNMXTDrmgaqbhJzhV5WUXmqJeFMTyY8NguMLS6A9uP4iWe9RnMs2X5dFnhMPQzfCzL6wjVKJ99gRaqqDundDhdoYUdvgC17jY4pB9tMhL9bNBs9VwWu66dSYzQX");
    assert_eq!(output.unsigned_tx, "87PYr2vKPjNPfwNmqTvhgkThhohTqFNKYJgCHcrUCeayX6daQs9AFvMA698MG9TknbSnUxNXaNaReatkevLDgiTG5FqcBgVHG5PLPrq3PCdKPLjAN9RMQJXM5i6KaVMDzGJGMfgSFMS4ecEjqumZX4nux9rhG4jpYaQbe5sgyYUetwMmemoNiCgW2qCFsGnTYR9rWSU7S9zF");
}

#[test]
fn test_solana_compile_transfer_with_fake_signature() {
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
    let public_key = "0d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0"
        .decode_hex()
        .unwrap();
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();

    // Compile with other fake signature
    let signature = "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        .decode_hex()
        .unwrap();
    let output = compiler.compile(CoinType::Solana, &input, vec![signature], vec![public_key]);
    assert_eq!(output.error, SigningError::Error_signing);
}
