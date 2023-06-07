// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{KeyPairError, KeyPairResult};
use k256::ecdsa;

/// The standard binary signature representation length.
/// RS, where R - 32 byte array, S - 32 byte array.
const SIGNATURE_LENGTH: usize = 64;

/// Tries to parse ECDSA signature from ASN.1 DER encoded bytes.
/// Returns the signature standard binary representation:
/// RS, where R - 32 byte array, S - 32 byte array.
pub fn parse_ecdsa_signature(input: &[u8]) -> KeyPairResult<[u8; 64]> {
    let sign = ecdsa::Signature::from_der(input).map_err(|_| KeyPairError::InvalidSignature)?;
    let sign_bytes = sign.to_bytes();

    assert_eq!(sign_bytes.len(), SIGNATURE_LENGTH);

    let mut res = [0u8; SIGNATURE_LENGTH];
    res.copy_from_slice(sign_bytes.as_slice());
    Ok(res)
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex;

    #[test]
    fn test_parse_ecdsa_signature() {
        #[track_caller]
        fn test_parse_ecdsa_sign_impl(encoded: &str, expected: &str) {
            let encoded_bytes = hex::decode(encoded).unwrap();
            let actual = parse_ecdsa_signature(&encoded_bytes).unwrap();
            let expected_bytes = hex::decode(expected).unwrap();
            assert_eq!(actual, expected_bytes.as_slice());
        }

        test_parse_ecdsa_sign_impl(
            "3045022100B2B31575F8536B284410D01217F688BE3A9FAF4BA0BA3A9093F983E40D630EC7022022A7A25B01403CFF0D00B3B853D230F8E96FF832B15D4CCC75203CB65896A2D5",
            "b2b31575f8536b284410d01217f688be3a9faf4ba0ba3a9093f983e40d630ec722a7a25b01403cff0d00b3b853d230f8e96ff832b15d4ccc75203cb65896a2d5"
        );
        test_parse_ecdsa_sign_impl(
            "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1",
            "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1"
        );
    }
}
