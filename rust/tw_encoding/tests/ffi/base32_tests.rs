use std::ffi::CString;
use tw_encoding::ffi::{decode_base32, encode_base32};

/// Checks if the encoded `input` with the given `alphabet` and `padding` parameters
/// equals to `expected`.
#[track_caller]
fn test_base32_encode_ffi_helper(
    input: &[u8],
    expected: &str,
    alphabet: Option<&str>,
    padding: bool,
) {
    let alphabet = alphabet
        .map(|alphabet| CString::new(alphabet).unwrap().into_raw())
        .unwrap_or_else(std::ptr::null_mut);

    let result_ptr = encode_base32(input.as_ptr(), input.len(), alphabet, padding);
    let result = unsafe { CString::from_raw(result_ptr) };
    assert_eq!(result.to_str().unwrap(), expected);
}

/// Checks if the decoded `input` with the given `alphabet` and `padding` parameters
/// equals to `expected`.
#[track_caller]
fn test_base32_decode_ffi_helper(
    input: &str,
    expected: &[u8],
    alphabet: Option<&str>,
    padding: bool,
) {
    let input = CString::new(input).unwrap();
    let alphabet = alphabet
        .map(|alphabet| CString::new(alphabet).unwrap().into_raw())
        .unwrap_or_else(std::ptr::null_mut);

    let decoded_ptr = decode_base32(input.as_ptr(), alphabet, padding);
    let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };
    assert_eq!(decoded_slice, expected);
}

#[test]
fn test_base32_encode_ffi() {
    let input = b"Hello, world!";
    let alphabet = "abcdefghijklmnopqrstuvwxyz234567";

    test_base32_encode_ffi_helper(input, "JBSWY3DPFQQHO33SNRSCC===", None, true);
    test_base32_encode_ffi_helper(input, "JBSWY3DPFQQHO33SNRSCC", None, false);
    test_base32_encode_ffi_helper(input, "jbswy3dpfqqho33snrscc===", Some(alphabet), true);
    test_base32_encode_ffi_helper(input, "jbswy3dpfqqho33snrscc", Some(alphabet), false);
}

#[test]
fn test_base32_decode_ffi() {
    let expected = b"Hello, world!";
    let alphabet = "abcdefghijklmnopqrstuvwxyz234567";

    test_base32_decode_ffi_helper("JBSWY3DPFQQHO33SNRSCC===", expected, None, true);
    test_base32_decode_ffi_helper("JBSWY3DPFQQHO33SNRSCC", expected, None, false);
    test_base32_decode_ffi_helper("jbswy3dpfqqho33snrscc===", expected, Some(alphabet), true);
    test_base32_decode_ffi_helper("jbswy3dpfqqho33snrscc", expected, Some(alphabet), false);
}
