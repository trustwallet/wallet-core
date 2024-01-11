// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::envelope::{Envelope, EnvelopeCallContent, EnvelopeReadStateContent};
use serde::Serialize;

/// The types of requests that are available from the Rosetta node.
/// This enum is truncated to include support only for the
/// operations that this crate can currently perform.
#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub enum RequestType {
    // Aliases for backwards compatibility
    #[serde(rename = "TRANSACTION")]
    #[serde(alias = "Send")]
    Send,
}

/// The type (encoded as CBOR) returned by the Rosetta node's
/// /construction/combine endpoint. It contains the
/// IC calls to submit the transaction and to check the result.
pub type SignedTransaction = Vec<Request>;

/// A vector of update/read-state calls for different ingress windows
/// of the same call.
pub type Request = (RequestType, Vec<EnvelopePair>);

#[derive(Debug, Clone)]
pub enum EnvelopePairError {
    InvalidUpdateEnvelope,
    InvalidReadStateEnvelope,
}

/// A signed IC update call and the corresponding read-state call for
/// a particular ingress window.
#[derive(Debug, Clone, Serialize)]
pub struct EnvelopePair {
    update: Envelope<EnvelopeCallContent>,
    read_state: Envelope<EnvelopeReadStateContent>,
}

impl EnvelopePair {
    pub fn new(
        update_envelope: Envelope<EnvelopeCallContent>,
        read_state_envelope: Envelope<EnvelopeReadStateContent>,
    ) -> Result<Self, EnvelopePairError> {
        Ok(Self {
            update: update_envelope,
            read_state: read_state_envelope,
        })
    }
}
