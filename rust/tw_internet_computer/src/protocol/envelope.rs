// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::collections::BTreeMap;

use serde::{Serialize, Serializer};

use super::{
    principal::Principal,
    request_id::{hash_of_map, RawHttpRequestVal, RequestId},
};

pub trait RepresentationHashable {
    fn request_id(&self) -> RequestId;
}

#[derive(Debug, Clone, Serialize)]
pub struct Envelope<C: RepresentationHashable> {
    pub content: C,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sender_pubkey: Option<Vec<u8>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sender_sig: Option<Vec<u8>>,
}

/// A replicated call to a canister method, whether update or query.
#[derive(Debug, Clone, Serialize)]
#[serde(tag = "request_type", rename = "call")]
pub struct EnvelopeCallContent {
    /// A random series of bytes to uniquely identify this message.
    #[serde(
        default,
        skip_serializing_if = "Option::is_none",
        serialize_with = "serialize_nonce"
    )]
    pub nonce: Option<Vec<u8>>,
    /// A nanosecond timestamp after which this request is no longer valid.
    pub ingress_expiry: u64,
    /// The principal that is sending this request.
    pub sender: Principal,
    /// The ID of the canister to be called.
    pub canister_id: Principal,
    /// The name of the canister method to be called.
    pub method_name: String,
    /// The argument to pass to the canister method.
    #[serde(serialize_with = "serialize_arg")]
    pub arg: Vec<u8>,
}

impl RepresentationHashable for EnvelopeCallContent {
    fn request_id(&self) -> RequestId {
        let mut map = vec![
            (
                "request_type".to_string(),
                RawHttpRequestVal::String("call".to_string()),
            ),
            (
                "canister_id".to_string(),
                RawHttpRequestVal::Bytes(self.canister_id.as_slice().to_vec()),
            ),
            (
                "method_name".to_string(),
                RawHttpRequestVal::String(self.method_name.to_string()),
            ),
            (
                "arg".to_string(),
                RawHttpRequestVal::Bytes(self.arg.clone()),
            ),
            (
                "ingress_expiry".to_string(),
                RawHttpRequestVal::U64(self.ingress_expiry),
            ),
            (
                "sender".to_string(),
                RawHttpRequestVal::Bytes(self.sender.as_slice().to_vec()),
            ),
        ]
        .into_iter()
        .collect::<BTreeMap<_, _>>();

        if let Some(some_nonce) = &self.nonce {
            map.insert(
                "nonce".to_string(),
                RawHttpRequestVal::Bytes(some_nonce.clone()),
            );
        }
        RequestId(hash_of_map(&map))
    }
}

/// A request for information from the [IC state tree](https://internetcomputer.org/docs/current/references/ic-interface-spec#state-tree).
#[derive(Debug, Clone, Serialize)]
#[serde(tag = "request_type", rename = "read_state")]
pub struct EnvelopeReadStateContent {
    /// A nanosecond timestamp after which this request is no longer valid.
    pub ingress_expiry: u64,
    /// The principal that is sending this request.
    pub sender: Principal,
    /// A list of paths within the state tree to fetch.
    pub paths: Vec<Vec<Label>>,
}

impl RepresentationHashable for EnvelopeReadStateContent {
    fn request_id(&self) -> RequestId {
        let map = vec![
            (
                "request_type".to_string(),
                RawHttpRequestVal::String("read_state".to_string()),
            ),
            (
                "ingress_expiry".to_string(),
                RawHttpRequestVal::U64(self.ingress_expiry),
            ),
            (
                "paths".to_string(),
                RawHttpRequestVal::Array(
                    self.paths
                        .iter()
                        .map(|p| {
                            RawHttpRequestVal::Array(
                                p.iter()
                                    .map(|b| RawHttpRequestVal::Bytes(b.as_slice().to_vec()))
                                    .collect(),
                            )
                        })
                        .collect(),
                ),
            ),
            (
                "sender".to_string(),
                RawHttpRequestVal::Bytes(self.sender.as_slice().to_vec()),
            ),
        ]
        .into_iter()
        .collect::<BTreeMap<_, _>>();

        RequestId(hash_of_map(&map))
    }
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

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn representation_independent_hash_call_or_query() {
        let content = EnvelopeCallContent {
            ingress_expiry: 1685570400000000000,
            sender: Principal::anonymous(),
            canister_id: Principal::from_slice(&[0, 0, 0, 0, 0, 0, 4, 210]),
            method_name: "hello".to_string(),
            arg: b"DIDL\x00\xFD*".to_vec(),
            nonce: None,
        };

        assert_eq!(
            tw_encoding::hex::encode(content.request_id().0, false),
            "1d1091364d6bb8a6c16b203ee75467d59ead468f523eb058880ae8ec80e2b101"
        );
    }

    #[test]
    fn representation_independent_hash_read_state() {
        let content = EnvelopeCallContent {
            ingress_expiry: 1685570400000000000,
            sender: Principal::anonymous(),
            canister_id: Principal::from_slice(&[0, 0, 0, 0, 0, 0, 4, 210]),
            method_name: "hello".to_string(),
            arg: b"DIDL\x00\xFD*".to_vec(),
            nonce: None,
        };
        let update_request_id = content.request_id();

        let content = EnvelopeReadStateContent {
            ingress_expiry: 1685570400000000000,
            sender: Principal::anonymous(),
            paths: vec![vec![
                Label::from("request_status"),
                Label::from(update_request_id),
            ]],
        };
        let request_id = content.request_id();
        assert_eq!(
            tw_encoding::hex::encode(request_id.0, false),
            "3cde0f14a953c3afbe1335f22e861bb62389f1449beca02707ab197e6829c2a3"
        );
    }
}
