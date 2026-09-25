// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Serialize;
use serde_repr::Serialize_repr;

// Code snippets from:
// https://github.com/MystenLabs/sui/blob/a16c942b72c13f42846b3c543b6622af85a5f634/crates/shared-crypto/src/intent.rs

/// This enums specifies the intent scope.
#[derive(Serialize_repr)]
#[repr(u8)]
pub enum IntentScope {
    /// Used for a user signature on a transaction data.
    TransactionData = 0,
    /// Used for a user signature on a personal message.
    PersonalMessage = 3,
}

/// The version here is to distinguish between signing different versions of the struct
/// or enum. Serialized output between two different versions of the same struct/enum
/// might accidentally (or maliciously on purpose) match.
#[derive(Serialize_repr)]
#[repr(u8)]
pub enum IntentVersion {
    V0 = 0,
}

/// This enums specifies the application ID. Two intents in two different applications
/// (i.e., Narwhal, Sui, Ethereum etc) should never collide, so that even when a signing
/// key is reused, nobody can take a signature designated for app_1 and present it as a
/// valid signature for an (any) intent in app_2.
#[derive(Serialize_repr)]
#[repr(u8)]
pub enum AppId {
    Sui = 0,
}

/// An intent is a compact struct serves as the domain separator for a message that a signature commits to.
/// It consists of three parts: [enum IntentScope] (what the type of the message is),
/// [enum IntentVersion], [enum AppId] (what application that the signature refers to).
/// It is used to construct [struct IntentMessage] that what a signature commits to.
///
/// The serialization of an Intent is a 3-byte array where each field is represented by a byte.
#[derive(Serialize)]
pub struct Intent {
    pub scope: IntentScope,
    pub version: IntentVersion,
    pub app_id: AppId,
}

impl Intent {
    pub fn sui_transaction() -> Self {
        Self {
            scope: IntentScope::TransactionData,
            version: IntentVersion::V0,
            app_id: AppId::Sui,
        }
    }

    pub fn personal_message() -> Self {
        Self {
            scope: IntentScope::PersonalMessage,
            version: IntentVersion::V0,
            app_id: AppId::Sui,
        }
    }
}

/// Intent Message is a wrapper around a message with its intent. The message can
/// be any type that implements [trait Serialize]. *ALL* signatures in Sui must commits
/// to the intent message, not the message itself. This guarantees any intent
/// message signed in the system cannot collide with another since they are domain
/// separated by intent.
///
/// The serialization of an IntentMessage is compact: it only appends three bytes
/// to the message itself.
#[derive(Serialize)]
pub struct IntentMessage<T> {
    pub intent: Intent,
    pub value: T,
}

impl<T> IntentMessage<T> {
    pub fn new(intent: Intent, value: T) -> Self {
        Self { intent, value }
    }
}

/// A person message that wraps around a byte array.
#[derive(Serialize)]
pub struct PersonalMessage {
    pub message: Vec<u8>,
}
