use tw_memory::ffi::{c_byte_array::CByteArray, free_string};

/// Test `pass_eth_signing_msg_through` to avoid dropping code coverage.
#[test]
fn test_pass_eth_signing_msg_through() {
    let serialized = vec![
        10, 1, 1, 18, 1, 0, 34, 5, 9, 199, 101, 36, 0, 42, 3, 1, 48, 185, 66, 42, 48, 120, 54, 98,
        49, 55, 53, 52, 55, 52, 101, 56, 57, 48, 57, 52, 99, 52, 52, 100, 97, 57, 56, 98, 57, 53,
        52, 101, 101, 100, 101, 97, 99, 52, 57, 53, 50, 55, 49, 100, 48, 102, 74, 32, 96, 141, 203,
        23, 66, 187, 63, 183, 174, 192, 2, 7, 78, 52, 32, 228, 250, 183, 208, 12, 206, 215, 156,
        205, 172, 83, 237, 91, 39, 19, 129, 81, 82, 56, 18, 54, 10, 42, 48, 120, 53, 51, 50, 50,
        98, 51, 52, 99, 56, 56, 101, 100, 48, 54, 57, 49, 57, 55, 49, 98, 102, 53, 50, 97, 55, 48,
        52, 55, 52, 52, 56, 102, 48, 102, 52, 101, 102, 99, 56, 52, 18, 8, 27, 193, 109, 103, 78,
        200, 0, 0,
    ];
    let actual = unsafe {
        let array = CByteArray::new(serialized.clone());
        tw_proto::ffi::pass_eth_signing_msg_through(array.data(), array.size())
            .unwrap()
            .into_vec()
    };
    assert_eq!(actual, serialized);
}

/// Test `polkadot_test_signing_input` to avoid dropping code coverage.
#[test]
fn test_polkadot_test_signing_input() {
    unsafe { tw_proto::ffi::polkadot_test_signing_input().unwrap() };
}

/// Test `polkadot_tx_expected_encoded` to avoid dropping code coverage.
#[test]
fn polkadot_tx_expected_encoded() {
    unsafe {
        let str = tw_proto::ffi::polkadot_tx_expected_encoded();
        free_string(str);
    };
}
