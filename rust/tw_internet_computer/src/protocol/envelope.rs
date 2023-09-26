// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::{Serialize, Serializer};

use super::{principal::Principal, request_id::RequestId};

#[derive(Debug, Clone, Serialize)]
pub struct Envelope {
    pub content: EnvelopeContent,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sender_pubkey: Option<Vec<u8>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sender_sig: Option<Vec<u8>>,
}

/// The content of an IC ingress message, not including any signature information.
#[derive(Debug, Clone, Serialize)]
#[serde(tag = "request_type", rename_all = "snake_case")]
pub enum EnvelopeContent {
    /// A replicated call to a canister method, whether update or query.
    Call {
        /// A random series of bytes to uniquely identify this message.
        #[serde(
            default,
            skip_serializing_if = "Option::is_none",
            serialize_with = "serialize_nonce"
        )]
        nonce: Option<Vec<u8>>,
        /// A nanosecond timestamp after which this request is no longer valid.
        ingress_expiry: u64,
        /// The principal that is sending this request.
        sender: Principal,
        /// The ID of the canister to be called.
        canister_id: Principal,
        /// The name of the canister method to be called.
        method_name: String,
        /// The argument to pass to the canister method.
        #[serde(serialize_with = "serialize_arg")]
        arg: Vec<u8>,
    },
    /// A request for information from the [IC state tree](https://internetcomputer.org/docs/current/references/ic-interface-spec#state-tree).
    ReadState {
        /// A nanosecond timestamp after which this request is no longer valid.
        ingress_expiry: u64,
        /// The principal that is sending this request.
        sender: Principal,
        /// A list of paths within the state tree to fetch.
        paths: Vec<Vec<Label>>,
    },
}

fn serialize_arg<S>(arg: &[u8], s: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
    s.serialize_bytes(arg)
}

fn serialize_nonce<S>(nonce: &Option<Vec<u8>>, s: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
    match nonce {
        Some(nonce) => s.serialize_bytes(nonce),
        None => s.serialize_none(),
    }
}

#[derive(Debug, Clone)]
pub struct Label(Vec<u8>);

impl Label {
    #[inline]
    pub fn as_slice(&self) -> &[u8] {
        self.0.as_slice()
    }
}

impl From<&str> for Label {
    fn from(value: &str) -> Self {
        Label(value.as_bytes().to_vec())
    }
}

impl From<RequestId> for Label {
    fn from(value: RequestId) -> Self {
        Label(value.0.as_slice().to_vec())
    }
}

// Serialization
impl serde::Serialize for Label {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        serializer.serialize_bytes(self.0.as_slice())
    }
}
