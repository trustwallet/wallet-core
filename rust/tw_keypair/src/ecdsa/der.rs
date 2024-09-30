// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{KeyPairError, KeyPairResult};
use der::asn1::UintRef;
use der::{Decode, Encode, Reader};
use tw_hash::{H256, H512};
use tw_memory::Data;

/// The standard binary signature representation length.
/// RS, where R - 32 byte array, S - 32 byte array.
const R_LENGTH: usize = H256::LEN;
const S_LENGTH: usize = H256::LEN;

pub type SignatureBytes = H512;

/// ASN.1 DER-encoded signature as specified in [RFC5912 Appendix A]:
///
/// ```text
/// ECDSA-Sig-Value ::= SEQUENCE {
///   r  INTEGER,
///   s  INTEGER
/// }
/// ```
///
/// [RFC5912 Appendix A]: https://www.rfc-editor.org/rfc/rfc5912#appendix-A
#[derive(Debug)]
pub struct Signature {
    r: H256,
    s: H256,
    /// ASN.1 DER-encoded signature data.
    der_bytes: Data,
}

impl Signature {
    pub fn new(r: H256, s: H256) -> KeyPairResult<Signature> {
        let der_bytes =
            encode_der(r.as_slice(), s.as_slice()).map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(Signature { r, s, der_bytes })
    }

    /// Parses signature from DER-encoded bytes.
    /// Source: https://github.com/RustCrypto/signatures/blob/ecb112aa25ac210d86946089cddf0a62a672ce14/ecdsa/src/der.rs#L83-L108
    pub fn from_bytes(input: &[u8]) -> KeyPairResult<Signature> {
        let (r, s) = decode_der(input).map_err(|_| KeyPairError::InvalidSignature)?;

        if r.as_bytes().len() > R_LENGTH || s.as_bytes().len() > S_LENGTH {
            return Err(KeyPairError::InvalidSignature);
        }

        let r_begin = R_LENGTH.saturating_sub(r.as_bytes().len());
        let s_begin = S_LENGTH.saturating_sub(s.as_bytes().len());

        let mut r_complete = H256::default();
        r_complete[r_begin..].copy_from_slice(r.as_bytes());

        let mut s_complete = H256::default();
        s_complete[s_begin..].copy_from_slice(s.as_bytes());

        Ok(Signature {
            r: r_complete,
            s: s_complete,
            der_bytes: input.to_vec(),
        })
    }

    /// Returns ASN.1 DER-encoded signature data.
    pub fn der_bytes(&self) -> Data {
        self.der_bytes.clone()
    }

    /// Get the `r` component of the signature.
    pub fn r(&self) -> &[u8] {
        self.r.as_slice()
    }

    /// Get the `s` component of the signature.
    pub fn s(&self) -> &[u8] {
        self.s.as_slice()
    }

    /// Returns the standard binary signature representation:
    /// RS, where R - 32 byte array, S - 32 byte array.
    pub fn to_bytes(&self) -> SignatureBytes {
        let mut sign = SignatureBytes::default();

        sign[0..R_LENGTH].copy_from_slice(self.r());
        sign[R_LENGTH..].copy_from_slice(self.s());

        sign
    }

    pub fn to_vec(&self) -> Vec<u8> {
        self.to_bytes().to_vec()
    }
}

/// Decode the `r` and `s` components of a DER-encoded ECDSA signature.
/// Source: https://github.com/RustCrypto/signatures/blob/ecb112aa25ac210d86946089cddf0a62a672ce14/ecdsa/src/der.rs#L357C69-L370
fn decode_der(der_bytes: &[u8]) -> der::Result<(UintRef<'_>, UintRef<'_>)> {
    let mut reader = der::SliceReader::new(der_bytes)?;
    let header = der::Header::decode(&mut reader)?;
    header.tag.assert_eq(der::Tag::Sequence)?;

    let ret = reader.read_nested(header.length, |reader| {
        let r = UintRef::decode(reader)?;
        let s = UintRef::decode(reader)?;
        Ok((r, s))
    })?;

    reader.finish(ret)
}

/// Create an ASN.1 DER encoded signature from big endian `r` and `s` scalar components.
/// Source: https://github.com/RustCrypto/signatures/blob/ecb112aa25ac210d86946089cddf0a62a672ce14/ecdsa/src/der.rs#L110-L128
fn encode_der(r: &[u8], s: &[u8]) -> der::Result<Data> {
    const DER_CAPACITY: usize = 100;

    let r_uint = UintRef::new(r)?;
    let s_uint = UintRef::new(s)?;

    let mut bytes = [0u8; DER_CAPACITY];
    let mut writer = der::SliceWriter::new(bytes.as_mut_slice());

    writer.sequence((r_uint.encoded_len()? + s_uint.encoded_len()?)?, |seq| {
        seq.encode(&r_uint)?;
        seq.encode(&s_uint)
    })?;

    // Returns an actual der signature bytes.
    Ok(writer.finish()?.to_vec())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::str::FromStr;
    use tw_encoding::hex;
    use tw_encoding::hex::ToHex;

    #[test]
    fn test_parse_ecdsa_signature() {
        #[track_caller]
        fn test_impl(encoded: &str, expected: &str) {
            // Test decoding an ecdsa signature from der.
            let encoded_bytes = hex::decode(encoded).unwrap();
            let sign = Signature::from_bytes(&encoded_bytes).unwrap();
            let actual = sign.to_bytes();
            let expected_bytes = SignatureBytes::from_str(expected).expect("!decode");
            assert_eq!(actual, expected_bytes, "!decode");

            // Test encoding an ecdsa signature to der.
            let new_signature = Signature::new(sign.r, sign.s).expect("!encode");
            assert_eq!(new_signature.der_bytes().to_hex(), encoded, "!encode");
        }

        test_impl(
            "3045022100b2b31575f8536b284410d01217f688be3a9faf4ba0ba3a9093f983e40d630ec7022022a7a25b01403cff0d00b3b853d230f8e96ff832b15d4ccc75203cb65896a2d5",
            "b2b31575f8536b284410d01217f688be3a9faf4ba0ba3a9093f983e40d630ec722a7a25b01403cff0d00b3b853d230f8e96ff832b15d4ccc75203cb65896a2d5"
        );
        test_impl(
            "304402200f5d5a9e5fc4b82a625312f3be5d3e8ad017d882de86c72c92fcefa924e894c102202071772a14201a3a0debf381b5e8dea39fadb9bcabdc02ee71ab018f55bf717f",
            "0f5d5a9e5fc4b82a625312f3be5d3e8ad017d882de86c72c92fcefa924e894c12071772a14201a3a0debf381b5e8dea39fadb9bcabdc02ee71ab018f55bf717f"
        );
        test_impl(
            "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1",
            "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1"
        );
        test_impl(
            "3046022100b4877d788d3756399873fd0346765c5462ec07093e58360f5d29cd76bf7d390b022100de9c7ab3fd733d3385416a430b2976875e2a749e203cf5cc1bf0135d1a2ccbef",
            "b4877d788d3756399873fd0346765c5462ec07093e58360f5d29cd76bf7d390bde9c7ab3fd733d3385416a430b2976875e2a749e203cf5cc1bf0135d1a2ccbef"
        );
        test_impl(
            "3044022029acfd2df79350389e6a33ccddd845b7adb9eb7fe08d2ed05f141330e369b518022043989b824b1532e9d51d87253f0698afa9d1081f0609cc50ad4f3c46e7bdedcf",
            "29acfd2df79350389e6a33ccddd845b7adb9eb7fe08d2ed05f141330e369b51843989b824b1532e9d51d87253f0698afa9d1081f0609cc50ad4f3c46e7bdedcf"
        );
        test_impl(
            "3045022100c0b8b2992cf439ea37d8686cd0515c255381e58b7de4f2ba87cc24445f1efcc30220300262b0a687bc89e22ecb61fc85d70dd9d8f326c81694bbc6e505bcde3a2094",
            "c0b8b2992cf439ea37d8686cd0515c255381e58b7de4f2ba87cc24445f1efcc3300262b0a687bc89e22ecb61fc85d70dd9d8f326c81694bbc6e505bcde3a2094"
        );
        test_impl(
            "303d021d00f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f600000000021c70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf900000000",
            "00000000f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6000000000000000070d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf900000000"
        );
        test_impl(
            "3006020110020110",
            "00000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010"
        );
    }

    #[test]
    fn test_parse_ecdsa_signature_invalid() {
        #[track_caller]
        fn test_impl(encoded: &str) {
            let encoded_bytes = hex::decode(encoded).unwrap();
            Signature::from_bytes(&encoded_bytes).unwrap_err();
        }

        test_impl("");
        test_impl(
            "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100",
        );
        test_impl(
            "3045022100B2B31575F8536B284410D01217F688BE3A9FAF4BA0BA3A9093F983E40D630EC7022022A7A25B01403CFF0D00B3B853D230F8E96FF832B15D4CCC75203CB65896A2"
        );
        test_impl(
            "3045022100B2B31575F8536B284410D01217F688BE3A9FAF4BA0BA3A9093F983E40D630EC7022022A7A25B01403CFF0D00B3B853D230F8E96FF832B15D4CCC75203CB65896A2D5D5"
        );
        test_impl(
            "3045022100B2B31575F8536B284410D01217F688BE3A9FAF4BA0BA3A9093F983E40D630EC7022022A7A25B01403CFF0D00B3B853D230F8E96FF832B15D4CCC75203CB65896A2D53045022100B2B31575F8536B284410D01217F688BE3A9FAF4BA0BA3A9093F983E40D630EC7022022A7A25B01403CFF0D00B3B853D230F8E96FF832B15D4CCC75203CB65896A2D5"
        );
    }
}
