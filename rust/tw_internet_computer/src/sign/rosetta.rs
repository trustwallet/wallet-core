use super::interface_spec::envelope::{Envelope, EnvelopeContent};
use serde::{Deserialize, Serialize};

/// `RequestType` contains all supported values of `Operation.type`.
/// Extra information, such as `neuron_index` should only be included
/// if it cannot be parsed from the submit payload.
#[derive(Debug, Clone, PartialEq, Eq, Deserialize, Serialize)]
pub enum RequestType {
    // Aliases for backwards compatibility
    #[serde(rename = "TRANSACTION")]
    #[serde(alias = "Send")]
    Send,
}

/// The type (encoded as CBOR) returned by /construction/combine, containing the
/// IC calls to submit the transaction and to check the result.
pub type SignedTransaction = Vec<Request>;

/// A vector of update/read-state calls for different ingress windows
/// of the same call.
pub type Request = (RequestType, Vec<EnvelopePair>);

/// A signed IC update call and the corresponding read-state call for
/// a particular ingress window.
#[derive(Debug, Clone, Serialize, Deserialize)]
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
