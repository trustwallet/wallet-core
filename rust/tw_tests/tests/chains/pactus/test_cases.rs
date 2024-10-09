// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Pactus::Proto;

pub mod transfer_1 {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    /// private key to sign the transaction.
    pub const PRIVATE_KEY: &str =
        "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6";

    pub const SIGN_BYTES: &str = concat!(
        "01",                                         // Version
        "01020300",                                   // LockTime
        "e807",                                       // Fee
        "0474657374",                                 // Memo
        "01",                                         // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b", // Sender
        "037a14ae24533816e7faaa6ed28fcdde8e55a7df21", // Receiver
        "a09c01"                                      // Amount
    );

    pub const TRANSACTION_NOT_SIGNED_DATA: &str = concat!(
        "02",                                         // Flags
        "01",                                         // Version
        "01020300",                                   // LockTime
        "e807",                                       // Fee
        "0474657374",                                 // Memo
        "01",                                         // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b", // Sender
        "037a14ae24533816e7faaa6ed28fcdde8e55a7df21", // Receiver
        "a09c01",                                     // Amount
    );

    pub const TRANSACTION_SIGNED_DATA: &str = concat!(
        "00",                                                               // Flags
        "01",                                                               // Version
        "01020300",                                                         // LockTime
        "e807",                                                             // Fee
        "0474657374",                                                       // Memo
        "01",                                                               // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b",                       // Sender
        "037a14ae24533816e7faaa6ed28fcdde8e55a7df21",                       // Receiver
        "a09c01",                                                           // Amount
        "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47ff", // Signature
        "f027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09",
        "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa" // PublicKey
    );

    pub const TX_ID: &str = "34cd4656a98f7eb996e83efdc384cefbe3a9c52dca79a99245b4eacc0b0b4311";

    pub const SIGNATURE: &str = "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09";

    pub fn pactus_sign_transfer_input() -> Proto::SigningInput<'static> {
        let transfer_payload = Proto::TransferPayload {
            receiver: "pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra".into(),
            amount: 20000,
        };

        let transaction = Proto::TransactionMessage {
            lock_time: 0x00030201,
            fee: 1000,
            memo: "test".into(),
            payload: Proto::mod_TransactionMessage::OneOfpayload::transfer(transfer_payload),
        };

        let private_key_bytes = PRIVATE_KEY.decode_hex().unwrap();

        Proto::SigningInput {
            transaction: Some(transaction),
            private_key: private_key_bytes.into(),
        }
    }
}
