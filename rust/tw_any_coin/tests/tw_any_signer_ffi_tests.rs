// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_any_signer_sign_unknown_coin() {
    let unsupported_coin = u32::MAX;

    let input_data = TWDataHelper::create(vec![]);
    let output =
        TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), unsupported_coin) });
    assert!(output.is_null());
}
