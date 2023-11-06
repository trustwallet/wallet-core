// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::private_key::SignatureData;
use crate::public_key::JsonPublicKey;
use serde::Serialize;
use std::marker::PhantomData;
use tw_encoding::base64::Base64Encoded;

/// `JsonSerializer` serializes transaction to JSON in Cosmos specific way.
pub struct JsonSerializer<Address, PublicKey> {
    _phantom: PhantomData<(Address, PublicKey)>,
}

#[derive(Serialize)]
pub struct AnyMsg<Value> {
    #[serde(rename = "type")]
    msg_type: String,
    value: Value,
}

#[derive(Serialize)]
pub struct SignatureJson {
    pub_key: AnyMsg<Base64Encoded>,
    signature: Base64Encoded,
}

impl SignatureJson {
    pub fn to_json_string(&self) -> String {
        // It's safe to unwrap here because `SignatureJson` consists of checked fields only.
        serde_json::to_string(self).expect("Unexpected error on serializing a SignatureJson")
    }
}

impl<Address, PublicKey> JsonSerializer<Address, PublicKey>
where
    Address: ToString,
    PublicKey: JsonPublicKey,
{
    pub fn serialize_signature(public_key: &PublicKey, signature: SignatureData) -> SignatureJson {
        SignatureJson {
            pub_key: Self::serialize_public_key(public_key),
            signature: Base64Encoded(signature),
        }
    }

    pub fn serialize_public_key(public_key: &PublicKey) -> AnyMsg<Base64Encoded> {
        AnyMsg {
            msg_type: public_key.public_key_type(),
            value: Base64Encoded(public_key.to_bytes()),
        }
    }
}
