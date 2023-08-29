use crate::interface_spec::envelope::{Envelope, EnvelopeContent};
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

/// A signed IC update call and the corresponding read-state call for
/// a particular ingress window.
#[derive(Debug, Clone, Serialize)]
pub struct EnvelopePair {
    pub update: Envelope,
    pub read_state: Envelope,
}

impl EnvelopePair {
    pub fn new(update_envelope: Envelope, read_state_envelope: Envelope) -> Self {
        assert!(matches!(
            update_envelope.content,
            EnvelopeContent::Call { .. }
        ));

        assert!(matches!(
            read_state_envelope.content,
            EnvelopeContent::ReadState { .. }
        ));

        Self {
            update: update_envelope,
            read_state: read_state_envelope,
        }
    }
}
