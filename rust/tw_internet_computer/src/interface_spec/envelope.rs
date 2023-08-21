use candid::Principal;
use ic_certification::Label;
use serde::{Deserialize, Serialize};

use super::request_id::{self, RequestId};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Envelope {
    pub content: EnvelopeContent,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sender_pubkey: Option<Vec<u8>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sender_sig: Option<Vec<u8>>,
}

/// The content of an IC ingress message, not including any signature information.
#[derive(Debug, Clone, Serialize, Deserialize)]
#[serde(tag = "request_type", rename_all = "snake_case")]
pub enum EnvelopeContent {
    /// A replicated call to a canister method, whether update or query.
    Call {
        /// A random series of bytes to uniquely identify this message.
        #[serde(default, skip_serializing_if = "Option::is_none", with = "serde_bytes")]
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
        #[serde(with = "serde_bytes")]
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
    /// An unreplicated call to a canister query method.
    Query {
        /// A nanosecond timestamp after which this request is no longer valid.
        ingress_expiry: u64,
        /// The principal that is sending this request.
        sender: Principal,
        /// The ID of the canister to be called.
        canister_id: Principal,
        /// The name of the canister method to be called.
        method_name: String,
        /// The argument to pass to the canister method.
        #[serde(with = "serde_bytes")]
        arg: Vec<u8>,
    },
}

impl EnvelopeContent {
    pub fn to_request_id(&self) -> RequestId {
        match self {
            EnvelopeContent::Call {
                nonce,
                ingress_expiry,
                sender,
                canister_id,
                method_name,
                arg,
            } => request_id::representation_independent_hash_call_or_query(
                request_id::CallOrQuery::Call,
                canister_id.as_slice().to_vec(),
                method_name,
                arg.clone(),
                *ingress_expiry,
                sender.as_slice().to_vec(),
                nonce.as_deref(),
            ),
            EnvelopeContent::Query {
                ingress_expiry,
                sender,
                canister_id,
                method_name,
                arg,
            } => request_id::representation_independent_hash_call_or_query(
                request_id::CallOrQuery::Query,
                canister_id.as_slice().to_vec(),
                method_name,
                arg.clone(),
                *ingress_expiry,
                sender.as_slice().to_vec(),
                None,
            ),
            EnvelopeContent::ReadState {
                ingress_expiry,
                sender,
                paths,
            } => request_id::representation_independent_hash_read_state(
                *ingress_expiry,
                paths,
                sender.as_slice().to_vec(),
                None,
            ),
        }
    }
}
