// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::Error;
use k256::ecdh::{diffie_hellman, SharedSecret};
use k256::ecdsa::signature::hazmat::PrehashVerifier;
use k256::ecdsa::{SigningKey, VerifyingKey};
use k256::elliptic_curve::sec1::Tag;
use k256::{AffinePoint, EncodedPoint, FieldBytes};
use std::ops::{Range, RangeInclusive};
use tw_encoding::hex;
use tw_hash::{H256, H264, H520};

const R_RANGE: Range<usize> = 0..32;
const S_RANGE: Range<usize> = 32..64;
const RECOVERY_LAST: usize = Signature::len() - 1;
/// Expected signature with or without recovery byte in the end of the slice.
const VERIFY_SIGNATURE_LEN_RANGE: RangeInclusive<usize> = 64..=65;

#[derive(Debug, PartialEq)]
pub struct Signature {
    signature: k256::ecdsa::Signature,
    v: u8,
}

impl Signature {
    pub const fn len() -> usize {
        65
    }

    pub fn r(&self) -> H256 {
        let (r, _s) = self.signature.split_bytes();
        H256::try_from(r.as_slice()).expect("Expected 'r' 32 byte length array")
    }

    pub fn s(&self) -> H256 {
        let (_, s) = self.signature.split_bytes();
        H256::try_from(s.as_slice()).expect("Expected 's' 32 byte length array")
    }

    pub fn v(&self) -> u8 {
        self.v
    }

    pub fn from_bytes(sig: &[u8]) -> Result<Signature, Error> {
        if sig.len() != Signature::len() {
            return Err(Error::InvalidSignature);
        }

        Ok(Signature {
            signature: Self::signature_from_slices(&sig[R_RANGE], &sig[S_RANGE])?,
            v: sig[RECOVERY_LAST],
        })
    }

    // pub fn from_vrs_bytes(sig: &[u8]) -> Result<Signature, Error> {
    //     if sig.len() != Signature::len() {
    //         return Err(Error::InvalidSignature);
    //     }
    //
    //     let shifted = &sig[1..];
    //     Ok(Signature {
    //         r: H256::try_from(&shifted[R_RANGE]).expect("Expected valid R_RANGE"),
    //         s: H256::try_from(&shifted[S_RANGE]).expect("Expected valid S_RANGE"),
    //         v: sig[0],
    //     })
    // }

    /// Returns a standard binary signature representation:
    /// RSV, where R - 32 byte array, S - 32 byte array, V - 1 byte.
    pub fn to_bytes(&self) -> H520 {
        let (r, s) = self.signature.split_bytes();

        let mut dest = H520::default();
        dest[R_RANGE].copy_from_slice(r.as_slice());
        dest[S_RANGE].copy_from_slice(s.as_slice());
        dest[RECOVERY_LAST] = self.v;
        dest
    }

    // /// Returns a VRS signature representation,
    // /// where V - 1 byte, R - 32 byte array, S - 32 byte array.
    // pub fn to_vrs_bytes(&self) -> H520 {
    //     let mut dest = H520::default();
    //     dest[0] = self.v;
    //
    //     let shifted = &mut dest[1..];
    //     shifted[R_RANGE].copy_from_slice(&self.r);
    //     shifted[S_RANGE].copy_from_slice(&self.s);
    //
    //     dest
    // }

    /// # Panic
    ///
    /// `r` and `s` must be 32 byte arrays, otherwise the function panics.
    fn signature_from_slices(r: &[u8], s: &[u8]) -> Result<k256::ecdsa::Signature, Error> {
        let r = FieldBytes::clone_from_slice(r);
        let s = FieldBytes::clone_from_slice(s);

        k256::ecdsa::Signature::from_scalars(r, s).map_err(|_| Error::InvalidSignature)
    }
}

/// To verify the signature, it's enough to check `r` and `s` parts without the recovery ID.
pub struct VerifySignature {
    pub signature: k256::ecdsa::Signature,
}

impl VerifySignature {
    pub fn from_bytes(sig: &[u8]) -> Result<Self, Error> {
        if !VERIFY_SIGNATURE_LEN_RANGE.contains(&sig.len()) {
            return Err(Error::InvalidSignature);
        }

        Ok(VerifySignature {
            signature: Signature::signature_from_slices(&sig[R_RANGE], &sig[S_RANGE])?,
        })
    }

    // pub fn from_vrs_bytes(sig: &[u8]) -> Result<Self, Error> {
    //     if sig.len() != Signature::len() {
    //         return Err(Error::InvalidSignature);
    //     }
    //
    //     let shifted = &sig[1..];
    //     VerifySignature::from_bytes(shifted)
    // }
}

impl From<Signature> for VerifySignature {
    fn from(sig: Signature) -> Self {
        VerifySignature {
            signature: sig.signature,
        }
    }
}

#[derive(Debug)]
pub struct PrivateKey {
    secret: SigningKey,
}

impl PrivateKey {
    pub fn to_bytes(&self) -> H256 {
        H256::try_from(self.secret.to_bytes().as_slice())
            .expect("'PrivateKey::secret' is 32 byte length array")
    }

    pub fn sign(&self, hash: H256) -> Result<Signature, Error> {
        let (signature, recovery_id) = self
            .secret
            .sign_prehash_recoverable(&hash)
            .map_err(|_| Error::SigningError)?;
        Ok(Signature {
            signature,
            v: recovery_id.to_byte(),
        })
    }

    pub fn public(&self) -> PublicKey {
        PublicKey {
            public: *self.secret.verifying_key(),
        }
    }

    pub fn shared_key_hash(&self, public: &PublicKey) -> H256 {
        let public_raw = public.compressed();
        let public_point = EncodedPoint::from_bytes(public_raw.as_slice())
            .expect("Expected valid 'k256::EncodedPoint'");
        let public_affine =
            AffinePoint::try_from(public_point).expect("Expected valid 'AffinePoint'");

        let secret: SharedSecret = diffie_hellman(self.secret.as_nonzero_scalar(), &public_affine);

        // `SharedSecret` is 32 byte array. We need to push the `compress` tag front.
        let mut secret_tagged = Vec::with_capacity(33);
        secret_tagged.push(Tag::CompressedEvenY as u8);
        secret_tagged.extend_from_slice(secret.raw_secret_bytes().as_slice());

        let secret_hash = tw_hash::sha2::sha256(&secret_tagged);
        // TODO refactor `sha256` to return `H256`.
        H256::try_from(secret_hash.as_slice()).expect("Expected 32 byte array sha256 hash")
    }
}

/// Implement `str` -> `PrivateKey` conversion for test purposes.
impl From<&'static str> for PrivateKey {
    fn from(hex: &'static str) -> Self {
        let data = H256::from(hex);
        PrivateKey::try_from(data.as_slice()).expect("Expected valid Private Key hex")
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = Error;

    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        let secret = SigningKey::from_slice(data).map_err(|_| Error::InvalidSecretKey)?;
        Ok(PrivateKey { secret })
    }
}

#[derive(Debug)]
pub struct PublicKey {
    public: VerifyingKey,
}

impl PublicKey {
    pub fn compressed(&self) -> H264 {
        let compressed = true;
        H264::try_from(self.public.to_encoded_point(compressed).as_bytes())
            .expect("Expected 33 byte array Public Key")
    }

    pub fn uncompressed(&self) -> H520 {
        let compressed = false;
        H520::try_from(self.public.to_encoded_point(compressed).as_bytes())
            .expect("Expected 65 byte array Public Key")
    }

    pub fn verify(&self, sig: VerifySignature, hash: H256) -> bool {
        self.public.verify_prehash(&hash, &sig.signature).is_ok()
    }
}

impl From<&'static str> for PublicKey {
    fn from(hex: &'static str) -> Self {
        // Expected either `H264` or `H520`.
        let data = hex::decode(hex).expect("Expected valid hex");
        PublicKey::try_from(data.as_slice()).expect("Expected valid Public Key hex")
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = Error;

    /// Expected either `H264` or `H520` slice.
    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        Ok(PublicKey {
            public: VerifyingKey::from_sec1_bytes(data).map_err(|_| Error::InvalidPublicKey)?,
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex;
    use tw_hash::sha3::keccak256;

    #[test]
    fn test_private_key_from() {
        let hex = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let expected = hex::decode(hex).unwrap();

        // Test `From<&'static str>`.
        let private = PrivateKey::from(hex);
        assert_eq!(private.to_bytes().as_slice(), expected);

        // Test `From<&'a [u8]>`.
        let private = PrivateKey::try_from(expected.as_slice()).unwrap();
        assert_eq!(private.to_bytes().as_slice(), expected);
    }

    #[test]
    fn test_private_key_sign_verify() {
        let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let private = PrivateKey::from(secret);
        let public = private.public();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();
        let signature = private.sign(hash_to_sign).unwrap();

        let expected = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
        assert_eq!(signature.to_bytes(), expected);

        let verify_signature = VerifySignature::from(signature);
        assert!(public.verify(verify_signature, hash_to_sign));
    }

    #[test]
    fn test_verify_invalid() {
        let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let private = PrivateKey::from(secret);

        let signature_bytes  = H520::from("375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f0801");
        let verify_sig = VerifySignature::from_bytes(&signature_bytes).unwrap();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();

        assert!(!private.public().verify(verify_sig, hash_to_sign));
    }

    #[test]
    fn test_shared_key_hash() {
        let private =
            PrivateKey::from("9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0");
        let public =
            PublicKey::from("02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992");
        let actual = private.shared_key_hash(&public);
        let expected =
            H256::from("ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a");
        assert_eq!(actual, expected);
    }
}

#[cfg(test)]
mod tests_todo {
    use super::*;
    use k256::ecdh::{diffie_hellman, SharedSecret};
    use k256::ecdsa::hazmat::{bits2field, DigestPrimitive};
    use k256::ecdsa::signature::hazmat::PrehashVerifier;
    use k256::ecdsa::{RecoveryId, VerifyingKey};
    use k256::elliptic_curve::generic_array::ArrayLength;
    use k256::elliptic_curve::subtle::{Choice, ConditionallySelectable, ConstantTimeEq};
    use k256::elliptic_curve::{Curve, FieldBytesEncoding, PrimeField};
    use k256::sha2::digest::core_api::BlockSizeUser;
    use k256::sha2::digest::{FixedOutput, FixedOutputReset};
    use k256::{AffinePoint, EncodedPoint, Scalar, Secp256k1};
    use rfc6979::{ByteArray, HmacDrbg};
    use tw_encoding::hex;

    #[test]
    fn test_sign_and_verify() {
        let priv_key = tw_encoding::hex::decode(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let signing_key = SigningKey::from_slice(&priv_key).unwrap();

        let msg = b"hello";

        let hash_to_sign = tw_hash::sha3::keccak256(msg);
        let (signature, recovery_id) = signing_key.sign_prehash_recoverable(&hash_to_sign).unwrap();
        // let hash_to_sign = Keccak256::new_with_prefix(msg);
        // let (signature, recovery_id): (KSignature, _) =
        //     signing_key.sign_digest_recoverable(hash_to_sign).unwrap();

        let expected = tw_encoding::hex::decode("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de9").unwrap();
        assert_eq!(signature.to_vec(), expected);
        assert_eq!(recovery_id, RecoveryId::from_byte(1).unwrap());

        let verifying = signing_key.verifying_key();
        let public_key = verifying.to_encoded_point(true);

        let exp = tw_encoding::hex::decode(
            "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1",
        )
        .unwrap();
        assert_eq!(public_key.as_bytes(), exp);

        verifying.verify_prehash(&hash_to_sign, &signature).unwrap();

        // Create PublicKey from bytes.
        let vv = VerifyingKey::from_sec1_bytes(public_key.as_bytes()).unwrap();
        vv.verify_prehash(&hash_to_sign, &signature).unwrap();
    }

    // #[test]
    // fn test_canonical() {
    //     let priv_key = tw_encoding::hex::decode(
    //         "ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035",
    //     )
    //     .unwrap();
    //     let signing_key = SigningKey::from_slice(&priv_key).unwrap();
    //
    //     let hash_to_sign = [
    //         113, 183, 9, 142, 129, 80, 205, 233, 15, 62, 192, 2, 128, 129, 93, 48, 105, 248, 28,
    //         124, 219, 109, 131, 187, 226, 184, 151, 177, 175, 190, 124, 214,
    //     ];
    //
    //     let z = bits2field::<Secp256k1>(hash_to_sign.as_slice()).unwrap();
    //     let priv_scalar = signing_key.as_nonzero_scalar();
    //
    //     let k = rfc6979::generate_k::<<Secp256k1 as DigestPrimitive>::Digest, _>(
    //         &priv_scalar.to_repr(),
    //         &Secp256k1::ORDER.encode_field_bytes(),
    //         &z,
    //         &[],
    //     );
    //     println!("Given k(0): {:?}", k.as_slice());
    //     let invert_k = Scalar::from_repr(k).unwrap();
    //     println!("Given invert(k(0)): {:?}", invert_k.to_bytes().as_slice());
    //
    //     // let (signature, recovery_id) = signing_key.sign_prehash_recoverable(&hash_to_sign).unwrap();
    //
    //     let x = &priv_scalar.to_repr();
    //     let n = &Secp256k1::ORDER.encode_field_bytes();
    //     let h = &z;
    //     let mut hmac_drbg = HmacDrbg::<<Secp256k1 as DigestPrimitive>::Digest>::new(x, h, &[]);
    //
    //     fn ct_eq<N: ArrayLength<u8>>(a: &ByteArray<N>, b: &ByteArray<N>) -> Choice {
    //         let mut ret = Choice::from(1);
    //
    //         for (a, b) in a.iter().zip(b.iter()) {
    //             ret.conditional_assign(&Choice::from(0), !a.ct_eq(b));
    //         }
    //
    //         ret
    //     }
    //
    //     pub(crate) fn ct_lt<N: ArrayLength<u8>>(a: &ByteArray<N>, b: &ByteArray<N>) -> Choice {
    //         let mut borrow = 0;
    //
    //         // Perform subtraction with borrow a byte-at-a-time, interpreting a
    //         // no-borrow condition as the less-than case
    //         for (&a, &b) in a.iter().zip(b.iter()).rev() {
    //             let c = (b as u16).wrapping_add(borrow >> (u8::BITS - 1));
    //             borrow = (a as u16).wrapping_sub(c) >> u8::BITS as u8;
    //         }
    //
    //         !borrow.ct_eq(&0)
    //     }
    //
    //     pub(crate) fn generate_k<D, N>(
    //         hmac_drbg: &mut HmacDrbg<D>,
    //         n: &ByteArray<N>,
    //     ) -> ByteArray<N>
    //     where
    //         D: Digest + BlockSizeUser + FixedOutput<OutputSize = N> + FixedOutputReset,
    //         N: ArrayLength<u8>,
    //     {
    //         loop {
    //             let mut k = ByteArray::<N>::default();
    //             hmac_drbg.fill_bytes(&mut k);
    //
    //             let k_is_zero = ct_eq(&k, &ByteArray::default());
    //             if (!k_is_zero & ct_lt(&k, n)).into() {
    //                 return k;
    //             }
    //         }
    //     }
    //
    //     let first_k = generate_k(&mut hmac_drbg, n);
    //     let second_k = generate_k(&mut hmac_drbg, n);
    //
    //     println!("k(0): {:?}", first_k.as_slice());
    //     println!("k(1): {:?}", second_k.as_slice());
    //
    //     // Hash:
    //     // 113, 183, 9, 142, 129, 80, 205, 233, 15, 62, 192, 2, 128, 129, 93, 48, 105, 248, 28, 124, 219, 109, 131, 187, 226, 184, 151, 177, 175, 190, 124, 214,
    //     // rng.idig=1424193935 rng.odig=-1755635692 rng.v=-694351588
    //     // k[0]=342935452 k[2]=251653400 k[4]=442959146 k[6]=178128301 k[8]=9682733
    //     // isCanonical? 0
    //     // k[0]=485786034 k[2]=1216029 k[4]=84607918 k[6]=152435080 k[8]=11446431
    //     // isCanonical? 1
    //
    //     // generate_k_rfc6979: 147, 191, 45, 214, 78, 238, 178, 167, 129, 107, 111, 86, 20, 87, 166, 112, 82, 174, 162, 141, 202, 187, 255, 180, 96, 32, 116, 199, 84, 112, 199, 156,
    //     // isCanonical? 0
    //     // generate_k_rfc6979: 174, 168, 159, 148, 195, 158, 178, 69, 126, 98, 26, 117, 176, 156, 80, 176, 58, 228, 75, 242, 69, 0, 74, 56, 119, 220, 105, 246, 28, 244, 129, 178,
    //     // isCanonical? 1
    // }

    #[test]
    fn test_public_key_extended() {
        let pb = tw_encoding::hex::decode("04a07c4250acb599a1bc4c895ddd3e91ac0c773a5b63ca0e637816d2dc8e857b7abae5478d0646d394a21e712baca3df6234be35b798cc8de22b898484e0098140").unwrap();
        let pk = VerifyingKey::from_sec1_bytes(&pb).unwrap();

        let compressed = tw_encoding::hex::decode(
            "02a07c4250acb599a1bc4c895ddd3e91ac0c773a5b63ca0e637816d2dc8e857b7a",
        )
        .unwrap();
        assert_eq!(pk.to_encoded_point(true).as_bytes(), compressed);

        let short = VerifyingKey::from_sec1_bytes(pk.to_encoded_point(true).as_bytes()).unwrap();
        assert_eq!(short.to_encoded_point(false).as_bytes(), pb);
    }

    #[test]
    fn test_shared_key() {
        let priv_key = tw_encoding::hex::decode(
            "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0",
        )
        .unwrap();
        let signing_key = SigningKey::from_slice(&priv_key).unwrap();

        let public_key_raw = tw_encoding::hex::decode(
            "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992",
        )
        .unwrap();
        let _public_key = VerifyingKey::from_sec1_bytes(&public_key_raw).unwrap();
        let x = EncodedPoint::from_bytes(&public_key_raw).unwrap();
        let y = AffinePoint::try_from(x).unwrap();

        let secret: SharedSecret = diffie_hellman(signing_key.as_nonzero_scalar(), &y);
        println!(
            "{}",
            hex::encode(secret.raw_secret_bytes().as_slice(), false)
        );
        let mut secret_tagged = Vec::with_capacity(33);
        secret_tagged.push(2);
        secret_tagged.extend_from_slice(secret.raw_secret_bytes().as_slice());
        // let secret_array = GenericArray::from_slice(secret.raw_secret_bytes().as_slice());
        // let secret_point = EncodedPoint::from_untagged_bytes(secret.raw_secret_bytes());
        // let xxxxxx: k256::PublicKey = k256::PublicKey::from_encoded_point(&);
        let secret_hash = tw_hash::sha2::sha256(&secret_tagged);
        let expected_secret =
            hex!("ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a");
        assert_eq!(secret_hash, expected_secret);

        // actual: a07c4250acb599a1bc4c895ddd3e91ac0c773a5b63ca0e637816d2dc8e857b7a
        // sharedKeyExtended: 04a07c4250acb599a1bc4c895ddd3e91ac0c773a5b63ca0e637816d2dc8e857b7abae5478d0646d394a21e712baca3df6234be35b798cc8de22b898484e0098140
        // sharedKey.compressed(): 02a07c4250acb599a1bc4c895ddd3e91ac0c773a5b63ca0e637816d2dc8e857b7a
    }

    #[test]
    fn test_get_shared_key_wycherproof() {
        let priv_key = tw_encoding::hex::decode(
            "f4b7ff7cccc98813a69fae3df222bfe3f4e28f764bf91b4a10d8096ce446b254",
        )
        .unwrap();
        let signing_key = SigningKey::from_slice(&priv_key).unwrap();

        let public_key_raw = tw_encoding::hex::decode(
            "02d8096af8a11e0b80037e1ee68246b5dcbb0aeb1cf1244fd767db80f3fa27da2b",
        )
        .unwrap();
        let public_key_point = EncodedPoint::from_bytes(&public_key_raw).unwrap();
        let public_key_affine_point = AffinePoint::try_from(public_key_point).unwrap();

        let secret: SharedSecret =
            diffie_hellman(signing_key.as_nonzero_scalar(), &public_key_affine_point);

        let mut secret_tagged = Vec::with_capacity(33);
        secret_tagged.push(2);
        secret_tagged.extend_from_slice(secret.raw_secret_bytes().as_slice());

        // expected: 02544dfae22af6af939042b1d85b71a1e49e9a5614123c4d6ad0c8af65baf87d65
        println!("{}", tw_encoding::hex::encode(&secret_tagged, false));

        let secret_hash = tw_hash::sha2::sha256(&secret_tagged);
        let expected = hex!("81165066322732362ca5d3f0991d7f1f7d0aad7ea533276496785d369e35159a");
        assert_eq!(secret_hash, expected);
    }
}
