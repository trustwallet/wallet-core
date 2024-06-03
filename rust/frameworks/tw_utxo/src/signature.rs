// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::sighash::SighashType;
use tw_coin_entry::error::prelude::*;
use tw_hash::H512;
use tw_keypair::ecdsa::der;
use tw_keypair::{ecdsa, schnorr};
use tw_memory::Data;

/// Use the max value of the u8 type to avoid potential serialization optimizations (like ASN.1 DER).
const ESTIMATION_DEFAULT_BYTE: u8 = u8::MAX;

/// A Bitcoin ECDSA signature with a sighash type, which must be serialzed
/// occordingly in the scriptSig/Witness data to spend an output.
pub struct BitcoinEcdsaSignature {
    /// ECDSA signature serialized as DER. See [`der::Signature`].
    der_sig: Data,
    sighash_ty: u8,
}

impl BitcoinEcdsaSignature {
    // The max size of the serialized signature including sighash type.
    const SER_SIZE: usize = 73;

    pub fn new(sig: der::Signature, sighash_ty: SighashType) -> SigningResult<Self> {
        let der_sig = sig.der_bytes();
        let sighash_ty = sighash_ty.serialize()?;
        Ok(BitcoinEcdsaSignature {
            der_sig,
            sighash_ty,
        })
    }

    pub fn estimated() -> Self {
        BitcoinEcdsaSignature {
            der_sig: vec![ESTIMATION_DEFAULT_BYTE; Self::SER_SIZE - 1],
            sighash_ty: ESTIMATION_DEFAULT_BYTE,
        }
    }

    pub fn serialize(&self) -> Data {
        let mut ser = Vec::with_capacity(Self::SER_SIZE);
        ser.extend_from_slice(self.der_sig.as_slice());
        ser.push(self.sighash_ty);
        debug_assert!(ser.len() <= Self::SER_SIZE);
        ser
    }
}

pub struct BitcoinSchnorrSignature {
    sig: H512,
    sighash_ty: u8,
}

impl BitcoinSchnorrSignature {
    pub fn new(
        sig: schnorr::Signature,
        sighash_ty: SighashType,
    ) -> SigningResult<BitcoinSchnorrSignature> {
        Ok(BitcoinSchnorrSignature {
            sig: sig.bytes(),
            sighash_ty: sighash_ty.serialize_as_taproot()?,
        })
    }

    pub fn estimated() -> BitcoinSchnorrSignature {
        BitcoinSchnorrSignature {
            sig: H512::from([ESTIMATION_DEFAULT_BYTE; H512::LEN]),
            sighash_ty: ESTIMATION_DEFAULT_BYTE,
        }
    }

    pub fn serialize(&self) -> Vec<u8> {
        let mut ser = Vec::with_capacity(65);
        ser.extend_from_slice(self.sig.as_slice());
        if self.sighash_ty != 0 {
            ser.push(self.sighash_ty)
        }
        ser
    }
}

pub trait FromRawOrDerBytes: Sized {
    fn from_raw_or_der_bytes(bytes: &[u8]) -> SigningResult<Self>;
}

impl FromRawOrDerBytes for ecdsa::secp256k1::VerifySignature {
    /// Tries to create a secp256k1 verify signature from either a raw bytes (64-65 length)
    /// or ASN.1 DER bytes (70-72 length).
    fn from_raw_or_der_bytes(bytes: &[u8]) -> SigningResult<Self> {
        if ecdsa::secp256k1::Signature::VERIFY_SIGNATURE_LEN_RANGE.contains(&bytes.len()) {
            return ecdsa::secp256k1::VerifySignature::try_from(bytes)
                .into_tw()
                .context("Given an invalid ecdsa signature");
        }

        der::Signature::from_bytes(bytes)
            .and_then(ecdsa::secp256k1::VerifySignature::from_der)
            .into_tw()
            .context("Given an invalid ASN.1 DER signature")
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    #[test]
    fn test_verify_signature_from_raw_or_der_bytes() {
        struct TestCase {
            bytes: &'static str,
            expected: &'static str,
        }

        #[track_caller]
        fn test_impl(input: TestCase) {
            let bytes = input.bytes.decode_hex().unwrap();
            let sign = ecdsa::secp256k1::VerifySignature::from_raw_or_der_bytes(&bytes).unwrap();
            assert_eq!(sign.to_bytes().to_string(), input.expected);
        }

        // From DER 70 bytes.
        test_impl(TestCase {
            bytes: "304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a40",
            expected: "1857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b3490a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a40",
        });
        // From DER 71 bytes.
        test_impl(TestCase {
            bytes: "3045022100b2b31575f8536b284410d01217f688be3a9faf4ba0ba3a9093f983e40d630ec7022022a7a25b01403cff0d00b3b853d230f8e96ff832b15d4ccc75203cb65896a2d5",
            expected: "b2b31575f8536b284410d01217f688be3a9faf4ba0ba3a9093f983e40d630ec722a7a25b01403cff0d00b3b853d230f8e96ff832b15d4ccc75203cb65896a2d5",
        });
        // From DER 72 bytes.
        test_impl(TestCase {
            bytes: "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1",
            expected: "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1",
        });
        // From ECDSA 64 bytes.
        test_impl(TestCase {
            bytes: "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1",
            expected: "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1",
        });
        // From ECDSA 65 bytes.
        test_impl(TestCase {
            bytes: "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901",
            expected: "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de9",
        });
    }

    #[test]
    fn test_verify_signature_from_raw_or_der_bytes_error() {
        // 63 byte array is not a valid verify signature.
        let bytes = "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254d".decode_hex().unwrap();
        assert!(
            ecdsa::secp256k1::VerifySignature::from_raw_or_der_bytes(&bytes).is_err(),
            "Expected an error"
        );
    }
}
