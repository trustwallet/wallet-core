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

fn b58(s: &'static str) -> Cow<'static, [u8]> {
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
