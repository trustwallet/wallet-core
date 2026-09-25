// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};
use tw_coin_entry::error::prelude::{
    MapTWError, ResultContext, SigningError, SigningErrorType, SigningResult,
};

pub mod common;
pub mod orchard;
pub mod sapling;
pub mod transparent;

const MAGIC_BYTES: &[u8] = b"PCZT";
const PCZT_VERSION_1: u32 = 1;

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Pczt {
    /// Global fields that are relevant to the transaction as a whole.
    pub global: common::Global,

    //
    // Protocol-specific fields.
    //
    // Unlike the `TransactionData` type in `zcash_primitives`, these are not optional.
    // This is because a PCZT does not always contain a semantically-valid transaction,
    // and there may be phases where we need to store protocol-specific metadata before
    // it has been determined whether there are protocol-specific inputs or outputs.
    //
    pub transparent: transparent::Bundle,
    pub sapling: sapling::Bundle,
    pub orchard: orchard::Bundle,
}

impl Pczt {
    pub fn deserialize(bytes: &[u8]) -> SigningResult<Self> {
        if bytes.len() < 8 {
            return SigningError::err(SigningErrorType::Error_input_parse)
                .context("PCZT is too short");
        }
        if &bytes[..4] != MAGIC_BYTES {
            return SigningError::err(SigningErrorType::Error_input_parse)
                .context("PCZT magic prefix mismatch");
        }
        let version = u32::from_le_bytes(bytes[4..8].try_into().unwrap());
        if version != PCZT_VERSION_1 {
            return SigningError::err(SigningErrorType::Error_input_parse)
                .context("PCZT version mismatch");
        }

        // This is a v1 PCZT.
        postcard::from_bytes(&bytes[8..])
            .tw_err(SigningErrorType::Error_input_parse)
            .context("PCZT deserialization failed")
    }

    pub fn serialize(&self) -> SigningResult<Vec<u8>> {
        let mut bytes = vec![];
        bytes.extend_from_slice(MAGIC_BYTES);
        bytes.extend_from_slice(&PCZT_VERSION_1.to_le_bytes());
        postcard::to_extend(self, bytes)
            .tw_err(SigningErrorType::Error_internal)
            .context("PCZT serialization failed")
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::base64;

    #[test]
    fn test_pczt_serde() {
        let input = base64::decode("UENaVAEAAAAFis6ctQLVoJzHDAEAmei/AYUBgwACD/xF9t6uGgnrn6HPk4TTxoTB42mxjfZ2E8dQ7UzfodcAAf////8PAAAAgIl6GXapFFWjZeeDsUiGjDI8hm1k+WCt0MtqiKwAAAEAAAAAAACg+XQ96jWHJsJWFA3/kk+6ZaHhUI5wi57Hf3ZM6xQzRQEB/////w8AAAC1lhgZdqkUVaNl54OxSIaMMjyGbWT5YK3Qy2qIrAAAAQAAAAAAAALD8X4ZdqkUoMoEavwq69Y+ntl8YeB6ACxeCG2IrAAAASN0MVlYbjhTNm40S0F6QXhHUHJ1Tk1wd2ZDZTJwVFpKWWQzSwDCwxEZdqkUVaNl54OxSIaMMjyGbWT5YK3Qy2qIrAAAASN0MVJnUkJqam54WFNnMXB0TERya2FNTmlWNHRKVlh1N2RXVgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==", base64::STANDARD).unwrap();
        let pczt = Pczt::deserialize(&input).unwrap();
        let output = pczt.serialize().unwrap();
        assert_eq!(input, output);
    }
}
