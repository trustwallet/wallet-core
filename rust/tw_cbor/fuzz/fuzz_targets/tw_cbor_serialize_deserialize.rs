#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};

use serde::{Deserialize, Serialize};

use tw_cbor::{deserialize, serialize};

#[derive(arbitrary::Arbitrary, Serialize, Deserialize, PartialEq, Debug)]
struct User {
    name: String,
    age: u64,
    friends: Vec<String>,
    #[serde(with = "serde_bytes")]
    address: Vec<u8>,
    key: Vec<u8>,
}

fuzz_target!(|se_user: User| {
    let serialized = serialize(&se_user).unwrap();
    let de_user = deserialize::<User>(&serialized).unwrap();
    assert_eq!(se_user, de_user);
});
