// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Pactus::Proto;

/// A macro to define three constants: `SIGN_BYTES`, `NOT_SIGNED_DATA`, and `SIGNED_DATA`.
/// Transaction format explained here: https://docs.pactus.org/protocol/transaction/format/
macro_rules! define_expected_data {
    (  $tx_id:expr,
       $signature:expr,
       $public_key:expr
       $(, $param:expr)*,
    ) => {
        pub const SIGN_BYTES: &str = concat!(
            $( $param, )*
        );

        pub const NOT_SIGNED_DATA: &str = concat!(
            "02", // Not Sign Flag
            $($param, )*
        );

        pub const SIGNED_DATA: &str = concat!(
            "00", // Signed Flag
            $($param, )*
            $signature,
            $public_key
        );

        pub const TX_ID: &str = $tx_id;
        pub const SIGNATURE: &str  = $signature;
    };
}

// Private key for all the test cases
pub const PRIVATE_KEY: &str = "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6";

// Successfully broadcasted transfer transaction:
// https://pacviewer.com/transaction/1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f
pub mod transfer_test_case {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    define_expected_data!(
        "1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f", // transaction ID
        "4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9b49d33a0fc\
         8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736693eda8506", // Signature
        "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa", // PublicKey
        "01",                                                               // Version
        "24a32300",                                                         // LockTime
        "80ade204",                                                         // Fee
        "0b77616c6c65742d636f7265",                                         // Memo
        "01",                                                               // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b",                       // Sender
        "037a14ae24533816e7faaa6ed28fcdde8e55a7df21",                       // Receiver
        "8084af5f",                                                         // Amount
    );

    pub fn sign_input() -> Proto::SigningInput<'static> {
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

// Successfully broadcasted bond transaction:
// https://pacviewer.com/transaction/d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f
pub mod bond_test_case {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    define_expected_data!(
        "d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f", // transaction ID
        "0d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda55b4338328adac7\
         9d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff65e39ce7006300", // Signature
        "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa", // PublicKey
        "01",                                                               // Version
        "c1b02300",                                                         // LockTime
        "80ade204",                                                         // Fee
        "0b77616c6c65742d636f7265",                                         // Memo
        "02",                                                               // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b",                       // Sender
        "0129288df0bf7bd4b5e9eeed8b932d0c76f451823d",                       // Receiver
        "60\
         98bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a3935\
         5cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a56bbc\
         dde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb13833b", // Validator Public key
        "8094ebdc03",                                                       // Stake
    );

    pub fn sign_input() -> Proto::SigningInput<'static> {
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

// Successfully broadcasted bond without public key transaction:
// https://pacviewer.com/transaction/f83f583a5c40adf93a90ea536a7e4b467d30ca4f308d5da52624d80c42adec80
//
// If the validator exists and has already been staked, there’s no need to send the public key.
// If the validator does not exist, the public key is required, as it is not indexed on the chain.
pub mod bond_without_public_key_test_case {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    define_expected_data!(
        "f83f583a5c40adf93a90ea536a7e4b467d30ca4f308d5da52624d80c42adec80", // transaction ID
        "9e6279fb64067c7d7316ac74630bbb8589df268aa4548f1c7d85c087a8748ff0\
         715b9149afbd94c5d8ee6b37c787ec63e963cbb38be513ebc436aa58f9a8f00d", // Signature
        "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa", // PublicKey
        "01",                                                               // Version
        "5ca32300",                                                         // LockTime
        "80ade204",                                                         // Fee
        "0b77616c6c65742d636f7265",                                         // Memo
        "02",                                                               // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b",                       // Sender
        "01d2fa2a7d560502199995ea260954f064d90278be",                       // Receiver
        "00",                                                               // Public key zero
        "8094ebdc03",                                                       // Stake
    );

    pub fn sign_input() -> Proto::SigningInput<'static> {
        let bond_payload = Proto::BondPayload {
            sender: "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr".into(),
            receiver: "pc1p6taz5l2kq5ppnxv4agnqj48svnvsy797xpe6wd".into(),
            stake: 1000000000,
            public_key: Default::default(),
        };

        let transaction = Proto::TransactionMessage {
            lock_time: 2335580,
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