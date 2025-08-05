// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_aes_cbc::*;
use tw_encoding::hex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_crypto_aes_cbc_encrypt_zero_padding() {
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let iv = hex::decode("000102030405060708090A0B0C0D0E0F").unwrap();
    let iv = TWDataHelper::create(iv);

    let data = hex::decode("6bc1bee22e409f96e93d7e117393172a").unwrap();
    let data = TWDataHelper::create(data);

    let encrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });

    assert_eq!(
        hex::encode(&encrypt_result.to_vec().unwrap(), false),
        "f58c4c04d6e5f1ba779eabfb5f7bfbd6"
    );
}

#[test]
fn test_crypto_aes_cbc_decrypt_zero_padding() {
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let iv = hex::decode("000102030405060708090A0B0C0D0E0F").unwrap();
    let iv = TWDataHelper::create(iv);

    let cipher = hex::decode("f58c4c04d6e5f1ba779eabfb5f7bfbd6").unwrap();
    let cipher = TWDataHelper::create(cipher);

    let decrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_decrypt_cbc(key.ptr(), cipher.ptr(), iv.ptr(), 0) });

    assert_eq!(
        hex::encode(&decrypt_result.to_vec().unwrap(), false),
        "6bc1bee22e409f96e93d7e117393172a"
    );
}

#[test]
fn test_crypto_aes_cbc_encrypt_pkcs7_padding() {
    let key2 =
        hex::decode("bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96").unwrap();
    let key2 = TWDataHelper::create(key2);

    let iv = hex::decode("37f8687086d31852979e228f4a97925b").unwrap();
    let iv = TWDataHelper::create(iv);

    let data = hex::decode("7b226a736f6e727063223a22322e30222c226964223a313535343334333833343735323434362c226572726f72223a7b22636f6465223a2d33323030302c226d657373616765223a2253657373696f6e2052656a6563746564227d7d").unwrap();
    let data = TWDataHelper::create(data);

    let encrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc(key2.ptr(), data.ptr(), iv.ptr(), 1) });

    assert_eq!(hex::encode(&encrypt_result.to_vec().unwrap(), false), "23c75d1b3228742ddb12eeef5a5016e37a8980a77fabc6dd01e6a355d88851c611d37e0d17a2f9c30f659da6d42ba77aca9b84bd6a95e3924f47d9093fbf16e0fb55b165ec193489645b4f7d2573959305c8fa70f88fe5affc43e3084a5878d1");
}

#[test]
fn test_crypto_aes_cbc_decrypt_pkcs7_padding() {
    let key2 =
        hex::decode("bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96").unwrap();
    let key2 = TWDataHelper::create(key2);

    let iv = hex::decode("debb62725b21c7577e4e498e10f096c7").unwrap();
    let iv = TWDataHelper::create(iv);

    let cipher = hex::decode("e7df9810ce66defcc03023ee945f5958c1d4697bf97945daeab5059c2bc6262642cbca82982ac690e77e16671770c200f348f743a7c6e5df5c74eb892ef9b45a9b5ddf0f08fa60c49e5b694688d1b0b521b43975e65b4e8d557a83f4d1aab0af").unwrap();
    let cipher = TWDataHelper::create(cipher);

    let decrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_decrypt_cbc(key2.ptr(), cipher.ptr(), iv.ptr(), 1) });

    assert_eq!(hex::encode(&decrypt_result.to_vec().unwrap(), false), "7b226a736f6e727063223a22322e30222c226964223a313535343334333833343735323434362c226572726f72223a7b22636f6465223a2d33323030302c226d657373616765223a2253657373696f6e2052656a6563746564227d7d");
}

#[test]
fn test_crypto_aes_cbc_encrypt() {
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let iv = hex::decode("000102030405060708090A0B0C0D0E0F").unwrap();
    let iv = TWDataHelper::create(iv);

    let data = hex::decode("6bc1bee22e409f96e93d7e117393172a").unwrap();
    let data = TWDataHelper::create(data);

    let encrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });

    assert_eq!(
        hex::encode(&encrypt_result.to_vec().unwrap(), false),
        "f58c4c04d6e5f1ba779eabfb5f7bfbd6"
    );
}

#[test]
fn test_crypto_aes_cbc_encrypt_with_padding() {
    let key =
        hex::decode("bf6cfdd852f79460981062f551f1dc3215b5e26609bc2a275d5b2da21798b489").unwrap();
    let key = TWDataHelper::create(key);

    let message = "secret message".as_bytes();
    let message = TWDataHelper::create(message.to_vec());

    // Test with PKCS7 padding
    {
        let iv = hex::decode("f300888ca4f512cebdc0020ff0f7224c").unwrap();
        let iv = TWDataHelper::create(iv);

        let encrypt_result = TWDataHelper::wrap(unsafe {
            tw_aes_encrypt_cbc(key.ptr(), message.ptr(), iv.ptr(), 1)
        });

        assert_eq!(
            hex::encode(&encrypt_result.to_vec().unwrap(), false),
            "7f896315e90e172bed65d005138f224d"
        );
    }

    // Test with Zero padding
    {
        let iv = hex::decode("f300888ca4f512cebdc0020ff0f7224c").unwrap();
        let iv = TWDataHelper::create(iv);

        let encrypt_result = TWDataHelper::wrap(unsafe {
            tw_aes_encrypt_cbc(key.ptr(), message.ptr(), iv.ptr(), 0)
        });

        assert_eq!(
            hex::encode(&encrypt_result.to_vec().unwrap(), false),
            "11bcbfebb2db19fb5a5cbf458e0f699e"
        );
    }
}

#[test]
fn test_crypto_aes_cbc_decrypt() {
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let iv = hex::decode("000102030405060708090A0B0C0D0E0F").unwrap();
    let iv = TWDataHelper::create(iv);

    let cipher = hex::decode("f58c4c04d6e5f1ba779eabfb5f7bfbd6").unwrap();
    let cipher = TWDataHelper::create(cipher);

    let decrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_decrypt_cbc(key.ptr(), cipher.ptr(), iv.ptr(), 0) });

    assert_eq!(
        hex::encode(&decrypt_result.to_vec().unwrap(), false),
        "6bc1bee22e409f96e93d7e117393172a"
    );
}

#[test]
fn test_crypto_aes_cbc_decrypt_with_padding() {
    let key =
        hex::decode("bf6cfdd852f79460981062f551f1dc3215b5e26609bc2a275d5b2da21798b489").unwrap();
    let key = TWDataHelper::create(key);

    // Test with PKCS7 padding
    {
        let encrypted_padded = hex::decode("7f896315e90e172bed65d005138f224d").unwrap();
        let encrypted_padded = TWDataHelper::create(encrypted_padded);

        let iv = hex::decode("f300888ca4f512cebdc0020ff0f7224c").unwrap();
        let iv = TWDataHelper::create(iv);

        let decrypt_result = TWDataHelper::wrap(unsafe {
            tw_aes_decrypt_cbc(key.ptr(), encrypted_padded.ptr(), iv.ptr(), 1)
        });

        let secret_message = "secret message".as_bytes();
        assert_eq!(decrypt_result.to_vec().unwrap(), secret_message);
    }

    // Test with no padding
    {
        let encrypted_not_padded = hex::decode("11bcbfebb2db19fb5a5cbf458e0f699e").unwrap();
        let encrypted_not_padded = TWDataHelper::create(encrypted_not_padded);

        let iv = hex::decode("f300888ca4f512cebdc0020ff0f7224c").unwrap();
        let iv = TWDataHelper::create(iv);

        let decrypt_result = TWDataHelper::wrap(unsafe {
            tw_aes_decrypt_cbc(key.ptr(), encrypted_not_padded.ptr(), iv.ptr(), 0)
        });

        let mut expected = "secret message".as_bytes().to_vec();
        expected.push(0);
        expected.push(0);
        assert_eq!(decrypt_result.to_vec().unwrap(), expected);
    }
}

#[test]
fn test_crypto_aes_cbc_encrypt_multiple_blocks() {
    let key = hex::decode("e1094a016e6029eabc6f9e3c3cd9afb8").unwrap();
    let key = TWDataHelper::create(key);
    let iv = hex::decode("884b972d70acece4ecf9b790ffce177e").unwrap();
    let iv = TWDataHelper::create(iv);
    let data = hex::decode("726970706c652073636973736f7273206b69636b206d616d6d616c206869726520636f6c756d6e206f616b20616761696e2073756e206f66666572207765616c746820746f6d6f72726f77207761676f6e207475726e20666174616c00").unwrap();
    let data = TWDataHelper::create(data);

    let encrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });

    assert_eq!(
        hex::encode(&encrypt_result.to_vec().unwrap(), false),
        "30e3ce939cdc80df375aaf6c2cdc7bc265f4eea20c90ab4825c5fc4b5c4517395ea1c28559bf0832a07f9a7fb8fc58786683a48aa8319be215a6b4a597eeaa443973b76401fe959c1bcb4991c9ee20b54c0244f8f43f0f0adcbb50e9ea913bf0"
    );
}

#[test]
fn test_crypto_aes_cbc_decrypt_multiple_blocks() {
    let key = hex::decode("fac192ceb5fd772906bea3e118a69e8b").unwrap();
    let key = TWDataHelper::create(key);
    let iv = hex::decode("83dbcc02d8ccb40e466191a123791e0e").unwrap();
    let iv = TWDataHelper::create(iv);
    let data =
        hex::decode("d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c").unwrap();
    let data = TWDataHelper::create(data);

    let decrypt_result =
        TWDataHelper::wrap(unsafe { tw_aes_decrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });

    assert_eq!(
        hex::encode(&decrypt_result.to_vec().unwrap(), false),
        "d4ade7189ee99ba50399e60a27c9e0fd02cfd1cfa2d15e7491329f361645d2a4"
    );
}

#[test]
fn test_crypto_aes_cbc_encrypt_invalid_key_size() {
    let iv = TWDataHelper::create(vec![0; 16]);
    let key = TWDataHelper::create(vec![0; 19]);
    let data = TWDataHelper::create(vec![0; 100]);

    let result =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });
    assert!(
        result.is_null(),
        "Expected null result for invalid key size"
    );
}

#[test]
fn test_crypto_aes_cbc_decrypt_invalid_key_size() {
    let iv = TWDataHelper::create(vec![0; 16]);
    let key = TWDataHelper::create(vec![0; 19]);
    let data = TWDataHelper::create(vec![0; 100]);

    let result =
        TWDataHelper::wrap(unsafe { tw_aes_decrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });
    assert!(
        result.is_null(),
        "Expected null result for invalid key size"
    );
}

#[test]
fn test_crypto_aes_cbc_decrypt_invalid_data_size() {
    let iv = TWDataHelper::create(vec![0; 16]);
    let key = TWDataHelper::create(vec![0; 16]);
    let data = TWDataHelper::create(vec![0; 100]);

    let result =
        TWDataHelper::wrap(unsafe { tw_aes_decrypt_cbc(key.ptr(), data.ptr(), iv.ptr(), 0) });
    assert!(
        result.is_null(),
        "Expected null result for invalid data size"
    );
}

#[test]
fn test_crypto_aes_cbc_128_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key = hex::decode("2b7e151628aed2a6abf7158809cf4f3c").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc_128(key.ptr(), data.ptr(), iv.ptr(), 0) });

    let decrypted = TWDataHelper::wrap(unsafe {
        tw_aes_decrypt_cbc_128(key.ptr(), encrypted.ptr(), iv.ptr(), 0)
    });

    let decrypted_data = decrypted.to_vec().unwrap();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_cbc_192_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key = hex::decode("8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc_192(key.ptr(), data.ptr(), iv.ptr(), 0) });

    let decrypted = TWDataHelper::wrap(unsafe {
        tw_aes_decrypt_cbc_192(key.ptr(), encrypted.ptr(), iv.ptr(), 0)
    });

    let decrypted_data = decrypted.to_vec().unwrap();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_cbc_256_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted =
        TWDataHelper::wrap(unsafe { tw_aes_encrypt_cbc_256(key.ptr(), data.ptr(), iv.ptr(), 0) });

    let decrypted = TWDataHelper::wrap(unsafe {
        tw_aes_decrypt_cbc_256(key.ptr(), encrypted.ptr(), iv.ptr(), 0)
    });

    let decrypted_data = decrypted.to_vec().unwrap();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}
