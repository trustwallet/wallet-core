use std::ffi::{c_char, CString};
use tw_memory::c_string_standalone;
use tw_memory::ffi::c_result::OK_CODE;
use tw_memory::ffi::free_string;
use tw_starknet::ffi::{
    starknet_pubkey_from_private, starknet_sign, starknet_verify, CStarknetCode,
};

#[test]
fn test_starknet_pubkey_from_private() {
    let priv_key_hex =
        c_string_standalone("058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe");
    let pub_key_raw = unsafe { starknet_pubkey_from_private(priv_key_hex) };
    assert_eq!(pub_key_raw.code, OK_CODE);
    let actual = unsafe {
        CString::from_raw(pub_key_raw.result as *mut c_char)
            .into_string()
            .unwrap()
    };
    assert_eq!(
        actual,
        "0x2a4c7332c55d6c1c510d24272d1db82878f2302f05b53bcc38695ed5f78fffd"
    );

    unsafe { free_string(priv_key_hex) };
}

#[test]
fn test_starknet_pubkey_from_private_invalid() {
    let priv_key_raw =
        c_string_standalone("058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afx");
    let pub_key_raw = unsafe { starknet_pubkey_from_private(priv_key_raw) };
    assert_eq!(pub_key_raw.code, CStarknetCode::PrivKeyError as i32);
    assert!(pub_key_raw.result.is_null());

    // Non-null terminated string.
    let invalid_data = vec![159, 146, 150];
    let invalid_priv_key_ptr = invalid_data.as_ptr() as *const c_char;

    let pub_key_raw = unsafe { starknet_pubkey_from_private(invalid_priv_key_ptr) };
    assert_eq!(pub_key_raw.code, CStarknetCode::InvalidInput as i32);
    assert!(pub_key_raw.result.is_null());

    unsafe { free_string(priv_key_raw) };
}

#[test]
fn test_starknet_sign() {
    let priv_key_raw =
        c_string_standalone("0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79");
    let digest_raw =
        c_string_standalone("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");

    let signature = unsafe { starknet_sign(priv_key_raw, digest_raw) };
    assert_eq!(signature.code, OK_CODE);
    let actual = unsafe { CString::from_raw(signature.result.cast_mut()) }
        .into_string()
        .unwrap();
    let expected = "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a";
    assert_eq!(actual, expected);

    unsafe { free_string(priv_key_raw) };
    unsafe { free_string(digest_raw) };
}

#[test]
fn test_starknet_verify() {
    let pubkey_raw =
        c_string_standalone("02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f83159");
    let hash_raw =
        c_string_standalone("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");
    let signature_r_raw =
        c_string_standalone("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f");
    let signature_s_raw =
        c_string_standalone("04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a");

    let res = unsafe { starknet_verify(pubkey_raw, hash_raw, signature_r_raw, signature_s_raw) };
    assert_eq!(res.code, OK_CODE);
    assert!(res.result);

    unsafe { free_string(pubkey_raw) };
    unsafe { free_string(hash_raw) };
    unsafe { free_string(signature_r_raw) };
    unsafe { free_string(signature_s_raw) };
}

#[test]
fn test_starknet_verify_fail() {
    let pubkey_raw =
        c_string_standalone("02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f83159");
    let hash_raw =
        c_string_standalone("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");
    let signature_r_raw =
        c_string_standalone("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f");
    let signature_s_raw =
        c_string_standalone("04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9b");

    let res = unsafe { starknet_verify(pubkey_raw, hash_raw, signature_r_raw, signature_s_raw) };
    assert_eq!(res.code, OK_CODE);
    assert!(!res.result);

    unsafe { free_string(pubkey_raw) };
    unsafe { free_string(hash_raw) };
    unsafe { free_string(signature_r_raw) };
    unsafe { free_string(signature_s_raw) };
}

#[test]
fn test_starknet_private_key_invalid() {
    let private_raw =
        c_string_standalone("02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f831590123");

    let res = unsafe { starknet_pubkey_from_private(private_raw) };
    assert_eq!(res.code, CStarknetCode::PrivKeyError as i32);

    unsafe { free_string(private_raw) };
}

#[test]
fn test_starknet_private_key_zero() {
    let private_raw =
        c_string_standalone("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    let res = unsafe { starknet_pubkey_from_private(private_raw) };
    assert_eq!(res.code, CStarknetCode::PrivKeyError as i32);

    unsafe { free_string(private_raw) };
}
