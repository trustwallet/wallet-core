// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_aes_ctr::*;
use tw_encoding::hex;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_crypto_aes_ctr_128_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key = hex::decode("2b7e151628aed2a6abf7158809cf4f3c").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted = unsafe { crypto_aes_ctr_encrypt_128(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypted = unsafe { TWData::from_ptr_as_mut(encrypted).unwrap() };

    let decrypted = unsafe { crypto_aes_ctr_decrypt_128(encrypted, iv.ptr(), key.ptr()) };
    let decrypted = unsafe { TWData::from_ptr_as_mut(decrypted).unwrap() };

    let decrypted_data = decrypted.to_vec();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_ctr_192_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key = hex::decode("8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted = unsafe { crypto_aes_ctr_encrypt_192(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypted = unsafe { TWData::from_ptr_as_mut(encrypted).unwrap() };

    let decrypted = unsafe { crypto_aes_ctr_decrypt_192(encrypted, iv.ptr(), key.ptr()) };
    let decrypted = unsafe { TWData::from_ptr_as_mut(decrypted).unwrap() };

    let decrypted_data = decrypted.to_vec();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_ctr_256_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted: *mut TWData =
        unsafe { crypto_aes_ctr_encrypt_256(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypted = unsafe { TWData::from_ptr_as_mut(encrypted).unwrap() };

    let decrypted = unsafe { crypto_aes_ctr_decrypt_256(encrypted, iv.ptr(), key.ptr()) };
    let decrypted = unsafe { TWData::from_ptr_as_mut(decrypted).unwrap() };

    let decrypted_data = decrypted.to_vec();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_ctr_encrypt() {
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let data = hex::decode("6bc1bee22e409f96e93d7e117393172a").unwrap();
    let data = TWDataHelper::create(data);
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let encrypt_result = unsafe { crypto_aes_ctr_encrypt(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypt_result = unsafe { TWData::from_ptr_as_mut(encrypt_result).unwrap() };

    assert_eq!(
        hex::encode(&encrypt_result.to_vec(), false),
        "601ec313775789a5b7a7f504bbf3d228"
    );
}

#[test]
fn test_crypto_aes_ctr_decrypt() {
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let cipher = hex::decode("601ec313775789a5b7a7f504bbf3d228").unwrap();
    let cipher = TWDataHelper::create(cipher);
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let decrypt_result = unsafe { crypto_aes_ctr_decrypt(cipher.ptr(), iv.ptr(), key.ptr()) };
    let decrypt_result = unsafe { TWData::from_ptr_as_mut(decrypt_result).unwrap() };

    assert_eq!(
        hex::encode(&decrypt_result.to_vec(), false),
        "6bc1bee22e409f96e93d7e117393172a"
    );
}

#[test]
fn test_crypto_aes_ctr_decrypt_multiple_blocks() {
    let key = hex::decode("fac192ceb5fd772906bea3e118a69e8b").unwrap();
    let key = TWDataHelper::create(key);
    let iv = hex::decode("83dbcc02d8ccb40e466191a123791e0e").unwrap();
    let iv = TWDataHelper::create(iv);
    let data =
        hex::decode("d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c").unwrap();
    let data = TWDataHelper::create(data);

    let decrypt_result = unsafe { crypto_aes_ctr_decrypt(data.ptr(), iv.ptr(), key.ptr()) };
    let decrypt_result = unsafe { TWData::from_ptr_as_mut(decrypt_result).unwrap() };

    assert_eq!(
        hex::encode(&decrypt_result.to_vec(), false),
        "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d"
    );
}

#[test]
fn test_crypto_aes_ctr_encrypt_multiple_blocks() {
    let key = hex::decode("e1094a016e6029eabc6f9e3c3cd9afb8").unwrap();
    let key = TWDataHelper::create(key);
    let iv = hex::decode("884b972d70acece4ecf9b790ffce177e").unwrap();
    let iv = TWDataHelper::create(iv);
    let data = hex::decode("726970706c652073636973736f7273206b69636b206d616d6d616c206869726520636f6c756d6e206f616b20616761696e2073756e206f66666572207765616c746820746f6d6f72726f77207761676f6e207475726e20666174616c00").unwrap();
    let data = TWDataHelper::create(data);

    let result = unsafe { crypto_aes_ctr_encrypt(data.ptr(), iv.ptr(), key.ptr()) };
    let result = unsafe { TWData::from_ptr_as_mut(result).unwrap() };

    assert_eq!(
        hex::encode(&result.to_vec(), false),
        "76b0a3ae037e7d6a50236c4c3ba7560edde4a8a951bf97bc10709e74d8e926c0431866b0ba9852d95bb0bbf41d109f1f3cf2f0af818f96d4f4109a1e3e5b224e3efd57288906a48d47b0006ccedcf96fde7362dedca952dda7cbdd359d"
    );
}

#[test]
fn test_crypto_aes_ctr_encrypt_invalid_key_size() {
    let iv = vec![0; 16];
    let iv = TWDataHelper::create(iv);
    let key = vec![0; 19]; // Invalid key size
    let key = TWDataHelper::create(key);
    let data = vec![0; 100];
    let data = TWDataHelper::create(data);

    let result = unsafe { crypto_aes_ctr_encrypt(data.ptr(), iv.ptr(), key.ptr()) };
    assert!(
        result.is_null(),
        "Expected null result for invalid key size"
    );
}

#[test]
fn test_crypto_aes_ctr_decrypt_invalid_key_size() {
    let iv = vec![0; 16];
    let iv = TWDataHelper::create(iv);
    let key = vec![0; 19]; // Invalid key size
    let key = TWDataHelper::create(key);
    let data = vec![0; 100];
    let data = TWDataHelper::create(data);

    let result = unsafe { crypto_aes_ctr_decrypt(data.ptr(), iv.ptr(), key.ptr()) };
    assert!(
        result.is_null(),
        "Expected null result for invalid key size"
    );
}
