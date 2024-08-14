// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::test_utils::tw_private_key_helper::TWPrivateKeyHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use wallet_core_rs::ffi::ton::message_signer::tw_ton_message_signer_sign_message;

#[test]
fn test_ton_wallet_create_state_init() {
    let private_key = TWPrivateKeyHelper::with_hex(
        "112d4e2e700a468f1eae699329202f1ee671d6b665caa2d92dea038cf3868c18",
    );
    assert!(!private_key.is_null());
    let message = TWStringHelper::create("Hello world");

    let signature = TWStringHelper::wrap(unsafe {
        tw_ton_message_signer_sign_message(private_key.ptr(), message.ptr())
    });
    assert!(!signature.ptr().is_null());
    assert_eq!(signature.to_string().unwrap(), "2490fbaa72aec0b77b19162bbbe0b0e3f7afd42cc9ef469f0494cd4a366a4bf76643300cd5991f66bce6006336742b8d1d435d541d244dcc013d428472e89504");
}
