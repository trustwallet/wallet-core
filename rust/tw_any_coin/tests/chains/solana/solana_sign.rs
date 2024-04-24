// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58::{self, Alphabet};
use tw_encoding::hex::DecodeHex;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto;

use tw_proto::Solana::Proto::mod_SigningInput::OneOftransaction_type as TransactionType;

fn b58(s: &str) -> Cow<'static, [u8]> {
    base58::decode(s, Alphabet::BITCOIN).unwrap().into()
}

#[test]
fn test_solana_sign_transfer() {
    let transfer = Proto::Transfer {
        recipient: "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd".into(),
        value: 42,
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM");
    assert_eq!(output.unsigned_tx, "87PYsiS4MUU1UqXrsDoCBmD5FcKsXhwEBD8hc4zbq78yePu7bLENmbnmjmVbsj4VvaxnZhy4bERndPFzjSRH5WpwKwMLSCKvn9eSDmPESNcdkqne2UdMfWiFoq8ZeQBnF9h98dP8GM9kfzWPjvLmhjwuwA1E2k5WCtfii7LKQ34v6AtmFQGZqgdKiNqygP7ZKusHWGT8ZkTZ");
}

#[test]
fn test_solana_sign_transfer_v0() {
    let transfer = Proto::Transfer {
        recipient: "6pEfiZjMycJY4VA2FtAbKgYvRwzXDpxY58Xp4b7FQCz9".into(),
        value: 5000,
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        private_key: "833a053c59e78138a3ed090459bc6743cca6a9cbc2809a7bf5dbc7939b8775c8"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "HxKwWFTHixCu8aw35J1uxAX6yUhLHkFCdJJdK4y98Gyj".into(),
        v0_msg: true,
        transaction_type: TransactionType::transfer_transaction(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://explorer.solana.com/tx/4ffBzXxLPYEEdCYpQGETkCTCCsH6iTdmKzwUZXZZgFemdhRpxQwboguFFoKCeGF3SsZPzuwwE7LbRwLgJbsyRqyP?cluster=testnet
    assert_eq!(output.encoded, "6NijVxwQoDjqt6A41HXCK9kXwNDp48uLgvRyE8uz6NY5dEzaEDLzjzuMnc5TGatHZZUXehKrzUGzbg9jPSdn6pVsMc9TXNH6JGe5RJLmHwWey3MC1p8Hs2zhjw5P439P57NToatraDX9ZwvBtK4EzZzRjWbyGdicheTPjeYKCzvPCLxDkTFtPCM9VZGGXSN2Bne92NLDvf6ntNm5pxsPkZGxPe4w9Eq26gkE83hZyrYXKaiDh8TbqbHatSkw");
}

#[test]
fn test_solana_sign_transfer_to_self() {
    let transfer = Proto::Transfer {
        recipient: "zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu".into(),
        value: 42,
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://explorer.solana.com/tx/4ffBzXxLPYEEdCYpQGETkCTCCsH6iTdmKzwUZXZZgFemdhRpxQwboguFFoKCeGF3SsZPzuwwE7LbRwLgJbsyRqyP?cluster=testnet
    assert_eq!(output.encoded, "EKUmihvvUPKVN4GSCFwZRtz8WiyAuPvthW69Smo19SCjcPLQ6T7EVZd1HU71WAoe1bfgmPNS5JhU7ZLA9XKG3qbZqeEFJ1xmRwW9ZKw8SKMAL6VRWxp87oLu7PSmf5b8R34vCaww3XLKtZkoP49a7TUK31DqPN5xJCceMB3BZJyaojQaKU8nUkzSGf89LY6abZXp9krKAebvc6bSMzTP8SHSvbmZbf3VtejmpQeN9X6e7WVDn6oDa2bGT");
}

#[test]
fn test_solana_sign_transfer_with_memo_and_references() {
    let transfer = Proto::Transfer {
        recipient: "71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd".into(),
        value: 10000000,
        memo: "HelloSolanaMemo".into(),
        references: vec![
            "CuieVDEDtLo7FypA9SbLM9saXFdb1dsshEkyErMqkRQq".into(),
            "tFpP7tZUt6zb7YZPpQ11kXNmsc5YzpMXmahGMvCHhqS".into(),
        ],
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://explorer.solana.com/tx/4ffBzXxLPYEEdCYpQGETkCTCCsH6iTdmKzwUZXZZgFemdhRpxQwboguFFoKCeGF3SsZPzuwwE7LbRwLgJbsyRqyP?cluster=testnet
    assert_eq!(output.encoded, "NfNH76sST3nJ4FmFGTZJBUpJou7DRuHM3YNprT1HeEau699CQF65xNf21Hoi491bbtVKUXfqCJyeZhfTCEnABuXNC1JrhGBeCv2AbQdaS9gpp9j4xHHomhCYdwYaBWFMcKkdMXrx9xHqL9Vkny4HezkwQfb3wGqcaE9XVRdkkNxsoJnVKddRnrQbjhsZGTcKdfmbTghoUeRECNPTm6nZTA1owWF1Dq6mfr6M3GZRh4ucqEquxKsQC2HQwNRrGZahsfyUvwspPWwMt78q5Jpjd9kHqkFDspZL6Pepv4dAA4uHhYDCHeP2bbDiFMBYxxWCVDDtRKSh3H92xUgh1GCSgNcjGdbVfQUhSDPX3k9xuuszPTsVZ2GnsavAsRp6Vf6fFEikBX6pVV9zjW1cx94EepQ2aGEBSsVu4RzX7rJjCLCq87h8cxxf1XnF8mvYGEK7wzF");
}

#[test]
fn test_solana_sign_transfer_with_durable_nonce() {
    let transfer = Proto::Transfer {
        recipient: "3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe".into(),
        value: 1000,
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        private_key: "044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "5ycoKxPRpW2GdD4byZuMptHU3VU5MgUCh6NLGQ2U8VE5".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        nonce_account: "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc".into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "6zRqmNP5waeyartbf8GuQrWxdSy4SCYBTEmGhiXfYNxQTuUrvrBjia18YoCM367AQZWZ5yTjcN6FaXuaPWju7aVZNFjyqpuMZLNEbpm8ZNmKP4Na2VzR59iAdSPEZGTPuesZEniNMAD7ZSux6fayxgwrEwMWjeiskFQEwdvFzKNHfNLbjoVpdSTxhKiqfbwxnFBpBxNE4nqMj3bUR37cYJAFoDFokxy23HGpV93V9mbGG89aLBNQnd9LKTjpYFv49VMd48mptUd7uyrRwZLMneew2Bxq3PLsj9SaJyCWbsnqYj6bBahhsErz67PJTJepx4BEhqRxHGUSbpeNiL7qyERri1GZsXhN8fgU3nPiYr7tMMxuLAoUFRMJ79HCex7vxhf7SapvcP");
}

/// Transfer SOL with `priority_fee_limit` and `priority_fee_price`.
#[test]
fn test_solana_sign_transfer_with_priority_fee() {
    // Corresponding Solana address: 7HP8cENhoDxS7yT6UHyT3mVwvN5rv6gMrzwRXXWFJvDU.
    let private_key = "1eb5264f82747294e7481ecae700bd1d21e50db620bcf0d2af7582f712c71b40"
        .decode_hex()
        .unwrap();

    let transfer = Proto::Transfer {
        recipient: "HwHDGHYeYfXdzwvCZu7zm21SzQ7ZLBGTLNkuscyFdpvZ".into(),
        value: 1001,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.into(),
        recent_blockhash: "ABdaGDKfoBe2SJAYkD31sojYVwTM8ainsdBD7ZM9dut1".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        priority_fee_limit: Some(Proto::PriorityFeeLimit { limit: 2000 }),
        priority_fee_price: Some(Proto::PriorityFeePrice { price: 2 }),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "4WReC2rmup9nZjtFQbBbha5uADkVb54CRd6a3vbAoptbuzYvdud2Xau2tkzCejZsfsUC2ppqhMqbXXzaoiSpYH8XEDo6QtUV8EGXcE3GFxVjjtBT2U8abtC2qcBMDGAghtzpsmKWfWMxmQj8wWKjpmzamiU6PjfMbuAyAX5VPhQxZQ84Ay2HtQJBK8LA9CJ9qva9ehmu7g41wKBXLPq22NGjeSHq6ubjcNLQrFx34sKGtPJ68Z2oZgrju7XjKFhtnKzqUCWcgVUjUsyHyyBJ3awaExYHEw2uKq9NVoKmf9JDkjf2LBjNpUJ3EGzFHrfwnVEQ7B5XCX4nQRPir7eLuunLwVKhrdATKDbxin3ANe5D");
}

/// Transfer SOL with `priority_fee_limit` set only.
#[test]
fn test_solana_sign_transfer_with_priority_fee_limit() {
    // Corresponding Solana address: 7HP8cENhoDxS7yT6UHyT3mVwvN5rv6gMrzwRXXWFJvDU.
    let private_key = "1eb5264f82747294e7481ecae700bd1d21e50db620bcf0d2af7582f712c71b40"
        .decode_hex()
        .unwrap();

    let transfer = Proto::Transfer {
        recipient: "HwHDGHYeYfXdzwvCZu7zm21SzQ7ZLBGTLNkuscyFdpvZ".into(),
        value: 160000,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.into(),
        recent_blockhash: "8mhXYg8vpezXr7y7eP5q3oztzY9FRwTPSvyB3izzF88m".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        priority_fee_limit: Some(Proto::PriorityFeeLimit { limit: 1000 }),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://solscan.io/tx/4ei1iejYd1Yr11hdi7ziFjc6GyUAtNnUQqURqssBXB7H6kYJ4CXxYiSks3PjoPdJfZFMXTSAYTp7wqYrjCKgKxcV
    assert_eq!(output.encoded, "23MBXQ2GgD4CWLXG5MrhmXa6YDf4ghZdxEAzdrG9U457PMBAeg3WbSy7WA7H7a8VEFZ5eYR8TSvSAJ4ZBqhS4bLT7e8VSTszLDu159RmeKcfXtn7Ae4hQybb6uALWSkdBjSHyURYZoto6HWzS57htjUQkmAimaWFG5nwfkox9wqxAZqDNVAFYEAzM3eNWwWeN2rCnNy4vLfyKSZeNTRdvPg8PjUFyXrWcKkd3NFTqSchQmZsccaxpN32sNjg2Br2thCrcazon3rBY9T1BwuaWTtZsvm653dhhKENNajaD3TyA79K1m4rP6wYVaEs6UWr6qcwsdR6WD5TCkwmmmEjVYeQp2Py");
}

#[test]
fn test_solana_sign_delegate_stake_no_stake_account() {
    let delegate = Proto::DelegateStake {
        validator_pubkey: "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC".into(),
        value: 42,
        ..Proto::DelegateStake::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::delegate_stake_transaction(delegate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "j24mVM9Zgu5vDZhPLGGuCRXQnP9djNtxdHh4txN3S7dwJsNNL5fbhzGpPgSUAcLGoMVCfF9TuqTYfpfJnb4sJFe1ahM8yPL5HwuKL6py5AZJFi8SWx9fvaVB699dCPo1GT3JoEBLPCZ9o2jQtnwzLkzTYJnKv2axqhKWFE2sz6TBA5J39eZcjMFUYgyxz6Q5S4MWqYQCb8UET2NAEZoKcfy7j8N25WXL6Gj4j3hBZjpHQQNaGaNEprEqyma3ZuVhpGiCALSsuzVLX3wZVo4icXwe952deMFA4tH3BK1jcSQCgfmcKDJ9nd7bdrnUUs4BoMdF1uDZB5LxE2UH8QiqtYvaUcorF4SJ3gPxM5ykbyPsNK1cSYZF9NMpW2GofyC17eELwnHQTQB2kqphxJZu7BahvkwiDPPeeydiXAkBspJ3nc3PCBujv6WJw22ZHw5j6zAP8ZGnCW44pqtWD5qifF9tTKhySKdANNiWifs3tSCCPQqjfJXu14drNinR6VG8rJxS1qgmRYiRQUa7m1vtoaZFRN5qKUeAfoFKkAVaNnMdwgsNqNH4dqBodTCJFs1LkYwhgRZdZGbwXTn1j7vpR3DSnv4g72i2H556srzK53jdUmdv6yfxt516XDSshqZtHnKZ1tudxKjBXwsqT3imDiZFVka9wKWUAYMCi4XZ79CY6Xpsd9c18U2e9TCngQmgkTATFgrqysfraokNffgqWxvsPMugksbvbPjJs3iCzByvphkC9p7hCf6LwbeF8XnVB91EAgRDA4VLE1f9wkcq5zjy879YWJ4r516h3PQszTz1EaJXNAXdbk5Em7eyuuabGP1Q3nijFTL2yhMDsXpgrjAuEAABNxFMd4J1JRMaic615mHrhwociksrsfQK");
}

#[test]
fn test_solana_sign_delegate_stake_with_priority_fee() {
    let delegate = Proto::DelegateStake {
        validator_pubkey: "BWkvytz3MAiLkUbMuYK5yV1VYThbBYYQYG3gdef8NLw5".into(),
        // 0.01
        value: 10000000,
        ..Proto::DelegateStake::default()
    };

    let input = Proto::SigningInput {
        // Corresponding Solana address: GEjr7dsDVHZMzTvnz1AgDW76LJ2GC7DATnHHMUVoF4p6.
        private_key: "43c0aeb43038c582b1abac52e5bdc2c20c83a1995fbfde5936ccd2e6392e9d8b"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "7uKCh5WRoLdib5LfcX4NETFcJroQPjHVsekdXniyaoFZ".into(),
        transaction_type: TransactionType::delegate_stake_transaction(delegate),
        priority_fee_price: Some(Proto::PriorityFeePrice { price: 1000 }),
        priority_fee_limit: Some(Proto::PriorityFeeLimit { limit: 10_000 }),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // // https://solscan.io/tx/2DRUt4Q3BxYPB1Whd4MyekByh4pXWgrfQAybZ3obeNReR96sKYX1pZgZV12NUkLXJRb1c6ffAcEx1Eu7kYmA6zqH
    assert_eq!(output.encoded, "XUMBKTDMfK4i8N3RBJtzzDsA3KnKZ4R4HexjXxiwsKr3j4DhENHcpUFWA41xwU94dE4av4hqP9Z5nvES9FU7GuECsDE5CuRagt5b1EEH2SPW8AjdeGbTs7JfT4nm3nd6gh2uJHXVMCjTYEhTKSKgLehgCa2JqwodiKEoPUdPwRoq7YoznzSBzvwvmSZrySC2eA8pp1PaBNDCG9rMAg1mDAzAu4VNmH7Nhh9bssGPcxgKZfoWmrhpvNcx6bDV4xBJBtvsYoGasRRSTXwF82VjA1L5aaQWJjAbBBZgsiws2Z5CqeWWdt6XF7sUHovAZihUdxXzJXPjRpEgX4HqHdriqTnPuaNwmVMEcNKzmZWJ543LTz4yUhyLRbYbN9hxWMtpiKBnjZe5KX783NPiuSHioyxAvmfkdKbDyKUf6a9G9HocHZMjKT3Yz1yeanHo69V2mAfKGZCPT7hRhRae6QoXu16QjHDHNBDLUtX8Db49vNJLdV3cYWPuyZ5eHLfDbw6r9jzCF34nmXJ4kFZHJHatp3wGKGbTtyeoxZnJVxLFmv7byoKVZ1hQdxGYg3q2jfjc2NsSj2Expmz6qLGZWMyVy6cVYjfqcQz9bwT89zC8823mAnFEPKRs8whZevCxifGiUfT6H99zZzNKfuJAhwJ14rP5VpZHCiDSuDRHzjydzwFxHDKamnAQXYuadEWJB89gJWA6wPoBQE1q4kRFTrYjU32mmnfwCs6Ji18uzNfrP2W6xPMs93vz2Pejpgzyru6anf7cYhKWVtou6bM9uNdVV5Qj5zr3eY4FqbNXXMz4WkiWmKQb5nUeyvp4e424WS1QUkwNYVW5vmehM2AuzMM9VEA5F2VMyrDihE74ZFA9yctaYZ4ovtH2TvSVgCGsS7ujqHoETVtDiyrjmgJvm68NtjcTioSKpAPgEvMciwxVwsUmYC6NGC6iCP2oVc2S6QCzixF");
}

#[test]
fn test_solana_sign_delegate_stake_no_stake_account_5zrqgk1() {
    // Corresponding Solana address: GcQQ1qx822KK14zyfTkMLQMLEZ4a9d88HnKepaA2XbmW.
    let private_key = "507479b94db463a33cdc413db98437b4aa8c74b6053aea936d1830c27027b1e9"
        .decode_hex()
        .unwrap();
    let delegate = Proto::DelegateStake {
        validator_pubkey: "BWkvytz3MAiLkUbMuYK5yV1VYThbBYYQYG3gdef8NLw5".into(),
        value: 10000000,
        ..Proto::DelegateStake::default()
    };
    let input = Proto::SigningInput {
        private_key: private_key.into(),
        recent_blockhash: "6ocSmEiuSHkN5yKDUaQrJHCzWCYXuFk3g3jB4rN1uDzv".into(),
        transaction_type: TransactionType::delegate_stake_transaction(delegate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted:
    // https://solscan.io/tx/5zRQgK1oc1oHLhsM68AM2pSUUqif6VMMnuUKjyZgUsdySaNRufvzoEu8D8SF99GThw8iZiwRbPhnsDePpnDKKmRq
    assert_eq!(output.encoded, "nuo7rnbTG4QLqpjYoPPPhDusRjpEVDyzos8YvdX67sDaw2XwfTJNm7eXcdxqjkgLqzFiHbD7kvm1i7f1caGjxZwTEzWqNkxK28YXJXU4S2NbQFa7HwSdamF38PLgKwNKmnUkh1quaZDFK3X5swL49QPxFhtSwFcFo9TphPsfeFjMbqqsLR8vjzLkePTrWKUSMmbwMG8U9TDEeWrsWbKaFeWsRQxzYP7ubDokPLVxGJopaEUobtUGyRA5K2A9H4dbuN7pZJSLwL926hGJQphjiXuJhD9PqY5TCRAq9PhyS6jtYP6ToQUiPfvigm5aHwb5rtEZKu5gM393G7LKfedMi263FVSnJyAb41i4xZXTMD3HdKiDBF5UomqCQEyPQhM4X8mYzPDPofRZfQjz2xS1fDesxY3BWMxwKQ6bgtazJDprBWWNJ7ULpHFEJ8wXtHAR3CfTWowQJ23zsKNUPbb6Ft4Lng3DJrRkMcTvvecLx9nnNqyuwjkqcBysWqJ4pJdvLC4QAnwoZzKAn8uzTKUSA85gSeE96RUmRVE4CgtPkZ7jYgmsSS5XgcC7g4XegHHJFQ3iMG4Uuxft635txXHAbkh6x9Ra9JnPFUq6Zdgc9UyCWrhhaKDvfmFgC2K7tykqHLXdDninmR2s7zmc1wsoGYeGvy2NbLTU3mRpyVwQPFaKPDPSisJYKYY4HnBQfG89wRLh4jkRsrH9snvxYwhgRLcsaMniwGNPpQEU4tLAkxHwco32rnxyFwycaPqEYsYpTR5vc5RUrnyy4nhEx1V79hqBb7NScxAbBsaBXvYpYitgdfX1h19uBr7A7uqZ3mqptKqA2Q3VjZthnLLGjJNVuR4BNJEa7VzPnR8SLCckjVycSHkbG26cUmB985K5");
}

#[test]
fn test_solana_sign_delegate_stake_with_account() {
    let delegate = Proto::DelegateStake {
        validator_pubkey: "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC".into(),
        value: 42,
        stake_account: "6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv".into(),
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::delegate_stake_transaction(delegate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "TKPiN35HzeD3zdwxDFvnkgoqud7CZsda15JkBwM4nDpr623rM7MZsH6QvMMyKpiz7MeRNTrfyHkRLQSBT9Tbg2mgTdfrbhhqeF3Suu5ECphqn8DFYPoMnFzeg5u9gaqevfjhuizzeo2YDJF8aVGy1pez8gMbp5vHz1SuvQUgfcvFctggUMwNiJorSmmp3N6TzQSd38CZrA8ZLhaJjuwDwVMjmj18rGTV1gkX19L7byTFrus2vNvPeUa2AawwUnFpYMPgvCKkHTrpnjvypjoLof9yMUFQ5M1S3Ntv53KJyXwXq6ejJnBDtisnDcdMDNSZp3VeKz6XCr8XVM5xNVh3LX12V4kc3ueqkokYJLP1JmuhA3nNZA1G5KTNno93HUoBkEa1x5h3haoCSgmQC97LoJbJM6B6C2NbaDj2J6iiTaVQdin4He4Jpj575WDhNTqsLjzFUHPUHQF1CRnuss8UpVyMsa4kdVqCDQGeh5DKbkikgcB8GKPBuC91DRxGEqgoygNsu5nnQy4o3YAJnBBK6HsKxpdjbYD8wCUdLw8muhjpEqeBTPShEaogm9zfehidiCcnxbeoX3gmW8oH9gpWoX7GrkJgF6Wn7iWohmrzqzAjoBz8hpeY5nkkhHrf9iswVGMpakdLGy3YxkGJVpsW8KJACwEKXGLq8SVLtXSUHG8EP16zfYHxKjkCSs8PkdFsA5esxsxppPTVZivuEPqJ5og55aNmugdNDrAFYWdcH1Q4rm7BXN6oHECdz2yY4HFVWh9u592oqozt2gQKu3vmhcNFzzQe1xgs6zKSv38kSGTnipd7Hx2VL3qNAR6XBRiwAi226qSTzxi6R82p7cMB7TMy6fk5AZ3sXDSXFNJ9S5SSU1V63ruw75QMtVio");
}

#[test]
fn test_solana_sign_deactivate_stake() {
    let deactivate = Proto::DeactivateStake {
        stake_account: "6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB".into(),
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::deactivate_stake_transaction(deactivate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "6x3fSstNz4GpPxmT5jHXwyD62uyJMKaPWeBDNNcwXZA9NJ3E7KavCXPNUd8ZYTX5VpkfHKGszkwzM6AdAp4giLD29jvWdNYjkV1Nvb42xFwGD6ryMPZzXkJijaRTrA7SvPTDSRU2haGVmorqkywAXLQUCw47NmBUfLTb5gDcKoBeaAsahckv1eCE746thJVTg2dQNvUTULKF6xckUg7kwFkcUuRe4HCcRgrKcNAUKLR2rEM3brVQkUyAaAtMMtc3gVDXxxpbtW5Fa9wGaEnh31FdRo4z5YBzAUaz7vcrvzF2j81KCPTVnYyTmeJzCzJafzCVCtw");
}

#[test]
fn test_solana_sign_deactivate_all_stake() {
    let deactivate_all = Proto::DeactivateAllStake {
        stake_accounts: vec![
            "CJQStmfyoHbosX1GfVn64yWrNJAo214q2aqxwS6FGh4k".into(),
            "6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB".into(),
        ],
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::deactivate_all_stake_transaction(deactivate_all),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "U9azMJWRfDhypoDeQLYWyBYFZCwRNZy8sbrVX9awKK84zNGbSQfYTTJ3ZyzjNUVbU5npbw2MsWfmZGHZRvpfN7G7o3sVePyFRXrmLxrGZzGycFv25Zff4zPxDarbsugbCBgzVGpgwu8x7MdkwBAVHVtNsgMcHgArEAjEmk7YEGpZ15rjo39bCRvmuprWLqSv2SK1RyTZPpTPXVevAbA4i9vvcY8eUbwW29SZCoyGaagLU5EBV9vckMjzGa7gq2yMR6rbq8tDdWaXapYs8RavU49WN94yg4wdE4fzYq8DjqXHq3MuUBLxeYDKJnvj84ioeM4eR1EwjBNrGyz5GHTRuhbNg1nc57SpKsSMVSZW5Ra3tUk84YZXYFHxzeQ9Tv4o");
}

#[test]
fn test_solana_sign_withdraw_stake() {
    let withdraw = Proto::WithdrawStake {
        stake_account: "6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB".into(),
        value: 42,
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::withdraw_transaction(withdraw),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "gxr4o1trVP8DGG8UC21AA964YqAPFA3rBCF9MwmBQpn5fDtcujM9wp1gzT466MxWGR8wMciS6dSL771q29eURrEEuvhJzRaFDGPLgVB3UL4gd4T2amPQkR4Dzq5drKEtPJRBR86KVVc2kjDsbWNpdL8S7pZqW3VUijAbm9TS8ezG8NExSCkhxExKhUjXWWguEL4qXra7s2JZfhtmvuJneWnEY3isUVfC9knWtGNwpNFvRvzbH2sgHzwtSsD7mkYrBJoazLCwT8r9yypxycHL41XcGtH425MA16kVSunvvBfzG9PzBTS65YJBs64tzttasCU9uEphkwgmfrmoEC8iKt8xD47Ra79RyXd95yURsaxvpb1tVAH8kMNtj8iV1Pfm");
}

#[test]
fn test_solana_sign_withdraw_all_stake() {
    let withdraw_all = Proto::WithdrawAllStake {
        stake_accounts: vec![
            Proto::StakeAccountValue {
                stake_account: "CJQStmfyoHbosX1GfVn64yWrNJAo214q2aqxwS6FGh4k".into(),
                value: 42,
            },
            Proto::StakeAccountValue {
                stake_account: "6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB".into(),
                value: 67,
            },
        ],
    };
    let input = Proto::SigningInput {
        private_key: b58("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::withdraw_all_transaction(withdraw_all),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "cvBNusjtHkR74EfWsvFPEe2Mydcr7eoLeY2wJw2ZMZYViotbb63Adai7UD1PW9uLusoVHGLeJC5cPgVBC4F693P9tPAxLs9yiZj1ZJQ4DgnYbeXafqzjdWje1Ly5FgpDUJaaU2RnLCG51CcrmiTJ4KB5fwai6egZaNjbiqo1DEC1wJz4FgKug2aKQWLdeCiH9WhCuvqfhNV6mEE4qRCkU8uS2gfSqBd1AdrczvoDEbKQszosrwmawxqmvTE5EWaFzMb48x9nLqxvpQCvGQu1nX6FxZJjv2swekA7wGLEAA4uSdFLTHNrYSi8pn8hVYGwESEzth9oiPkJCvW7Y2KvGALeERUZn8knHiz2eqaaT72Ajp9UogMdZtiuFHufveLXpBLWUERchhB7eU1magYcPNHcZuEE4uQv5kZJhHAqYCGU6dyUFLVA9Edus7o6fTktYVCjoGb");
}

#[test]
fn test_solana_sign_create_token_account() {
    let create_token_acc = Proto::CreateTokenAccount {
        main_address: "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V".into(),
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        token_address: "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP".into(),
    };
    let input = Proto::SigningInput {
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K".into(),
        transaction_type: TransactionType::create_token_account_transaction(create_token_acc),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVnH2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtCjAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzjB2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2TN3xXwu1");
}

#[test]
fn test_solana_sign_create_token_account_5ktpn1() {
    let create_token_acc = Proto::CreateTokenAccount {
        main_address: "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ".into(),
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        token_address: "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf".into(),
    };
    let input = Proto::SigningInput {
        private_key: "4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "HxaCmxrXgzkzXYvDFTToENtf9rVKk7cbiuSUqnqNheHq".into(),
        transaction_type: TransactionType::create_token_account_transaction(create_token_acc),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/5KtPn1LGuxhFiwjxErkxTb7XxtLVYUBe6Cn33ej7ATNVyorrkk3UAFJWDBUmzP8CZjmkocCxiMAdYnvrKoGpMsJx
    assert_eq!(output.encoded, "EoJGDRFZdnjmx7rgwYSuDGTMTUdxCBeh8RggrQDzGht9bwzLPpCWkCrN4iQJqg3R6JxP7z2QZuf7dGCZcjMVBmmisYE8waRsohcvygRwmGr6nefbaujR5avm2x3EUvoTGyy8cMZJxX7URx45qQJyCgqFLNFCQzD1Kej3xCEPAJqCdGZgmqkryw2E2nkpGKXgRmbyEg2rFgd5kpvjG6jSLLYzGomxVnaKK2XyMQbcedkTMYJ8Ara71iWPRFUziWfgivZcA1qsQp92Fpao3FSsRprhoQz9u1VyAnh8zEM9jCKiE5s4dwCknqCJYeYsbMLn1be2vNP9bMQfu1jjGSHmbb9WR3E2vakTUEUByASXqSAJZuXYE5scopEzB28rC8nrC31ArLMZng5wWym3QbqEv2Syd6RHoEeoXR6vA5LPqvJKyvtH82p4hc4XbD18128aNrFG3GTD2P");
}

#[test]
fn test_solana_sign_create_token_account_for_other_3e6ufv() {
    let create_token_acc = Proto::CreateTokenAccount {
        main_address: "3xJ3MoUVFPNFEHfWdtNFa8ajXUHsJPzXcBSWMKLd76ft".into(),
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        token_address: "67BrwFYt7qUnbAcYBVx7sQ4jeD2KWN1ohP6bMikmmQV3".into(),
    };
    let input = Proto::SigningInput {
        private_key: "4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "HmWyvrif3QfZJnDiRyrojmH9iLr7eMxxqiC9RJWFeunr".into(),
        transaction_type: TransactionType::create_token_account_transaction(create_token_acc),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/3E6UFVamHCm6Bgk8gXdZex7R7tJAVxqJm6t9ephAKu1PjcfZrD7CJqMwKu6RrvWSUESbZFqzdUyLXuxAFaawPHvJ
    assert_eq!(output.encoded, "4BsrHedHuForcKDhLdnLYDXgtQgQEj3EQVDtEhqa7o6ukFjW3shpTWv6PeKQdMp6af4ASjD4xQeZvXxLK5WUjguVMUf3xdJn7RnFeM7hdDJ56RDBM5PRJbRJVHjz6FJ7SVNTvr9y3gVYQtWx7NfKRxiyEAfq9JG7nqxSWaW6raMr9t35aVcdAVuXE9iXj3rzhVfCS69vVzy5KcFEK3mvDYG6L12V2CfviCydmeCvPw5r3zBUrZSQv7Ti4XFNBrPbk28gcqQwsBknBqasHxHqD9VUyPmBTuUyXq75QN8rhqN55NjxKBUw37tEUS1jKVpWnTeLFq1eRAMdXvjftNuQ5Bmm8Zc12PGWj9vdorBaYyvZXexJST5xNjR4SCkXvXZoRScETck95chv3VBn54jP8DpB4GGUmATFKSxpdtnNV64i1SQXW13KJwswthJvAaDiqevQLKLkvrTEAdb4BxEfPkFjDVti6P58rTZCMg5CTVLqdmWwpTSW5V");
}

#[test]
fn test_solana_sign_create_token_account_with_priority_fee_price() {
    // Corresponding Solana address: 7HP8cENhoDxS7yT6UHyT3mVwvN5rv6gMrzwRXXWFJvDU.
    let private_key = "1eb5264f82747294e7481ecae700bd1d21e50db620bcf0d2af7582f712c71b40"
        .decode_hex()
        .unwrap();

    let create_token_acc = Proto::CreateTokenAccount {
        main_address: "7HP8cENhoDxS7yT6UHyT3mVwvN5rv6gMrzwRXXWFJvDU".into(),
        // WBTC
        token_mint_address: "3NZ9JMVBmGAqocybic2c7LQCJScmgsAZ6vQqTDzcqmJh".into(),
        token_address: "94JfTH8qCeBQDBatvULREG43msjSQscT7oHnqx7jppX".into(),
    };

    let input = Proto::SigningInput {
        private_key: private_key.into(),
        recent_blockhash: "FgDiTexKSzxGudn6pupKZZRm6J2GFvM1vGf7hPH2WX6r".into(),
        transaction_type: TransactionType::create_token_account_transaction(create_token_acc),
        priority_fee_price: Some(Proto::PriorityFeePrice { price: 20 }),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://solscan.io/tx/5B2Zxu14wvr7CJJa4L3934cU9GEi82JHF1B9pDcHgtTeTPqBckyL12JSuZN224EAMCBGiCXEezT8fReQu4PVmHS
    assert_eq!(output.encoded, "BtCPMKt9QUhF9cLrawa51GPZyY3GicQpaf1iX73sswSQ4t6gNNcoKM2kVkXne3Xcyzo3fRbuBxh6PsDEcBSi5TviDq7YZsS8usLchrGwCM41U1KqjsyABZSnwiyKNf4bUg77B3SiUqs5gWrH7s6QXJ4LDZvUMu3W9Hqo7uHbBkGGBPw4Pqjys1D963LnbBrSRHBRKuYLJauePJcQ9RoCNhWgy63RfuQMXyMLdVanUtiBCwY9BWxyFiwJTLvz2XbyY8CRruxQFvmDcyCSghMGvim8AKxah7EHgBzn9PTqfdRqYcdT3quHf6mfKCpxv13DuwbYUHAhQe4acCKQsV2ty3w6QtVaL9gND1CYp3rYpDcRXg8KyXrARcvyRmBU2oBsxyHyvwqxWarre8RZJxMKEHytYDo2Wkx36zgm8JZcruUajrZi4Z2RQNNhsKYSqU9AGzV31opPpa8YLa9hRFy3YmhrDGtBv897GqDV9eD5FiDkQsDAzH9JcSibaffPe3aK3E46fh");
}

#[test]
fn test_solana_sign_token_transfer() {
    let token_transfer = Proto::TokenTransfer {
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        sender_token_address: "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP".into(),
        recipient_token_address: "3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei".into(),
        // 0.004
        amount: 4000,
        decimals: 6,
        ..Proto::TokenTransfer::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi".into(),
        transaction_type: TransactionType::token_transfer_transaction(token_transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg
    assert_eq!(output.encoded, "PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jRe2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvPiPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMqDV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnotcrH7pUa94xCVvCPPaomF");
}

#[test]
fn test_solana_sign_token_transfer_2pmvzp() {
    let token_transfer = Proto::TokenTransfer {
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        sender_token_address: "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP".into(),
        recipient_token_address: "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf".into(),
        // 0.0061
        amount: 6100,
        decimals: 6,
        ..Proto::TokenTransfer::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "zMEbroNLJ4vfDTdQyA72rk35c7nPo4K38efHLujbSuz".into(),
        transaction_type: TransactionType::token_transfer_transaction(token_transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/2pMvzparE16evNgNhiexBfj15eurQgqFJXemYkuGasWV8RfT5tQseadqXA2VXbgGZPM1MpLcGwfkKKqvYvrKTmnR
    assert_eq!(output.encoded, "LCtawaKHmvh9WEjYPFFMDQXsdKMQbVyK4Q3aRRfLCouqw6GE4p31PRPFoQqtazTziEj3ex3iLgnCspz1MN4SUE9d33g3HiiA6oCS6wGMvB2i3ojtmJzndCiLoDmuZgiuGouVSeS2MAEUoS3CRjdnbNKbRwgKn8YsDe1bZ57ueipfBLJfiE7xr8ji678uAv8FcMgo8Mq88SBGxVCUhjMS2VGQZhRUHHzDmvnzxhbbUzsLDfApzjHExkUm7ws3cQ2i1cSpQNCQWJd6rcDv1sYwDAavPS571Ny3CUq4cZxABh45Gj88LkRpzBMRdoebrh9hPy8ZRnu7PocBVjZytCgdF4CuhzdYNsmdcuU2WN5CEmv5zQ7pBrFdLZ8bBifP");
}

#[test]
fn test_solana_sign_create_and_transfer_token() {
    let create_transfer_token = Proto::CreateAndTransferToken {
        recipient_main_address: "71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd".into(),
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        recipient_token_address: "EF6L8yJT1SoRoDCkAZfSVmaweqMzfhxZiptKi7Tgj5XY".into(),
        sender_token_address: "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf".into(),
        // 0.0029
        amount: 2900,
        decimals: 6,
        ..Proto::CreateAndTransferToken::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c"),
        recent_blockhash: "DMmDdJP41M9mw8Z4586VSvxqGCrqPy5uciF6HsKUVDja".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/449VaYo48LrkMJF6XVKt9sJwVQN6Seqrmh9erDCLtiuj6BgFG3wpF5TwjNkxgJ7qzNa6NTj3TFsU3h9hKszfkA7w
    assert_eq!(output.encoded, "3Y2MVz2VVi7aEyC9q1awwdk1ModDBPHRSacKmTYnSgkmbbJeZ62Fub1bVPSHaTy4LUcQpzCQYhHAKtTKXUDYijEeLsMAUqPBEMAq1w8zCdqDpdXy6M4PuwNtYVV1WgqeiEsiMWpPp4BGWKfcziwFbmYueUGituacJq4wTnt92fho8mFi49XW64gEG4iNGScDtJkY7Geq8PKiLh1E9JMJoceiHxKbmxzCmmLTxEHdhySYHcDUSXnXWogZskeZNBMtR9dNjEMkCzEjrxRpBtJPtUNshciY45mDPNmw4j3xyLCBTRikyfFLc5g11r3UgyVD4YokoPRvrEXsgt6W3yjBshropBm6mY2eJYvfY2eZz4Yq8kLcUatCHVKtjcb1mP9Ww57KisJ9bRhipC8sodFaMYhZARMEa4a1u9eH4MyNUATRGNXarwQSBY46PWS3nKP6QBK7Dw7Ppp9MmYkdPcXKaLScbyLF3jKu6dHWMkHw3WdXSsM1wwXjXnWF9LxdwaEVcDmySWybj6aKD9QCWTU5kdncqJU56f7SYNRTN289WdUFGNDmSh56tj2v1");
}

#[test]
fn test_solana_sign_create_and_transfer_token_2() {
    let create_transfer_token = Proto::CreateAndTransferToken {
        recipient_main_address: "3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei".into(),
        token_mint_address: "4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU".into(),
        recipient_token_address: "BwTAyrCEdkjNyGSGVNSSGh6phn8KQaLN638Evj7PVQfJ".into(),
        sender_token_address: "5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf".into(),
        // 0.004
        amount: 4000,
        decimals: 6,
        ..Proto::CreateAndTransferToken::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "2qkvFTcMk9kPaHtd7idJ1gJc4zTkuYDUJsC67kXvHjv3zwEyUx92QyhhSeBjL6h3Zaisj2nvTWid2UD1N9hbg9Ty7vSHLc7mcFVvy3yJmN9tz99iLKsf15rEeKUk3crXWLtKZEpcXJurN7vrxKwjQJnVob2RjyxwVfho1oNZ72BHvqToRM1W2KbcYhxK4d9zB4QY5tR2dzgCHWqAjf9Yov3y9mPBYCQBtw2GewrVMDbg5TK81E9BaWer3BcEafc3NCnRfcFEp7ZUXsGAgJYx32uUoJPP8ByTqBsp2JWgHyZhoz1WUUYRqWKZthzotErVetjSik4h5GcXk9Nb6kzqEf4nKEZ22eyrP5dv3eZMuGUUpMYUT9uF16T72s4TTwqiWDPFkidD33tACx74JKGoDraHEvEeAPrv6iUmC675kMuAV4EtVspVc5SnKXgRWRxb4dcH3k7K4ckjSxYZwg8UhTXUgPxA936jBr2HeQuPLmNVn2muA1HfL2DnyrobUP9vHpbL3HHgM2fckeXy8LAcjnoE9TTaAKX32wo5xoMj9wJmmtcU6YbXN4KgZ");
}

#[test]
fn test_solana_sign_create_and_transfer_token_with_memo_and_references() {
    let create_transfer_token = Proto::CreateAndTransferToken {
        recipient_main_address: "71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd".into(),
        token_mint_address: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt".into(),
        recipient_token_address: "EF6L8yJT1SoRoDCkAZfSVmaweqMzfhxZiptKi7Tgj5XY".into(),
        sender_token_address: "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf".into(),
        // 0.0029
        amount: 2900,
        decimals: 6,
        memo: "HelloSolanaMemo370".into(),
        references: vec![
            "CuieVDEDtLo7FypA9SbLM9saXFdb1dsshEkyErMqkRQq".into(),
            "tFpP7tZUt6zb7YZPpQ11kXNmsc5YzpMXmahGMvCHhqS".into(),
        ],
    };
    let input = Proto::SigningInput {
        private_key: b58("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c"),
        recent_blockhash: "DMmDdJP41M9mw8Z4586VSvxqGCrqPy5uciF6HsKUVDja".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "FuUw2MoEGPATE38roXAw9mGQhCfdsdpVDdhuf5h8LKc8iWj2HzNS3SteXqyUoZtQ7L1ufLvu7cTMwNzxT8snnVimcknsA52CeN7bgMz1Ad1hRTAr77zE5efzAi8B124kaQ1cBEb6nFMr5Zq4wwDRoJgBaiUaM1U9ZY6GofCKHGMQN7ZNqEFG4fFvPaMXB59dFtiqrtApBGzvDho3nGshyQWZVWfMY44hvVk45FqiGrXuqUwkiJqeRaDhooZdXiFR9ubwJLXo3Ux23ZyijWKXYNsx1Lm5zMFEgRz3kXhzxzb8uzHVSrFYNieXXCQEv1GtErMKeQWuAHcwS3zxC6avTnTWJhTz3kVSXfSTYEg4MF2MBWeGrzKZ7id88ZfbpG4ZwzsDsdUCSMV6YYRNmx9P3B6oC4DL7cbi2g8hwtBdeKojY4G6JMPeg629V9sPyg2KKeYxD3cjhMKAYtrsJEbixep4LZENtdQxmgZFouJVvGy9MVhiTzGEFVwm4G25p5FhWhiS9HxHWVRXpUFHi2K9K2ttoo4Ug39V9f8s9cG1Xb5A4bHhGSuKLeCCBcrBqPWEsuLdVhjxsKJrRBJhyrZ6mpxtDhUWivZa6skmEawTts9rN2aP3dXW3cNch3s3LTXZWXG9QPUARJJPy5QAYsBoR8GunF5FFgHVuEHVpjXAd8ku9f7aoF8RNiMnXAqQHxiM3ug6HZpLHLX8aGoUbJ7vVAnEDLH");
}

#[test]
fn test_solana_sign_create_and_transfer_token_with_durable_nonce() {
    let create_transfer_token = Proto::CreateAndTransferToken {
        recipient_main_address: "3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe".into(),
        token_mint_address: "4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU".into(),
        recipient_token_address: "93hbN3brRjZqRQTT9Xx6rAHVDFZFWD9ragFDXvDbTEjr".into(),
        sender_token_address: "5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf".into(),
        // 0.004
        amount: 4000,
        decimals: 6,
        ..Proto::CreateAndTransferToken::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "AaYfEmGQpfJWypZ8MNmBHTep1dwCHVYDRHuZ3gVFiJpY".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        nonce_account: "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ".into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "388uZws6GfA9aiH1LPsYBijGBEfLEgqe6q5NWVYhsmjXjrgZB4cScGuvja6nBL3i6qg6HA4a8ptW6aHsNKVdcBWKhjZjaTPH5heEThzwEsMDfnH2PWAUbqfiFgMZQRCkhyCj57hGUR7hBFPELfz3DBw5qMz1tnP9gU6KTqHUomu5UaadLHb2v5mbgTRcsMm3yDp2tzMwrp53VqvFNmHSau4ot4kdNL1jqEJC68Fj4ku6fMQaFSPyAeLQRF45ofYsFCa65fmtb4gBpqWUdqWLv5Dy6xQUQUDsin8qpEVds6unXw5f63UjZeD7XQdC6Vz5aq3e6P9ug8L41xc1rbuRU3Kp4arUKyqTsHMQ2dxMhPwEJLkHd4mFqqUWpYFTdfLFaNGU22hEkvP1esHUzaaGDmzAozbS96oaFw2jbHRRJtL8VjoA1aokGFFThM6M6mExuy8GhUXdGjxDFU83Dan1URmHMGBRC4J9RMZip9s1sktJw9Rj5Std9KVT8T7m4MxTVTx4QoBw6KAf6PgNHyHPtZSc7kzoCxDYNo2Myxvy8D95zk9YMp1MxeZXTDQ2aJuhWvfHhhrwgcQasAxRzbnJ9oehebVUNEcZEFsfnCgYuUmxWUemoKZnE1bNMCvERVkT5fKQ36e1rt5vTC2iES9jzr3hDC1Pk1");
}

#[test]
fn test_solana_sign_create_nonce_account() {
    let create_nonce_account = Proto::CreateNonceAccount {
        nonce_account_private_key:
            "2a9737aca3cde2dc0b4f3ae3487e3a90000490cb39fbc979da32b974ff5d7490"
                .decode_hex()
                .unwrap()
                .into(),
        rent: 10000000,
        ..Proto::CreateNonceAccount::default()
    };
    let input = Proto::SigningInput {
        private_key: "044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "mFmK2xFMhzJJaUN5cctfdCizE9dtgcSASSEDh1Yzmat".into(),
        transaction_type: TransactionType::create_nonce_account(create_nonce_account),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "3wu6xJSbb2NysVgi7pdfMgwVBT1knAdeCr9NR8EktJLoByzM4s9SMto2PPmrnbRqPtHwnpAKxXkC4vqyWY2dRBgdGGCC1bep6qN5nSLVzpPYAWUSq5cd4gfYMAVriFYRRNHmYUnEq8vMn4vjiECmZoHrpabBj8HpXGqYBo87sbZa8ZPCxUcB71hxXiHWZHj2rovx2kr75Uuv1buWXyW6M8uR4UNvQcPPvzVbwBG82RjDYTuancMSAxmrVNR8GLBQNhrCCYrZyte3EWgEyMQxxfW8T3xNXqnbgdfvFJ3UjRBxXj3hrmv17xEivTjfs81aG2AAi24yiYrk8ep7eQqwDHVSArsrynnwVKVNUcCQCnSy7fuiuS7FweFX8DEN1K9BrfecHyWrF15fYzhkmWSs64aH6ZTYHWPv5znhFKYmAuopGwbsBEb2j5p8NS3iJZ2skb2wi47n1rpLZfoCHWKxNiikkDUJTGQNcSDrGUMfeW5aGubJrCfecPKEo9Wo9kd36iSsxYPYSWNKrz2HTooa1rCRhqjXD8dyX3bXGV8TK6W2sEgf4JkcDnNoWQLbindcP8XR");
}

#[test]
fn test_solana_sign_create_nonce_account_with_durable_nonce() {
    let create_nonce_account = Proto::CreateNonceAccount {
        nonce_account_private_key:
            "2a9737aca3cde2dc0b4f3ae3487e3a90000490cb39fbc979da32b974ff5d7490"
                .decode_hex()
                .unwrap()
                .into(),
        rent: 10000000,
        ..Proto::CreateNonceAccount::default()
    };
    let input = Proto::SigningInput {
        private_key: "044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "E6hvnoXU9QmfWaibMk9NuT6QRZdfzbs96WGc2hhttqXQ".into(),
        transaction_type: TransactionType::create_nonce_account(create_nonce_account),
        nonce_account: "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc".into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "Fr8FzXoH7h6Xo2La6SE49BEPzRX6f93Qn1cFA5E8n6z2GJtZdTU2BfyYGr1zv21Zkq7h68Z3Q96VnFyUVVd1hTWeq6tHDamF1JK5L23yEeUXpEWv1KziWvG9XbxfseHUyWETQck7SY2HbsT4KSjRX9suDaBh68Bu8c96CVN7KtgYPhUrKP62dAMHsf5qo7MESFN8wKJto94ANNCbQMzPmhig9nfiAfvfz9CqV4nbnSiqBGwo2XoyPknDK8RJ1UmA5ptfZ6w6Fy4UmJbQZWuZwpUrkEkfgLMNJ36McHkGAnjpyzq9gMtzb33xSjx1BqnbWXkKJdi8HyQAHTtvtqPz7DMsW9qx5fu3TNz6iC8YHG2HiynFCRjTtc2aH1rpJ9TLdFQEK8WrhdMFr3yW27cg6NB3JUFopUkDg2k5FwtzFyCdfifwebD7eswVNnqjoZxW59fHgY3BrBH8uNst8YAQWvRH77y5L6imVmFhezU5JUb5sF58gR1D8eAQhUcHueakZb5FkFCaMeioTpKrVGgcSNe9zkBMuquoUR3t4MVTiUSLa815qKoBCRmdexQDBt5RQbdQhYyVWn3ovjdhkwDGBU2zywRvottGCcEStQrUrSQDg1tMVKxX5G3sBtxYf");
}

#[test]
fn test_solana_sign_withdraw_nonce_account() {
    let withdraw_nonce = Proto::WithdrawNonceAccount {
        nonce_account: "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ".into(),
        recipient: "3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe".into(),
        value: 10000000,
    };
    let input = Proto::SigningInput {
        private_key: "044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "5ccb7sRth3CP8fghmarFycr6VQX3NcfyDJsMFtmdkdU8".into(),
        transaction_type: TransactionType::withdraw_nonce_account(withdraw_nonce),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "7gdEdDymvtfPfVgVvCTPzafmZc1Z8Zu4uXgJDLm8KGpLyPHysxFGjtFzimZDmGtNhQCh22Ygv3ZtPZmSbANbafikR3S1tvujatHW9gMo35jveq7TxwcGoNSqc7tnH85hkEZwnDryVaiKRvtCeH3dgFE9YqPHxiBuZT5eChHJvVNb9iTTdMsJXMusRtzeRV45CvrLKUvsAH7SSWHYW6bGow5TbEJie4buuz2rnbeVG5cxaZ6vyG2nJWHNuDPWZJTRi1MFEwHoxst3a5jQPv9UrG9rNZFCw4uZizVcG6HEqHWgQBu8gVpYpzFCX5SrhjGPZpbK3YmHhUEMEpJx3Fn7jX7Kt4t3hhhrieXppoqKNuqjeNVjfEf3Q8dJRfuVMLdXYbmitCVTPQzYKWBR6ERqWLYoAVqjoAS2pRUw1nrqi1HR");
}

#[test]
fn test_solana_sign_withdraw_nonce_account_to_self_with_durable_nonce() {
    let withdraw_nonce = Proto::WithdrawNonceAccount {
        nonce_account: "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ".into(),
        recipient: "sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH".into(),
        value: 10000000,
    };
    let input = Proto::SigningInput {
        private_key: "044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1"
            .decode_hex()
            .unwrap()
            .into(),
        recent_blockhash: "5EtRPR4sTWRSwNUE5a5SnKB46ZqTJH8vgF1qZFTKGHvw".into(),
        transaction_type: TransactionType::withdraw_nonce_account(withdraw_nonce),
        nonce_account: "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc".into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "3rxbwm6dSX4SbFa7yitVQnoUGWkmRuQtg3V13a2jEAPfZZACCXZX2UFgWFpPqE7KfZSYhd5QE9TLzyikCwcmSBHhKXjMp4oktQXwRT66YaCK8rJdNzBUuS1D9tgHMkLUWKAR7ZRyWd3XvtQhe7nWD6YF6TRGoKPSuwsZAArBxogA7YddmEUKPsr2qjSKbjg7X5BbNceFwjEFAiafuizdSt7eGJHB5m9zJeYct8LCanTwJwyEVu1T9HTsgjW9hqHehqhCiHP46KGo63o7WAoappZvM4EJZemu4GfM6F6H48bPXF2z1QJz17wE6BYeMXfXuGkCRt5jYxrjdKuqvTDYV34X1HjZYUdrkW6mQotWDY3bS6zyAt784Vwzk2uiA8ytmWMbC24coUVwPSPGwZ92WJ6BpVCCtGDxLzp4CkahRu78UNWzdcEwPG6AUf");
}

#[test]
fn test_solana_sign_advance_nonce_account() {
    let advance_nonce = Proto::AdvanceNonceAccount {
        nonce_account: "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ".into(),
    };
    let input = Proto::SigningInput {
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "4KQLRUfd7GEVXxAeDqwtuGTdwKd9zMfPycyAG3wJsdck".into(),
        transaction_type: TransactionType::advance_nonce_account(advance_nonce),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "7YPgNzjCnUd2zBb6ZC6bf1YaoLjhJPHixLUdTjqMjq1YdzADJCx2wsTTBFFrqDKSHXEL6ntRq8NVJTQMGzGH5AQRKwtKtutehxesmtzkZCPY9ADZ4ijFyveLmTt7kjZXX7ZWVoUmKAqiaYsPTex728uMBSRJpV4zRw2yKGdQRHTKy2QFEb9acwLjmrbEgoyzPCarxjPhw21QZnNcy8RiYJB2mzZ9nvhrD5d2jB5TtdiroQPgTSdKFzkNEd7hJUKpqUppjDFcNHGK73FE9pCP2dKxCLH8Wfaez8bLtopjmWun9cbikxo7LZsarYzMXvxwZmerRd1");
}

#[test]
fn test_solana_sign_token_transfer_with_external_fee_payer() {
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
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "H4gZ56AdmHfZj1F36oWrxDJMUJ8ph7XdTHtmsbtHZshG".into(),
        transaction_type: TransactionType::token_transfer_transaction(token_transfer),
        fee_payer_private_key: b58("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c"),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    assert_eq!(output.encoded, "ushDP6dNZWq32FASGqdnw7E8x14zFDAEZBViTyevUptV9yb4WSgpYzEGCxt9sXkrEtHVyww4F4GdcGZbPEaQTjAeyX5KGvHHDhWoPeFNnECzjUTuPj35dpF7zJ75Jx3ADfLQtUyzu5w7812fQvhwBwP8XDm3btqSG4VLWeQU5XuVqpg33Mq1L9zkGHQ8PZ4WkgNuSrC584EVnDcFE4rZsUtAv2jFTMjinQJB1qQEGTCHbjgdtJt8PzmXGXeczNyisPsEDrhZUw3g7RFYsgBDB1RFe1TxspzbWmxwr6CNPkGVsopmS6cbvSG9ejXY8xRYaswP7knAoPXwYk26yetoA824mzdv9vJ2RYpyK72EyCqFfFidm8MrJjFR49KwV3HRQKxZzYcvhYuJhR15GKBUWAQvYJTQQWArTi7pr7m84wNsV1mCUjrYsCVK47QtMAYvWU4toTxfgThngfF47awnpcSxfy8ggbwamq7qcaSH6cQVk1LPGo1iB5YxitSbaXP");
}

#[test]
fn test_solana_sign_create_and_transfer_token_with_external_fee_payer() {
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
        private_key: b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"),
        recent_blockhash: "EsnN3ksLV6RLBW7qqgrvm2diwVJNTzL9QCuzm6tqWsU8".into(),
        transaction_type: TransactionType::create_and_transfer_token_transaction(
            create_transfer_token,
        ),
        fee_payer_private_key: b58("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c"),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // https://explorer.solana.com/tx/7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2?cluster=devnet
    assert_eq!(output.encoded, "5sxFkQYd2FvqRU64N79A6xjJKNkgUsEEg2wKgai2NiK7A7hF3q5GYEbjQsYBG9S2MejwTENbwHzvypaa3D3cEkxvVTg19aJFWdCtXQiz42QF5fN2MuAb6eJR4KHFnzCtxxnYGtN9swZ5B5cMSPCffCRZeUTe3kooRmbTYPvSaemU6reVSM7X2beoFKPd2svrLFa8XnvhBwL9EiFWQ9WhHB2cDV7KozCnJAW9kdNDR4RbfFQxboANGo3ZGE5ddcZ6YdomATKze1TtHj2qzJEJRwxsRr3iM3iNFb4Eav5Q2n71KUriRf73mo44GQUPbQ2LvpZKf4V6M2PzxJwzBo7FiFZurPmsanT3U5efEsKnnueddbiLHedc8JXc1d3Z53sFxVGJpsGA8RR6thse9wUvaEWqXVtPbNA6NMao9DFGD6Dudza9pJXSobPc7mDHZmVmookf5vi6Lb9Y1Q4EgcEPQmbaDnKGGB6uGfZe629i3iKXRzAd2dB7mKfffhDadZ8S1eYGT3dhddV3ExRxcqDP9BAGQT3rkRw1JpeSSi7ziYMQ3vn4t3okdgQSq6rrpbPDUNG8tLSHFMAq3ydnh4Cb4ECKkYoz9SFAnXACUu4mWETxijuKMK9kHrTqPGk9weHTzobzCC8q8fcPWV3TcyUyMxsbVxh5q1p5h5tWfD9td5TZJ2HEUbTop2dA53ZF");
}

#[test]
fn test_solana_sign_raw_message_legacy() {
    let legacy = Proto::mod_RawMessage::MessageLegacy {
        header: Some(Proto::mod_RawMessage::MessageHeader {
            num_required_signatures: 1,
            num_readonly_signed_accounts: 0,
            num_readonly_unsigned_accounts: 1,
        }),
        account_keys: vec![
            "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q".into(),
            "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd".into(),
            "11111111111111111111111111111111".into(),
        ],
        recent_blockhash: "11111111111111111111111111111111".into(),
        instructions: vec![Proto::mod_RawMessage::Instruction {
            program_id: 2,
            accounts: vec![0, 1],
            program_data: "020000002a00000000000000".decode_hex().unwrap().into(),
        }],
    };

    let raw_message = Proto::RawMessage {
        signatures: Vec::default(),
        message: Proto::mod_RawMessage::OneOfmessage::legacy(legacy),
    };
    let input = Proto::SigningInput {
        private_key: b58("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"),
        raw_message: Some(raw_message),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM");
}

#[test]
fn test_solana_sign_raw_message_v0() {
    let v0 = Proto::mod_RawMessage::MessageV0 {
        header: Some(Proto::mod_RawMessage::MessageHeader {
            num_required_signatures: 1,
            num_readonly_signed_accounts: 0,
            num_readonly_unsigned_accounts: 1,
        }),
        account_keys: vec![
            "6pEfiZjMycJY4VA2FtAbKgYvRwzXDpxY58Xp4b7FQCz9".into(),
            "11111111111111111111111111111111".into(),
        ],
        recent_blockhash: "HxKwWFTHixCu8aw35J1uxAX6yUhLHkFCdJJdK4y98Gyj".into(),
        instructions: vec![Proto::mod_RawMessage::Instruction {
            program_id: 1,
            accounts: vec![0, 0],
            program_data: "020000008813000000000000".decode_hex().unwrap().into(),
        }],
        address_table_lookups: Vec::default(),
    };

    let raw_message = Proto::RawMessage {
        signatures: Vec::default(),
        message: Proto::mod_RawMessage::OneOfmessage::v0(v0),
    };
    let input = Proto::SigningInput {
        private_key: "833a053c59e78138a3ed090459bc6743cca6a9cbc2809a7bf5dbc7939b8775c8"
            .decode_hex()
            .unwrap()
            .into(),
        raw_message: Some(raw_message),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://explorer.solana.com/tx/4ffBzXxLPYEEdCYpQGETkCTCCsH6iTdmKzwUZXZZgFemdhRpxQwboguFFoKCeGF3SsZPzuwwE7LbRwLgJbsyRqyP?cluster=testnet
    assert_eq!(output.encoded, "6NijVxwQoDjqt6A41HXCK9kXwNDp48uLgvRyE8uz6NY5dEzaEDLzjzuMnc5TGatHZZUXehKrzUGzbg9jPSdn6pVsMc9TXNH6JGe5RJLmHwWey3MC1p8Hs2zhjw5P439P57NToatraDX9ZwvBtK4EzZzRjWbyGdicheTPjeYKCzvPCLxDkTFtPCM9VZGGXSN2Bne92NLDvf6ntNm5pxsPkZGxPe4w9Eq26gkE83hZyrYXKaiDh8TbqbHatSkw");
}
