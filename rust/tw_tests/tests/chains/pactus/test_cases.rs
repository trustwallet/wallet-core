// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Pactus::Proto;

pub const PRIVATE_KEY: &str = "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6";

pub mod transfer {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    pub const TRANSFER_SIGN_BYTES: &str = concat!(
    "01",                                         // Version
    "24a32300",                                   // LockTime
    "80ade204",                                       // Fee
    "0b77616c6c65742d636f7265",                                 // Memo
    "01",                                         // PayloadType
    "037098338e0b6808119dfd4457ab806b9c2059b89b", // Sender
    "037a14ae24533816e7faaa6ed28fcdde8e55a7df21", // Receiver
    "8084af5f"                                      // Amount
    );

    pub const TRANSACTION_TRANSFER_NOT_SIGNED_DATA: &str = concat!(
    "02",                                         // Flags
    "01",                                         // Version
    "24a32300",                                   // LockTime
    "80ade204",                                       // Fee
    "0b77616c6c65742d636f7265",                                 // Memo
    "01",                                         // PayloadType
    "037098338e0b6808119dfd4457ab806b9c2059b89b", // Sender
    "037a14ae24533816e7faaa6ed28fcdde8e55a7df21", // Receiver
    "8084af5f"                                      // Amount
    );

    pub const TRANSACTION_TRANSFER_SIGNED_DATA: &str = concat!(
    "00",                                                               // Flags
    "01",                                         // Version
    "24a32300",                                   // LockTime
    "80ade204",                                       // Fee
    "0b77616c6c65742d636f7265",                                 // Memo
    "01",                                         // PayloadType
    "037098338e0b6808119dfd4457ab806b9c2059b89b", // Sender
    "037a14ae24533816e7faaa6ed28fcdde8e55a7df21", // Receiver
    "8084af5f",                                      // Amount
    "4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9b49d33a0fc", // Signature
    "8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736693eda8506",
    "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa" // PublicKey
    );

    pub const TRANSFER_TX_ID: &str = "1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f";

    pub const TRANSFER_SIGNATURE: &str = "4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9b49d33a0fc8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736693eda8506";

    pub fn pactus_sign_transfer_input() -> Proto::SigningInput<'static> {
        let transfer_payload = Proto::TransferPayload {
            sender: "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr".into(),
            receiver: "pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra".into(),
            amount: 200000000,
        };

        let transaction = Proto::TransactionMessage {
            lock_time: 2335524,
            fee: 10000000,
            memo: "wallet-core".into(),
            payload: Proto::mod_TransactionMessage::OneOfpayload::transfer(transfer_payload),
        };

        let private_key_bytes = PRIVATE_KEY.decode_hex().unwrap();

        Proto::SigningInput {
            transaction: Some(transaction),
            private_key: private_key_bytes.into(),
        }
    }
}

pub mod bond {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    pub const BOND_SIGN_BYTES: &str = concat!(
    "01",                                                                // Version
    "23b0c100",                                                          // LockTime
    "80ade204",                                                          // Fee
    "0b77616c6c65742d636f7265",                                          // Memo
    "02",                                                                // PayloadType
    "037098338e0b6808119dfd4457ab806b9c2059b89b",                        // Sender
    "0129288df0bf7bd4b5e9eeed8b932d0c76f451823d",                        // Receiver
    "6098bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a393", // Public key
    "55cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a56bbc",
    "dde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb13833b",
    "8094ebdc03"                                                         // Stake
    );

    pub const TRANSACTION_BOND_NOT_SIGNED_DATA: &str = concat!(
    "02",                                                                // Flags
    "01",                                                                // Version
    "23b0c100",                                                          // LockTime
    "80ade204",                                                          // Fee
    "0b77616c6c65742d636f7265",                                          // Memo
    "02",                                                                // PayloadType
    "037098338e0b6808119dfd4457ab806b9c2059b89b",                        // Sender
    "0129288df0bf7bd4b5e9eeed8b932d0c76f451823d",                        // Receiver
    "6098bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a393", // Public key
    "55cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a56bbc",
    "dde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb13833b",
    "8094ebdc03"                                                         // Stake
    );

    pub const TRANSACTION_BOND_SIGNED_DATA: &str = concat!(
    "00",                                                               // Flags
    "01",                                                               // Version
    "23b0c100",                                                         // LockTime
    "80ade204",                                                         // Fee
    "0b77616c6c65742d636f7265",                                         // Memo
    "02",                                                               // PayloadType
    "037098338e0b6808119dfd4457ab806b9c2059b89b",                       // Sender
    "0129288df0bf7bd4b5e9eeed8b932d0c76f451823d",                       // Receiver
    "6098bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a393", // Public key
    "55cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a56bbc",
    "dde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb13833b",
    "8094ebdc03"                                                        // Stake
    "0d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda55b4338328adac7", // Signature
    "9d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff65e39ce7006300"
    );

    pub const BOND_TX_ID: &str = "d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f";

    pub const BOND_SIGNATURE: &str = "0d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda55b4338328adac79d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff65e39ce7006300";

    pub fn pactus_sign_bond_input() -> Proto::SigningInput<'static> {
        let bond_payload = Proto::BondPayload {
            sender: "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr".into(),
            receiver: "pc1p9y5gmu9l002tt60wak9extgvwm69rq3a9ackrl".into(),
            stake: 1000000000,
            public_key: "public1pnz75msstqdrq5eguvcwanug0zauhqjw2cc4flmez3qethnp68y64ehc4k69amapj7x4na2uda0snqz4yxujgx3jsse4f64fgy7jkh0xauvhrc5ts09vfk48g85t0js66hvajm6xruemsvlxqv3xvkyur8v9v0mtn".into()
        };

        let transaction = Proto::TransactionMessage {
            lock_time: 2339009,
            fee: 10000000,
            memo: "wallet-core".into(),
            payload: Proto::mod_TransactionMessage::OneOfpayload::bond(bond_payload),
        };

        let private_key_bytes = PRIVATE_KEY.decode_hex().unwrap();

        Proto::SigningInput {
            transaction: Some(transaction),
            private_key: private_key_bytes.into(),
        }
    }
}