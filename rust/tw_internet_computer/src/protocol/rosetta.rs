// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use super::envelope::{Envelope, EnvelopeContent};
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
    update: Envelope,
    read_state: Envelope,
}

impl EnvelopePair {
    pub fn new(
        update_envelope: Envelope,
        read_state_envelope: Envelope,
    ) -> Result<Self, EnvelopePairError> {
        if !matches!(update_envelope.content, EnvelopeContent::Call { .. }) {
            return Err(EnvelopePairError::InvalidUpdateEnvelope);
        }

        if !matches!(
            read_state_envelope.content,
            EnvelopeContent::ReadState { .. }
        ) {
            return Err(EnvelopePairError::InvalidReadStateEnvelope);
        }

        Ok(Self {
            update: update_envelope,
            read_state: read_state_envelope,
        })
    }
}

#[cfg(test)]
mod test {
    use crate::protocol::principal::Principal;

    use super::*;

    #[test]
    fn envelope_pair_creation() {
        let update_envelope = Envelope {
            content: EnvelopeContent::Call {
                nonce: None,
                ingress_expiry: 0,
                sender: Principal::anonymous(),
                canister_id: Principal::anonymous(),
                method_name: "".to_string(),
                arg: vec![],
            },
            sender_pubkey: None,
            sender_sig: None,
        };

        let read_state_envelope = Envelope {
            content: EnvelopeContent::ReadState {
                ingress_expiry: 0,
                sender: Principal::anonymous(),
                paths: vec![],
            },
            sender_pubkey: None,
            sender_sig: None,
        };

        // A valid envelope pair.
        let _ = EnvelopePair::new(update_envelope.clone(), read_state_envelope.clone()).unwrap();

        // Invalid update envelope.
        let pair_result =
            EnvelopePair::new(read_state_envelope.clone(), read_state_envelope.clone());
        assert!(matches!(
            pair_result,
            Err(EnvelopePairError::InvalidUpdateEnvelope)
        ));

        // Invalid read state envelope.
        let pair_result = EnvelopePair::new(update_envelope.clone(), update_envelope.clone());
        assert!(matches!(
            pair_result,
            Err(EnvelopePairError::InvalidReadStateEnvelope)
        ));
    }
}
