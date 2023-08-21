use ic_certification::Label;
use maplit::btreemap;
use serde::{Deserialize, Serialize};
use sha2::{Digest, Sha256};
use std::collections::BTreeMap;

pub const DOMAIN_IC_REQUEST: &[u8; 11] = b"\x0Aic-request";

pub struct RequestId(pub [u8; 32]);

#[derive(Clone, Debug, PartialEq)]
pub enum CallOrQuery {
    Call,
    Query,
}

/// The different types of values supported in `RawHttpRequest`.
#[derive(Clone, Debug, Serialize, Deserialize, PartialEq, Eq, Hash)]
pub enum RawHttpRequestVal {
    Bytes(#[serde(with = "serde_bytes")] Vec<u8>),
    String(String),
    U64(u64),
    Array(Vec<RawHttpRequestVal>),
}

fn hash_string(value: String) -> Vec<u8> {
    let mut hasher = Sha256::new();
    hasher.update(value.as_bytes());
    let result = &hasher.finalize()[..];
    result.to_vec()
}

fn hash_bytes(value: Vec<u8>) -> Vec<u8> {
    /*let mut hasher = Sha256::new();
    hasher.update(&value);
    hasher.finish().to_vec()*/
    let mut hasher = Sha256::new();
    hasher.update(value.as_slice());
    let result = &hasher.finalize()[..];
    result.to_vec()
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
    let mut hasher = Sha256::new();
    elements
        .into_iter()
        // Hash the encoding of all the array elements.
        .for_each(|e| hasher.update(hash_val(e).as_slice()));
    //hasher.finish().to_vec() // hash the concatenation of the hashes.
    let result = &hasher.finalize()[..];
    result.to_vec()
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
fn hash_of_map<S: ToString>(map: &BTreeMap<S, RawHttpRequestVal>) -> [u8; 32] {
    let mut hashes: Vec<Vec<u8>> = Vec::new();
    for (key, val) in map.iter() {
        hashes.push(hash_key_val(key.to_string(), val.clone()));
    }

    // Computes hash by first sorting by "field name" hash, which is the
    // same as sorting by concatenation of H(field name) · H(field value)
    // (although in practice it's actually more stable in the presence of
    // duplicated field names).  Then concatenate all the hashes.
    hashes.sort();

    let mut hasher = Sha256::new();
    for hash in hashes {
        hasher.update(hash.as_slice());
    }

    let result = &hasher.finalize()[..];
    <[u8; 32]>::try_from(result).unwrap_or([0; 32])
}

pub fn representation_independent_hash_call_or_query(
    request_type: CallOrQuery,
    canister_id: Vec<u8>,
    method_name: &str,
    arg: Vec<u8>,
    ingress_expiry: u64,
    sender: Vec<u8>,
    nonce: Option<&[u8]>,
) -> RequestId {
    use RawHttpRequestVal::*;
    let mut map = btreemap! {
        "request_type".to_string() => match request_type {
            CallOrQuery::Call => String("call".to_string()),
            CallOrQuery::Query => String("query".to_string()),
        },
        "canister_id".to_string() => Bytes(canister_id),
        "method_name".to_string() => String(method_name.to_string()),
        "arg".to_string() => Bytes(arg),
        "ingress_expiry".to_string() => U64(ingress_expiry),
        "sender".to_string() => Bytes(sender),
    };
    if let Some(some_nonce) = nonce {
        map.insert("nonce".to_string(), Bytes(some_nonce.to_vec()));
    }
    RequestId(hash_of_map(&map))
}

pub fn representation_independent_hash_read_state(
    ingress_expiry: u64,
    paths: &[Vec<Label>],
    sender: Vec<u8>,
    nonce: Option<&[u8]>,
) -> RequestId {
    use RawHttpRequestVal::*;

    let mut map = btreemap! {
        "request_type".to_string() => String("read_state".to_string()),
        "ingress_expiry".to_string() => U64(ingress_expiry),
        "paths".to_string() => Array(paths
                .iter()
                .map(|p| {
                    RawHttpRequestVal::Array(
                        p.iter()
                            .map(|b| RawHttpRequestVal::Bytes(b.as_bytes().to_vec()))
                            .collect(),
                    )
                })
                .collect()),
        "sender".to_string() => Bytes(sender),
    };
    if let Some(some_nonce) = nonce {
        map.insert("nonce".to_string(), Bytes(some_nonce.to_vec()));
    }
    RequestId(hash_of_map(&map))
}

pub fn make_sig_data(message_id: &RequestId) -> Vec<u8> {
    // Lifted from canister_client::agent::sign_message_id
    let mut sig_data = vec![];
    sig_data.extend_from_slice(DOMAIN_IC_REQUEST);
    sig_data.extend_from_slice(message_id.0.as_slice());
    sig_data
}
