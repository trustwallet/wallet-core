// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::ffi::c_byte_array::CByteArray;

/// Test `pass_eth_signing_msg_through` to avoid dropping code coverage.
#[test]
fn test_pass_eth_signing_msg_through() {
    let serialized = tw_encoding::hex::decode("0a0101120100220509c76524002a030130b9422a3078366231373534373465383930393463343464613938623935346565646561633439353237316430664a20608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151523812360a2a30783533323262333463383865643036393139373162663532613730343734343866306634656663383412081bc16d674ec80000").unwrap();
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
