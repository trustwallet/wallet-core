use std::collections::BTreeMap;

use ic_certification::Label;
use serde::{Deserialize, Serialize};
use tw_hash::{sha2::sha256, H256};

use super::envelope::EnvelopeContent;

const DOMAIN_IC_REQUEST: &[u8; 11] = b"\x0Aic-request";

/// When signing requests or querying the status of a request
/// (see Request status) in the state tree, the user identifies
/// the request using a request id, which is the
/// representation-independent hash of the content map of the
/// original request. A request id must have length of 32 bytes.
pub struct RequestId(pub(crate) H256);

impl RequestId {
    /// Create the prehash from the request ID.
    /// See: https://internetcomputer.org/docs/current/references/ic-interface-spec#envelope-authentication
    pub fn sig_data(&self) -> H256 {
        let mut sig_data = vec![];
        sig_data.extend_from_slice(DOMAIN_IC_REQUEST);
        sig_data.extend_from_slice(self.0.as_slice());
        H256::try_from(sha256(&sig_data).as_slice()).unwrap_or_else(|_| H256::new())
    }
}

impl From<&EnvelopeContent> for RequestId {
    fn from(value: &EnvelopeContent) -> Self {
        match value {
            EnvelopeContent::Call {
                nonce,
                ingress_expiry,
                sender,
                canister_id,
                method_name,
                arg,
            } => representation_independent_hash_call(
                canister_id.as_slice().to_vec(),
                method_name,
                arg.clone(),
                *ingress_expiry,
                sender.as_slice().to_vec(),
                nonce.as_deref(),
            ),
            EnvelopeContent::ReadState {
                ingress_expiry,
                sender,
                paths,
            } => representation_independent_hash_read_state(
                *ingress_expiry,
                paths,
                sender.as_slice().to_vec(),
                None,
            ),
        }
    }
}

/// The different types of values supported in `RawHttpRequest`.
#[derive(Clone, Debug, Serialize, Deserialize, PartialEq, Eq, Hash)]
enum RawHttpRequestVal {
    Bytes(#[serde(with = "serde_bytes")] Vec<u8>),
    String(String),
    U64(u64),
    Array(Vec<RawHttpRequestVal>),
}

fn hash_string(value: String) -> Vec<u8> {
    sha256(value.as_bytes())
}

fn hash_bytes(value: Vec<u8>) -> Vec<u8> {
    sha256(value.as_slice())
}

fn hash_u64(value: u64) -> Vec<u8> {
    // We need at most ⌈ 64 / 7 ⌉ = 10 bytes to encode a 64 bit
    // integer in LEB128.
    let mut buf = [0u8; 10];
    let mut n = value;
    let mut i = 0;

    loop {
        let byte = (n & 0x7f) as u8;
        n >>= 7;

        if n == 0 {
            buf[i] = byte;
            break;
        } else {
            buf[i] = byte | 0x80;
            i += 1;
        }
    }

    hash_bytes(buf[..=i].to_vec())
}

// arrays, encoded as the concatenation of the hashes of the encodings of the
// array elements.
fn hash_array(elements: Vec<RawHttpRequestVal>) -> Vec<u8> {
    let mut buffer = vec![];
    elements
        .into_iter()
        // Hash the encoding of all the array elements.
        .for_each(|e| {
            let mut hashed_val = hash_val(e);
            buffer.append(&mut hashed_val);
        });
    sha256(&buffer)
}

fn hash_val(val: RawHttpRequestVal) -> Vec<u8> {
    match val {
        RawHttpRequestVal::String(string) => hash_string(string),
        RawHttpRequestVal::Bytes(bytes) => hash_bytes(bytes),
        RawHttpRequestVal::U64(integer) => hash_u64(integer),
        RawHttpRequestVal::Array(elements) => hash_array(elements),
    }
}

fn hash_key_val(key: String, val: RawHttpRequestVal) -> Vec<u8> {
    let mut key_hash = hash_string(key);
    let mut val_hash = hash_val(val);
    key_hash.append(&mut val_hash);
    key_hash
}

/// Describes `hash_of_map` as specified in the public spec.
/// See: https://internetcomputer.org/docs/current/references/ic-interface-spec#hash-of-map
fn hash_of_map<S: ToString>(map: &BTreeMap<S, RawHttpRequestVal>) -> H256 {
    let mut hashes: Vec<Vec<u8>> = Vec::new();
    for (key, val) in map.iter() {
        hashes.push(hash_key_val(key.to_string(), val.clone()));
    }

    // Computes hash by first sorting by "field name" hash, which is the
    // same as sorting by concatenation of H(field name) · H(field value)
    // (although in practice it's actually more stable in the presence of
    // duplicated field names).  Then concatenate all the hashes.
    hashes.sort();

    let buffer = hashes.into_iter().flatten().collect::<Vec<_>>();
    let hash = sha256(&buffer);

    H256::try_from(hash.as_slice()).unwrap_or_else(|_| H256::new())
}

fn representation_independent_hash_call(
    canister_id: Vec<u8>,
    method_name: &str,
    arg: Vec<u8>,
    ingress_expiry: u64,
    sender: Vec<u8>,
    nonce: Option<&[u8]>,
) -> RequestId {
    let mut map = vec![
        (
            "request_type".to_string(),
            RawHttpRequestVal::String("call".to_string()),
        ),
        (
            "canister_id".to_string(),
            RawHttpRequestVal::Bytes(canister_id),
        ),
        (
            "method_name".to_string(),
            RawHttpRequestVal::String(method_name.to_string()),
        ),
        ("arg".to_string(), RawHttpRequestVal::Bytes(arg)),
        (
            "ingress_expiry".to_string(),
            RawHttpRequestVal::U64(ingress_expiry),
        ),
        ("sender".to_string(), RawHttpRequestVal::Bytes(sender)),
    ]
    .into_iter()
    .collect::<BTreeMap<_, _>>();

    if let Some(some_nonce) = nonce {
        map.insert(
            "nonce".to_string(),
            RawHttpRequestVal::Bytes(some_nonce.to_vec()),
        );
    }
    RequestId(hash_of_map(&map))
}

fn representation_independent_hash_read_state(
    ingress_expiry: u64,
    paths: &[Vec<Label>],
    sender: Vec<u8>,
    nonce: Option<&[u8]>,
) -> RequestId {
    let mut map = vec![
        (
            "request_type".to_string(),
            RawHttpRequestVal::String("read_state".to_string()),
        ),
        (
            "ingress_expiry".to_string(),
            RawHttpRequestVal::U64(ingress_expiry),
        ),
        (
            "paths".to_string(),
            RawHttpRequestVal::Array(
                paths
                    .iter()
                    .map(|p| {
                        RawHttpRequestVal::Array(
                            p.iter()
                                .map(|b| RawHttpRequestVal::Bytes(b.as_bytes().to_vec()))
                                .collect(),
                        )
                    })
                    .collect(),
            ),
        ),
        ("sender".to_string(), RawHttpRequestVal::Bytes(sender)),
    ]
    .into_iter()
    .collect::<BTreeMap<_, _>>();

    if let Some(some_nonce) = nonce {
        map.insert(
            "nonce".to_string(),
            RawHttpRequestVal::Bytes(some_nonce.to_vec()),
        );
    }
    RequestId(hash_of_map(&map))
}

#[cfg(test)]
mod test {
    use crate::principal::Principal;

    #[test]
    fn representation_independent_hash_call_or_query() {
        let hash = super::representation_independent_hash_call(
            vec![0, 0, 0, 0, 0, 0, 4, 210],
            "hello",
            b"DIDL\x00\xFD*".to_vec(),
            1685570400000000000,
            Principal::anonymous().as_slice().to_vec(),
            None,
        );

        assert_eq!(
            tw_encoding::hex::encode(hash.0, false),
            "1d1091364d6bb8a6c16b203ee75467d59ead468f523eb058880ae8ec80e2b101"
        );
    }
}
