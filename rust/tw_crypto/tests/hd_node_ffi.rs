// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::crypto_hd_node::extended_key::bip32_public_key::BIP32PublicKey;
use tw_crypto::crypto_hd_node::hd_node::HDNode;
use tw_crypto::crypto_mnemonic::mnemonic::Mnemonic;
use tw_crypto::ffi::crypto_hd_node::{
    tw_hd_node_create_with_extended_private_key, tw_hd_node_create_with_seed,
    tw_hd_node_derive_from_path, tw_hd_node_extended_private_key, tw_hd_node_extended_public_key,
    tw_hd_node_private_key_data, tw_hd_node_public_key_data, TWHDNode,
};
use tw_crypto::ffi::crypto_hd_node_public::{
    tw_hd_node_public_create_with_extended_public_key, tw_hd_node_public_derive_from_path,
    tw_hd_node_public_public_key_data, TWHDNodePublic,
};
use tw_crypto::ffi::crypto_mnemonic::{tw_mnemonic_to_entropy, tw_mnemonic_to_seed};
use tw_encoding::hex;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::Curve;
use tw_memory::ffi::{tw_data::TWData, tw_string::TWString, RawPtrTrait};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

const BIP39_TEST_VECTORS: &str = include_str!("bip39_vectors.json");

#[test]
fn test_extended_private_key() {
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");
    let hd_node = HDNode::new(&seed, Curve::Secp256k1).unwrap();

    // BIP44 purpose and Bitcoin coin type
    let purpose = 44;
    let coin = 0;

    // ZPRV version
    let hd_version = 0x04b2430c;

    // default path m/44'/0'/0'
    let path = format!("m/{}'/{}'/{}'", purpose, coin, 0);
    let derived_node = hd_node
        .derive_from_path(&path, Hasher::Sha256ripemd)
        .unwrap();
    let ext_priv_key1 = derived_node
        .extended_private_key(hd_version, Hasher::Sha256d)
        .unwrap();
    assert_eq!(ext_priv_key1, "zprvAcwsTZNaY1f7rfwsy5GseSDStYBrxwtsBZDkb3iyuQUs4NF6n58BuH7Xj54RuaSCWtU5CiQzuYQgFgqr1HokgKcVAeGeXokhJUAJeP3VmvY");

    // explicitly specify default account=0
    let path = format!("m/{}'/{}'/{}'", purpose, coin, 0);
    let derived_node = hd_node
        .derive_from_path(&path, Hasher::Sha256ripemd)
        .unwrap();
    let ext_priv_key2 = derived_node
        .extended_private_key(hd_version, Hasher::Sha256d)
        .unwrap();
    assert_eq!(ext_priv_key2, "zprvAcwsTZNaY1f7rfwsy5GseSDStYBrxwtsBZDkb3iyuQUs4NF6n58BuH7Xj54RuaSCWtU5CiQzuYQgFgqr1HokgKcVAeGeXokhJUAJeP3VmvY");

    // custom account=1
    let path = format!("m/{}'/{}'/{}'", purpose, coin, 1);
    let derived_node = hd_node
        .derive_from_path(&path, Hasher::Sha256ripemd)
        .unwrap();
    let ext_priv_key3 = derived_node
        .extended_private_key(hd_version, Hasher::Sha256d)
        .unwrap();
    assert_eq!(ext_priv_key3, "zprvAcwsTZNaY1f7sifgNNgdNa4P9mPtyg3zRVgwkx2qF9Sn7F255MzP6Zyumn6bgV5xuoS8ZrDvjzE7APcFSacXdzFYpGvyybb1bnAoh5nHxpn");
}

#[test]
fn test_extended_private_key_ffi() {
    let curve: Curve = Curve::Secp256k1;

    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let mnemonic_string = TWStringHelper::create(mnemonic);
    let passphrase_string = TWStringHelper::create("");

    let seed_ptr = unsafe { tw_mnemonic_to_seed(mnemonic_string.ptr(), passphrase_string.ptr()) };

    let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_ptr, curve.to_raw()) };
    let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

    let pubkey_hasher = Hasher::Sha256ripemd;

    let base58_hasher = Hasher::Sha256d;

    // BIP44 purpose and Bitcoin coin type
    let purpose = 44;
    let coin = 0;

    // ZPRV version
    let hd_version = 0x04b2430c;

    // explicitly specify default account=0
    let path = format!("m/{}'/{}'/{}'", purpose, coin, 0);
    let path_string = TWStringHelper::create(&path);
    let derived_node_ptr =
        unsafe { tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into()) };
    let derived_node = unsafe { TWHDNode::from_ptr_as_ref(derived_node_ptr).unwrap() };

    let ext_priv_key =
        unsafe { tw_hd_node_extended_private_key(derived_node, hd_version, base58_hasher.into()) };
    let ext_priv_key_string = unsafe { TWString::from_ptr_as_ref(ext_priv_key).unwrap() };
    let ext_priv_key_string = ext_priv_key_string.as_str().unwrap();
    assert_eq!(ext_priv_key_string, "zprvAcwsTZNaY1f7rfwsy5GseSDStYBrxwtsBZDkb3iyuQUs4NF6n58BuH7Xj54RuaSCWtU5CiQzuYQgFgqr1HokgKcVAeGeXokhJUAJeP3VmvY");

    // custom account=1
    let path = format!("m/{}'/{}'/{}'", purpose, coin, 1);
    let path_string = TWStringHelper::create(&path);
    let derived_node_ptr =
        unsafe { tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into()) };
    let derived_node = unsafe { TWHDNode::from_ptr_as_ref(derived_node_ptr).unwrap() };

    let ext_priv_key =
        unsafe { tw_hd_node_extended_private_key(derived_node, hd_version, base58_hasher.into()) };
    let ext_priv_key_string = unsafe { TWString::from_ptr_as_ref(ext_priv_key).unwrap() };
    let ext_priv_key_string = ext_priv_key_string.as_str().unwrap();
    assert_eq!(ext_priv_key_string, "zprvAcwsTZNaY1f7sifgNNgdNa4P9mPtyg3zRVgwkx2qF9Sn7F255MzP6Zyumn6bgV5xuoS8ZrDvjzE7APcFSacXdzFYpGvyybb1bnAoh5nHxpn");
}

#[test]
fn test_extended_public_key_ffi() {
    let curve: Curve = Curve::Secp256k1;

    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let mnemonic_string = TWStringHelper::create(mnemonic);
    let passphrase_string = TWStringHelper::create("TREZOR");

    let seed_ptr = unsafe { tw_mnemonic_to_seed(mnemonic_string.ptr(), passphrase_string.ptr()) };

    let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_ptr, curve.to_raw()) };
    let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

    // BIP44 purpose and Bitcoin coin type
    let purpose = 44;
    let coin = 42;

    // DPRV version
    let prv_hd_version = 0x2fda4e8;
    let pub_hd_version = 0x2fda926;

    let pubkey_hasher = Hasher::Blake256ripemd;

    let base58_hasher = Hasher::Blake256d;

    // explicitly specify default account=0
    let path = format!("m/{}'/{}'/{}'", purpose, coin, 0);
    let path_string = TWStringHelper::create(&path);
    let derived_node_ptr =
        unsafe { tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into()) };
    let derived_node = unsafe { TWHDNode::from_ptr_as_ref(derived_node_ptr).unwrap() };

    let ext_pub_key = unsafe {
        tw_hd_node_extended_public_key(derived_node, pub_hd_version, base58_hasher.into())
    };
    let ext_pub_key_string = unsafe { TWString::from_ptr_as_ref(ext_pub_key).unwrap() };
    let ext_pub_key_string = ext_pub_key_string.as_str().unwrap();
    assert_eq!(ext_pub_key_string, "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP");

    let ext_priv_key = unsafe {
        tw_hd_node_extended_private_key(derived_node, prv_hd_version, base58_hasher.into())
    };
    let ext_priv_key_string = unsafe { TWString::from_ptr_as_ref(ext_priv_key).unwrap() };
    let ext_priv_key_string = ext_priv_key_string.as_str().unwrap();
    assert_eq!(ext_priv_key_string, "dprv3oggQ2FQ1chcr18hbW7Aur5x8SxQdES3FGa4WqeTZnFY88SNMzLdB7LkZLroF4bGAqWS8sDm3w4DKyYV7sDKfC6JMSVHnVJdpDLgHioq1vq");
}

#[test]
fn test_private_key_from_xprv() {
    use tw_keypair::ecdsa::secp256k1::PrivateKey;

    let xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    let xprv_string = TWStringHelper::create(xprv);

    // Create HDNode from extended private key
    let hd_node_ptr = unsafe {
        tw_hd_node_create_with_extended_private_key(
            xprv_string.ptr(),
            Curve::Secp256k1.to_raw(),
            Hasher::Sha256d.into(),
        )
    };

    let pubkey_hasher = Hasher::Sha256ripemd;

    let path = "m/0/3";
    let path_string = TWStringHelper::create(path);
    let derived_node_ptr = unsafe {
        tw_hd_node_derive_from_path(hd_node_ptr, path_string.ptr(), pubkey_hasher.into())
    };

    let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
    assert!(!private_key_data.is_null());

    let private_key_bytes = unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };
    let private_key = PrivateKey::try_from(private_key_bytes.as_slice()).unwrap();
    let public_key = private_key.public();

    let public_key_hex = hex::encode(public_key.to_bytes(), false);
    assert_eq!(
        public_key_hex,
        "025108168f7e5aad52f7381c18d8f880744dbee21dc02c15abe512da0b1cca7e2f"
    );
}

#[test]
fn test_private_key_from_xprv_invalid() {
    let pubkey_hasher = Hasher::Sha256ripemd;

    {
        // Version bytes (first 4) are invalid, 0x00000000
        let xprv = "11117pE7xwz2GARukXY8Vj2ge4ozfX4HLgy5ztnJXjr5btzJE8EbtPhZwrcPWAodW2aFeYiXkXjSxJYm5QrnhSKFXDgACcFdMqGns9VLqESCq3";
        let xprv_string = TWStringHelper::create(xprv);

        // Create HDNode from extended private key
        let hd_node_ptr = unsafe {
            tw_hd_node_create_with_extended_private_key(
                xprv_string.ptr(),
                Curve::Secp256k1.to_raw(),
                Hasher::Sha256d.into(),
            )
        };

        let path = "m/0/3";
        let path_string = TWStringHelper::create(path);
        let derived_node_ptr = unsafe {
            tw_hd_node_derive_from_path(hd_node_ptr, path_string.ptr(), pubkey_hasher.into())
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
        assert!(private_key_data.is_null());
    }

    {
        // Version bytes (first 4) are invalid, 0xdeadbeef
        let xprv = "pGoh3VZXR4mTkT4bfqj4paog12KmHkAWkdLY8HNsZagD1ihVccygLr1ioLBhVQsny47uEh5swP3KScFc4JJrazx1Y7xvzmH2y5AseLgVMwomBTg2";
        let xprv_string = TWStringHelper::create(xprv);

        // Create HDNode from extended private key
        let hd_node_ptr = unsafe {
            tw_hd_node_create_with_extended_private_key(
                xprv_string.ptr(),
                Curve::Secp256k1.to_raw(),
                Hasher::Sha256d.into(),
            )
        };

        let path = "m/0/3";
        let path_string = TWStringHelper::create(path);
        let derived_node_ptr = unsafe {
            tw_hd_node_derive_from_path(hd_node_ptr, path_string.ptr(), pubkey_hasher.into())
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
        assert!(private_key_data.is_null());
    }
}

#[test]
fn test_derive_xpub_pub_vs_priv_pub() {
    // Test different routes for deriving address from mnemonic, result should be the same:
    // - Direct: mnemonic -> seed -> privateKey -> publicKey -> address
    // - Extended Public: mnemonic -> seed -> zpub -> publicKey -> address
    // - Extended Private: mnemonic -> seed -> zpriv -> privateKey -> publicKey -> address
    let curve: Curve = Curve::Secp256k1;

    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let mnemonic_string = TWStringHelper::create(mnemonic);
    let passphrase_string = TWStringHelper::create("");

    let seed_ptr = unsafe { tw_mnemonic_to_seed(mnemonic_string.ptr(), passphrase_string.ptr()) };

    let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_ptr, curve.to_raw()) };
    let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

    let deriv_path1 = "m/84'/0'/0'/0/0";
    let deriv_path2 = "m/84'/0'/0'/0/2";
    let expected_public_key1 = "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac";
    let expected_public_key2 = "031e1f64d2f6768dccb6814545b2e2d58e26ad5f91b7cbaffe881ed572c65060db";

    let pubkey_hasher = Hasher::Sha256ripemd;

    let base58_hasher = Hasher::Sha256d;

    // -> privateKey -> publicKey
    {
        let path_string = TWStringHelper::create(deriv_path1);
        let derived_node_ptr = unsafe {
            tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into())
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
        let private_key_bytes =
            unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };
        let public_key_hex = hex::encode(
            tw_keypair::ecdsa::secp256k1::PrivateKey::try_from(private_key_bytes.as_slice())
                .unwrap()
                .public()
                .to_bytes(),
            false,
        );
        assert_eq!(public_key_hex, expected_public_key1);
    }
    {
        let path_string = TWStringHelper::create(deriv_path2);
        let derived_node_ptr = unsafe {
            tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into())
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
        let private_key_bytes =
            unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };
        let public_key_hex = hex::encode(
            tw_keypair::ecdsa::secp256k1::PrivateKey::try_from(private_key_bytes.as_slice())
                .unwrap()
                .public()
                .to_bytes(),
            false,
        );
        assert_eq!(public_key_hex, expected_public_key2);
    }

    // zpub -> publicKey
    let account_path = "m/84'/0'/0'";
    let account_path_string = TWStringHelper::create(account_path);
    let account_node_ptr = unsafe {
        tw_hd_node_derive_from_path(hd_node, account_path_string.ptr(), pubkey_hasher.into())
    };
    let account_node = unsafe { TWHDNode::from_ptr_as_ref(account_node_ptr).unwrap() };

    let zpub_version = 0x04b24746; // TWHDVersionZPUB
    let zpub_ptr =
        unsafe { tw_hd_node_extended_public_key(account_node, zpub_version, base58_hasher.into()) };
    let zpub = unsafe {
        TWString::from_ptr_as_ref(zpub_ptr)
            .unwrap()
            .as_str()
            .unwrap()
    };

    assert_eq!(zpub, "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9");

    {
        let hd_node_ptr = unsafe {
            tw_hd_node_public_create_with_extended_public_key(
                zpub_ptr,
                curve.to_raw(),
                Hasher::Sha256d.into(),
            )
        };
        let hd_node = unsafe { TWHDNodePublic::from_ptr_as_ref(hd_node_ptr).unwrap() };

        let child_path = "m/0/0";
        let child_path_string = TWStringHelper::create(child_path);
        let derived_node_ptr = unsafe {
            tw_hd_node_public_derive_from_path(
                hd_node,
                child_path_string.ptr(),
                pubkey_hasher.into(),
            )
        };

        let public_key_data = unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) };
        let public_key_bytes =
            unsafe { TWData::from_ptr_as_ref(public_key_data).unwrap().to_vec() };
        let public_key_hex = hex::encode(public_key_bytes, false);
        assert_eq!(public_key_hex, expected_public_key1);
    }

    {
        let hd_node_ptr = unsafe {
            tw_hd_node_public_create_with_extended_public_key(
                zpub_ptr,
                curve.to_raw(),
                Hasher::Sha256d.into(),
            )
        };
        let hd_node = unsafe { TWHDNodePublic::from_ptr_as_ref(hd_node_ptr).unwrap() };

        let child_path = "m/0/2";
        let child_path_string = TWStringHelper::create(child_path);
        let derived_node_ptr = unsafe {
            tw_hd_node_public_derive_from_path(
                hd_node,
                child_path_string.ptr(),
                pubkey_hasher.into(),
            )
        };

        let public_key_data = unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) };
        let public_key_bytes =
            unsafe { TWData::from_ptr_as_ref(public_key_data).unwrap().to_vec() };
        let public_key_hex = hex::encode(public_key_bytes, false);
        assert_eq!(public_key_hex, expected_public_key2);
    }

    // zpriv -> privateKey -> publicKey
    let zpriv_version = 0x04b2430c; // TWHDVersionZPRV
    let zpriv_ptr = unsafe {
        tw_hd_node_extended_private_key(account_node, zpriv_version, base58_hasher.into())
    };
    let zpriv = unsafe {
        TWString::from_ptr_as_ref(zpriv_ptr)
            .unwrap()
            .as_str()
            .unwrap()
    };

    assert_eq!(zpriv, "zprvAdP7yPRYjmifiGyiXw7zzFRDJtvWXmEFZADVVNbKVQBRSqLu8ACvu6eFvhrnnw4QwdTD8PUVa48MguwiPTiyfn85zWx9iA5MYy4Eufu5bas");

    {
        let zpriv_string = TWStringHelper::create(zpriv);
        let zpriv_node_ptr = unsafe {
            tw_hd_node_create_with_extended_private_key(
                zpriv_string.ptr(),
                Curve::Secp256k1.to_raw(),
                Hasher::Sha256d.into(),
            )
        };

        let child_path = "m/0/0";
        let child_path_string = TWStringHelper::create(child_path);
        let child_node_ptr = unsafe {
            tw_hd_node_derive_from_path(
                zpriv_node_ptr,
                child_path_string.ptr(),
                pubkey_hasher.into(),
            )
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(child_node_ptr) };
        let private_key_bytes =
            unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };
        let public_key_hex = hex::encode(
            tw_keypair::ecdsa::secp256k1::PrivateKey::try_from(private_key_bytes.as_slice())
                .unwrap()
                .public()
                .to_bytes(),
            false,
        );
        assert_eq!(public_key_hex, expected_public_key1);
    }
    {
        let zpriv_string = TWStringHelper::create(zpriv);
        let zpriv_node_ptr = unsafe {
            tw_hd_node_create_with_extended_private_key(
                zpriv_string.ptr(),
                Curve::Secp256k1.to_raw(),
                Hasher::Sha256d.into(),
            )
        };

        let child_path = "m/0/2";
        let child_path_string = TWStringHelper::create(child_path);
        let child_node_ptr = unsafe {
            tw_hd_node_derive_from_path(
                zpriv_node_ptr,
                child_path_string.ptr(),
                pubkey_hasher.into(),
            )
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(child_node_ptr) };
        let private_key_bytes =
            unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };
        let public_key_hex = hex::encode(
            tw_keypair::ecdsa::secp256k1::PrivateKey::try_from(private_key_bytes.as_slice())
                .unwrap()
                .public()
                .to_bytes(),
            false,
        );
        assert_eq!(public_key_hex, expected_public_key2);
    }
}

#[test]
fn test_hedera_key() {
    let curve: Curve = Curve::Ed25519;

    // Hedera specific DER prefixes
    let hedera_der_prefix_private = "302e020100300506032b657004220420";
    let hedera_der_prefix_public = "302a300506032b6570032100";

    let pubkey_hasher = Hasher::Sha256ripemd;

    {
        let mnemonic = "inmate flip alley wear offer often piece magnet surge toddler submit right radio absent pear floor belt raven price stove replace reduce plate home";
        let mnemonic_string = TWStringHelper::create(mnemonic);
        let passphrase_string = TWStringHelper::create("");

        let seed_ptr =
            unsafe { tw_mnemonic_to_seed(mnemonic_string.ptr(), passphrase_string.ptr()) };

        let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_ptr, curve.to_raw()) };
        let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

        let path = "m/44'/3030'/0'/0'/0";
        let path_string = TWStringHelper::create(&path);
        let derived_node_ptr = unsafe {
            tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into())
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
        let private_key_bytes =
            unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };

        let public_key_data = unsafe { tw_hd_node_public_key_data(derived_node_ptr) };
        let public_key_bytes =
            unsafe { TWData::from_ptr_as_ref(public_key_data).unwrap().to_vec() };

        assert_eq!(
            format!("{}{}", hedera_der_prefix_private, hex::encode(private_key_bytes, false)),
            "302e020100300506032b657004220420853f15aecd22706b105da1d709b4ac05b4906170c2b9c7495dff9af49e1391da"
        );
        assert_eq!(
            format!("{}{}", hedera_der_prefix_public, hex::encode(public_key_bytes, false)),
            "302a300506032b6570032100b63b3815f453cf697b53b290b1d78e88c725d39bde52c34c79fb5b4c93894673"
        );
    }

    {
        let mnemonic =
            "walk gun glide frequent exhaust sugar siege prosper staff skill swarm label";
        let mnemonic_string = TWStringHelper::create(mnemonic);
        let passphrase_string = TWStringHelper::create("");

        let seed_ptr =
            unsafe { tw_mnemonic_to_seed(mnemonic_string.ptr(), passphrase_string.ptr()) };

        let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_ptr, curve.to_raw()) };
        let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

        let path = "m/44'/3030'/0'/0'/0";
        let path_string = TWStringHelper::create(&path);
        let derived_node_ptr = unsafe {
            tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into())
        };

        let private_key_data = unsafe { tw_hd_node_private_key_data(derived_node_ptr) };
        let private_key_bytes =
            unsafe { TWData::from_ptr_as_ref(private_key_data).unwrap().to_vec() };

        let public_key_data = unsafe { tw_hd_node_public_key_data(derived_node_ptr) };
        let public_key_bytes =
            unsafe { TWData::from_ptr_as_ref(public_key_data).unwrap().to_vec() };

        assert_eq!(
            format!("{}{}", hedera_der_prefix_private, hex::encode(private_key_bytes, false)),
            "302e020100300506032b657004220420650c5120cbdc6244e3d10001eb27eea4dd3f80c331b3b6969fa434797d4edd50"
        );
        assert_eq!(
            format!("{}{}", hedera_der_prefix_public, hex::encode(public_key_bytes, false)),
            "302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"
        );
    }
}

#[test]
fn test_cardano_key() {
    let mnemonic = "cost dash dress stove morning robust group affair stomach vacant route volume yellow salute laugh";
    let mnemonic_string = TWStringHelper::create(mnemonic);

    let seed_ptr = unsafe { tw_mnemonic_to_entropy(mnemonic_string.ptr()) };

    let curve = Curve::Ed25519ExtendedCardano;
    let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_ptr, curve.to_raw()) };
    let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

    let pubkey_hasher = Hasher::Sha256ripemd;

    let deriv_path = "m/1852'/1815'/0'/0/0";
    let path_string = TWStringHelper::create(deriv_path);
    let derived_node_ptr =
        unsafe { tw_hd_node_derive_from_path(hd_node, path_string.ptr(), pubkey_hasher.into()) };

    let public_key_data = unsafe { tw_hd_node_public_key_data(derived_node_ptr) };
    let public_key_bytes = unsafe { TWData::from_ptr_as_ref(public_key_data).unwrap().to_vec() };

    assert_eq!(
        hex::encode(public_key_bytes, false),
        "fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26faf4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276"
    );
}

#[test]
fn test_bip39_vectors() {
    // BIP39 test vectors, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
    let vectors: serde_json::Value = serde_json::from_str(&BIP39_TEST_VECTORS).unwrap();
    let english_vectors = vectors["english"].as_array().unwrap();

    let curve: Curve = Curve::Secp256k1;
    let base58_hasher = Hasher::Sha256d;
    let version = 0x0488ade4;

    for v in english_vectors {
        let seed = v[2].as_str().unwrap();
        let xprv = v[3].as_str().unwrap();

        let seed_data = TWDataHelper::create(hex::decode(seed).unwrap());

        let hd_node_ptr = unsafe { tw_hd_node_create_with_seed(seed_data.ptr(), curve.to_raw()) };
        let hd_node = unsafe { TWHDNode::from_ptr_as_ref(hd_node_ptr).unwrap() };

        let xprv_ptr =
            unsafe { tw_hd_node_extended_private_key(hd_node, version, base58_hasher.into()) };
        let xprv_string = unsafe {
            TWString::from_ptr_as_ref(xprv_ptr)
                .unwrap()
                .as_str()
                .unwrap()
        };
        assert_eq!(xprv_string, xprv);
    }
}
