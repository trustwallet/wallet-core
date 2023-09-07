// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::{Deserialize, Serialize};

use tw_cbor::{deserialize, serialize};

const SERIALIZED: [u8; 70] = [
    165, 100, 110, 97, 109, 101, 101, 65, 108, 105, 99, 101, 99, 97, 103, 101, 24, 42, 103, 102,
    114, 105, 101, 110, 100, 115, 131, 99, 66, 111, 98, 101, 67, 97, 114, 111, 108, 100, 68, 97,
    118, 101, 103, 97, 100, 100, 114, 101, 115, 115, 68, 0, 1, 2, 3, 99, 107, 101, 121, 138, 1, 2,
    3, 4, 5, 6, 7, 8, 9, 0,
];

#[derive(Serialize, Deserialize, PartialEq, Debug)]
struct User {
    name: String,
    age: u64,
    friends: Vec<String>,
    #[serde(with = "serde_bytes")]
    address: Vec<u8>,
    key: Vec<u8>,
}

fn test_user() -> User {
    User {
        name: "Alice".to_string(),
        age: 42,
        friends: vec!["Bob".to_string(), "Carol".to_string(), "Dave".to_string()],
        address: vec![0, 1, 2, 3],
        key: vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 0],
    }
}

#[test]
fn test_serialize() {
    let user = test_user();
    let serialized = serialize(&user).unwrap();
    assert_eq!(serialized, SERIALIZED);
}

#[test]
fn test_deserialize() {
    let user = deserialize::<User>(&SERIALIZED).unwrap();
    assert_eq!(user, test_user());
}

#[test]
fn test_serialize_deserialize() {
    let se_user = test_user();
    let serialized = serialize(&se_user).unwrap();
    let de_user = deserialize::<User>(&serialized).unwrap();
    assert_eq!(se_user, de_user);
}
