// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_encoding::hex::FromHexError;
use tw_hash::ffi::{
    blake2_b, blake2_b_personal, blake_256, groestl_512, hmac__sha256, keccak256, keccak512,
    ripemd_160, sha1, sha256, sha3__256, sha3__512, sha512, sha512_256, CHashingCode,
};
use tw_hash::Error;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_result::ErrorCode;

type ExternFn = unsafe extern "C" fn(*const u8, usize) -> CByteArray;

#[track_caller]
pub fn test_hash_helper(hash: ExternFn, input: &[u8], expected: &str) {
    let decoded = unsafe { hash(input.as_ptr(), input.len()).into_vec() };
    assert_eq!(hex::encode(decoded, false), expected);
}

#[test]
fn test_blake2b() {
    const HASH_SIZE: usize = 64;

    /// Declare a `blake2_b` helper that forwards `HASH_SIZE`.
    unsafe extern "C" fn blake2_b_hash(input: *const u8, input_len: usize) -> CByteArray {
        blake2_b(input, input_len, HASH_SIZE).unwrap()
    }

    test_hash_helper(blake2_b_hash, b"Hello world", "6ff843ba685842aa82031d3f53c48b66326df7639a63d128974c5c14f31a0f33343a8c65551134ed1ae0f2b0dd2bb495dc81039e3eeb0aa1bb0388bbeac29183");
    test_hash_helper(blake2_b_hash,b"Hello world",  "6ff843ba685842aa82031d3f53c48b66326df7639a63d128974c5c14f31a0f33343a8c65551134ed1ae0f2b0dd2bb495dc81039e3eeb0aa1bb0388bbeac29183");
    test_hash_helper(blake2_b_hash,b"The quick brown fox jumps over the lazy dog", "a8add4bdddfd93e4877d2746e62817b116364a1fa7bc148d95090bc7333b3673f82401cf7aa2e4cb1ecd90296e3f14cb5413f8ed77be73045b13914cdcd6a918");
}

#[test]
fn test_blake2b_personal() {
    let personal_data = b"MyApp Files Hash";
    let content = b"the same content";
    let actual = unsafe {
        blake2_b_personal(
            content.as_ptr(),
            content.len(),
            32,
            personal_data.as_ptr(),
            personal_data.len(),
        )
        .unwrap()
        .into_vec()
    };
    let expected = "20d9cd024d4fb086aae819a1432dd2466de12947831b75c5a30cf2676095d3b4";
    assert_eq!(hex::encode(actual, false), expected);
}

#[test]
fn test_blake_256() {
    test_hash_helper(
        blake_256,
        b"hello world",
        "4a6f552c9a404582b4eeff4364df2d5a0da9e3f7635c7b638bacab9f5d09bdb1",
    );
    test_hash_helper(
        blake_256,
        b"The quick brown fox jumps over the lazy dog",
        "7576698ee9cad30173080678e5965916adbb11cb5245d386bf1ffda1cb26c9d7",
    );
}

#[test]
fn test_groestl_512() {
    test_hash_helper(groestl_512,b"hello world", "0059e9c916018f0e5efbe6e9376f7562203eb4b2c9267ba30353f1f0cfd4a950e7ae68d9e7cda002432c7f3efdf2a4c0cd673ddb9109e7d846538414d4a72c36");
    test_hash_helper(groestl_512,b"The quick brown fox jumps over the lazy dog", "badc1f70ccd69e0cf3760c3f93884289da84ec13c70b3d12a53a7a8a4a513f99715d46288f55e1dbf926e6d084a0538e4eebfc91cf2b21452921ccde9131718d");
}

#[test]
fn test_hmac_sha256() {
    let key =
        hex::decode("531cbfcf12a168faff61af28bf437377397b4bf435ee732cf4ac95761a651f14").unwrap();
    let key = key.as_slice();
    let data =
        hex::decode("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224d").unwrap();
    let data = data.as_slice();
    let actual =
        unsafe { hmac__sha256(key.as_ptr(), key.len(), data.as_ptr(), data.len()).into_vec() };
    let expected = "a7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab".to_string();
    assert_eq!(hex::encode(actual, false), expected);
}

#[test]
fn test_ripemd160() {
    test_hash_helper(
        ripemd_160,
        b"hello world",
        "98c615784ccb5fe5936fbc0cbe9dfdb408d92f0f",
    );
    test_hash_helper(
        ripemd_160,
        b"The quick brown fox jumps over the lazy dog",
        "37f332f68db77bd9d7edd4969571ad671cf9dd3b",
    );
}

#[test]
fn test_sha1() {
    test_hash_helper(sha1, b"", "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    test_hash_helper(
        sha1,
        b"The quick brown fox jumps over the lazy dog",
        "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12",
    );
    test_hash_helper(
        sha1,
        b"The quick brown fox jumps over the lazy dog.",
        "408d94384216f890ff7a0c3528e8bed1e0b01621",
    );
}

#[test]
fn test_sha256() {
    test_hash_helper(
        sha256,
        b"hello world",
        "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9",
    );
    test_hash_helper(
        sha256,
        b"The quick brown fox jumps over the lazy dog",
        "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592",
    );
}

#[test]
fn test_sha512() {
    test_hash_helper(
        sha512,
        b"hello world",
        "309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee511a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670da34255b45b0cfd830e81f605dcf7dc5542e93ae9cd76f",
    );
    test_hash_helper(
        sha512,
        b"The quick brown fox jumps over the lazy dog",
        "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6",
    );
}

#[test]
fn test_sha512_256() {
    test_hash_helper(
        sha512_256,
        b"hello world",
        "0ac561fac838104e3f2e4ad107b4bee3e938bf15f2b15f009ccccd61a913f017",
    );
    test_hash_helper(
        sha512_256,
        b"The quick brown fox jumps over the lazy dog",
        "dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d",
    );
}

#[test]
fn test_keccak256() {
    test_hash_helper(
        keccak256,
        b"hello world",
        "47173285a8d7341e5e972fc677286384f802f8ef42a5ec5f03bbfa254cb01fad",
    );
    test_hash_helper(
        keccak256,
        b"The quick brown fox jumps over the lazy dog",
        "4d741b6f1eb29cb2a9b9911c82f56fa8d73b04959d3d9d222895df6c0b28aa15",
    );
}

#[test]
fn test_keccak512() {
    test_hash_helper(
        keccak512,
        b"hello world",
        "3ee2b40047b8060f68c67242175660f4174d0af5c01d47168ec20ed619b0b7c42181f40aa1046f39e2ef9efc6910782a998e0013d172458957957fac9405b67d",
    );
    test_hash_helper(
        keccak512,
        b"The quick brown fox jumps over the lazy dog",
        "d135bb84d0439dbac432247ee573a23ea7d3c9deb2a968eb31d47c4fb45f1ef4422d6c531b5b9bd6f449ebcc449ea94d0a8f05f62130fda612da53c79659f609",
    );
}

#[test]
fn test_sha3_256() {
    test_hash_helper(
        sha3__256,
        b"hello world",
        "644bcc7e564373040999aac89e7622f3ca71fba1d972fd94a31c3bfbf24e3938",
    );
    test_hash_helper(
        sha3__256,
        b"The quick brown fox jumps over the lazy dog",
        "69070dda01975c8c120c3aada1b282394e7f032fa9cf32f4cb2259a0897dfc04",
    );
}

#[test]
fn test_sha3_512() {
    test_hash_helper(
        sha3__512,
        b"hello world",
        "840006653e9ac9e95117a15c915caab81662918e925de9e004f774ff82d7079a40d4d27b1b372657c61d46d470304c88c788b3a4527ad074d1dccbee5dbaa99a",
    );
    test_hash_helper(
        sha3__512,
        b"The quick brown fox jumps over the lazy dog",
        "01dedd5de4ef14642445ba5f5b97c15e47b9ad931326e4b0727cd94cefc44fff23f07bf543139939b49128caf436dc1bdee54fcb24023a08d9403f9b4bf0d450",
    );
}

#[test]
fn test_c_hashing_error_convert() {
    assert_eq!(ErrorCode::from(CHashingCode::Ok), 0);
    assert_eq!(ErrorCode::from(CHashingCode::InvalidHashLength), 1);
    assert_eq!(ErrorCode::from(CHashingCode::InvalidArgument), 2);

    assert_eq!(
        CHashingCode::InvalidArgument,
        Error::FromHexError(FromHexError::OddLength).into(),
    );
    assert_eq!(CHashingCode::InvalidArgument, Error::InvalidArgument.into());
    assert_eq!(
        CHashingCode::InvalidHashLength,
        Error::InvalidHashLength.into(),
    );
}
