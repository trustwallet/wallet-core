// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_scrypt::crypto_scrypt;
use tw_encoding::hex;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_crypto_scrypt_ffi_null_salt() {
    let password = hex::decode("70617373776f7264").unwrap();
    let password = TWDataHelper::create(password);

    let salt = TWDataHelper::create(Vec::new());

    let res = unsafe { crypto_scrypt(password.ptr(), salt.ptr(), 16384, 8, 4, 32) };
    let res = unsafe { TWData::from_ptr_as_mut(res).unwrap() };
    assert_eq!(
        hex::encode(res.to_vec(), false),
        "004f57df809101216a343d6215879a9a7f1d7e2c04ef2845b4494cf5f10181a1"
    );
}
