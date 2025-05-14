// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{DerivationPath, Prefix, XPub};
use std::str::FromStr;
use tw_encoding::hex;
use tw_hash::hasher::Hasher;

use tw_crypto::crypto_hd_node::{
    extended_key::bip32_private_key::BIP32PrivateKey,
    hd_node::{
        XPrvCardano, XPrvCurve25519Waves, XPrvEd25519, XPrvEd25519Blake2bNano, XPrvNist256p1,
        XPrvSecp256k1, XPrvZilliqaSchnorr,
    },
};
use tw_crypto::crypto_mnemonic::mnemonic::Mnemonic;

#[test]
fn test_from_seed() {
    let seed = Mnemonic::to_seed(
        "tiny escape drive pupil flavor endless love walk gadget match filter luxury",
        "",
    );
    assert_eq!(hex::encode(seed, false), "d430216f5b506dfd281d6ff6e92150d205868923df00774bc301e5ffdc2f4d1ad38a602017ddea6fc7d6315345d8b9cadbd8213ed2ffce5dfc550fa918665eb8");
    let xprv = XPrvSecp256k1::new(&seed);
    assert!(xprv.is_ok());
}

#[test]
fn test_extended_private_key() {
    let seed = Mnemonic::to_seed("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal", "");
    let xprv = XPrvSecp256k1::new(&seed).unwrap();

    let child_xprv = xprv
        .derive_from_path(
            &DerivationPath::from_str("m/44'/0'/0'").unwrap(),
            Hasher::Sha256ripemd,
        )
        .unwrap();
    let child_extended_key = child_xprv.to_extended_key(Prefix::ZPRV).unwrap();
    assert_eq!(child_extended_key.to_string(), "zprvAcwsTZNaY1f7rfwsy5GseSDStYBrxwtsBZDkb3iyuQUs4NF6n58BuH7Xj54RuaSCWtU5CiQzuYQgFgqr1HokgKcVAeGeXokhJUAJeP3VmvY");

    let child_xprv = xprv
        .derive_from_path(
            &DerivationPath::from_str("m/44'/0'/1'").unwrap(),
            Hasher::Sha256ripemd,
        )
        .unwrap();
    let child_extended_key = child_xprv.to_extended_key(Prefix::ZPRV).unwrap();
    assert_eq!(child_extended_key.to_string(), "zprvAcwsTZNaY1f7sifgNNgdNa4P9mPtyg3zRVgwkx2qF9Sn7F255MzP6Zyumn6bgV5xuoS8ZrDvjzE7APcFSacXdzFYpGvyybb1bnAoh5nHxpn");
}

#[test]
fn test_extended_private_key_mtpv() {
    let xprv = XPrvSecp256k1::from_base58("Mtpv7SkyM349Svcf1WiRtB5hC91ZZkVsGuv3kz1V7tThGxBFBzBLFnw6LpaSvwpHHuy8dAfMBqpBvaSAHzbffvhj2TwfojQxM7Ppm3CzW67AFL5", Hasher::Sha256d).unwrap();

    let path = "m/0/4";
    let child_xprv = xprv
        .derive_from_path(
            &DerivationPath::from_str(path).unwrap(),
            Hasher::Sha256ripemd,
        )
        .unwrap();

    let public_key = child_xprv.public_key();
    assert_eq!(
        hex::encode(public_key.to_bytes(), false),
        "02c36f9c3051e9cfbb196ecc35311f3ad705ea6798ffbe6b039e70f6bd047e6f2c"
    );
}

#[test]
fn test_extended_public_key() {
    let seed = Mnemonic::to_seed("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal", "");
    let xprv = XPrvSecp256k1::new(&seed).unwrap();

    let child_xprv = xprv
        .derive_from_path(
            &DerivationPath::from_str("m/44'/0'/0'").unwrap(),
            Hasher::Sha256ripemd,
        )
        .unwrap();
    let child_extended_key = child_xprv.public_key().to_extended_key(Prefix::ZPUB);
    assert_eq!(child_extended_key.to_string(), "zpub6qwDs4uUNPDR5A2M56ot1aABSa2MNQciYn9MPS8bTk1qwAaFKcSST5S1aLidvPp9twqpaumG7vikR2vHhBXjp5oGgHyMvWK3AtUkfeEgyns");

    let child_xprv = xprv
        .derive_from_path(
            &DerivationPath::from_str("m/44'/0'/1'").unwrap(),
            Hasher::Sha256ripemd,
        )
        .unwrap();
    let child_extended_key = child_xprv.public_key().to_extended_key(Prefix::ZPUB);
    assert_eq!(child_extended_key.to_string(), "zpub6qwDs4uUNPDR6Ck9UQDdji17hoEPP8mqnicYZLSSoUykz3MDcuJdeNJPd3BozqEafeLZkegWqzAvkgA4JZZ5tTN2rDpGKfk54essyfx1eZP");
}

#[test]
fn test_get_key_by_curve() {
    let deriv_path = "m/44'/539'/0'/0/0";
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");

    // Test with Secp256k1 curve
    {
        let xprv = XPrvSecp256k1::new(&seed).unwrap();
        let path = DerivationPath::from_str(deriv_path).unwrap();
        let xprv = xprv.derive_from_path(&path, Hasher::Sha256ripemd).unwrap();
        let private_key = xprv.private_key().to_bytes();
        assert_eq!(
            hex::encode(private_key, false),
            "4fb8657d6464adcaa086d6758d7f0b6b6fc026c98dc1671fcc6460b5a74abc62"
        );
    }

    // Test with NIST256p1 curve
    {
        let xprv = XPrvNist256p1::new(&seed).unwrap();
        let path = DerivationPath::from_str(deriv_path).unwrap();
        let xprv = xprv.derive_from_path(&path, Hasher::Sha256ripemd).unwrap();
        let private_key = xprv.private_key().to_bytes();
        assert_eq!(
            hex::encode(private_key, false),
            "a13df52d5a5b438bbf921bbf86276e4347fe8e2f2ed74feaaee12b77d6d26f86"
        );
    }
}

#[test]
fn test_derive_xpub_pub_vs_priv_pub() {
    // Test different routes for deriving address from mnemonic, result should be the same:
    // - Direct: mnemonic -> seed -> privateKey -> publicKey -> address
    // - Extended Public: mnemonic -> seed -> zpub -> publicKey -> address
    // - Extended Private: mnemonic -> seed -> zpriv -> privateKey -> publicKey -> address

    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");
    let xprv = XPrvSecp256k1::new(&seed).unwrap();

    let deriv_path1 = DerivationPath::from_str("m/84'/0'/0'/0/0").unwrap();
    let deriv_path2 = DerivationPath::from_str("m/84'/0'/0'/0/2").unwrap();
    let expected_public_key1 = "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac";
    let expected_public_key2 = "031e1f64d2f6768dccb6814545b2e2d58e26ad5f91b7cbaffe881ed572c65060db";

    // -> privateKey -> publicKey
    {
        let xprv1 = xprv
            .derive_from_path(&deriv_path1, Hasher::Sha256ripemd)
            .unwrap();
        assert_eq!(
            hex::encode(xprv1.public_key().to_bytes(), false),
            expected_public_key1
        );
    }
    {
        let xprv2 = xprv
            .derive_from_path(&deriv_path2, Hasher::Sha256ripemd)
            .unwrap();
        assert_eq!(
            hex::encode(xprv2.public_key().to_bytes(), false),
            expected_public_key2
        );
    }

    // zpub -> publicKey
    let account_path = DerivationPath::from_str("m/84'/0'/0'").unwrap();
    let account_xprv = xprv
        .derive_from_path(&account_path, Hasher::Sha256ripemd)
        .unwrap();
    let zpub = account_xprv
        .public_key()
        .to_extended_key(Prefix::ZPUB)
        .to_string();
    assert_eq!(zpub, "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9");

    let deriv_path1 = DerivationPath::from_str("m/0/0").unwrap();
    {
        let public_key1 = XPub::from_str(&zpub).unwrap();
        let public_key1 = deriv_path1.iter().fold(public_key1, |key, child_num| {
            key.derive_child(child_num).unwrap() //_or(key)
        });
        assert_eq!(
            hex::encode(public_key1.to_bytes(), false),
            expected_public_key1
        );
    }
    let deriv_path2 = DerivationPath::from_str("m/0/2").unwrap();
    {
        let public_key2 = XPub::from_str(&zpub).unwrap();
        let public_key2 = deriv_path2.iter().fold(public_key2, |key, child_num| {
            key.derive_child(child_num).unwrap() //_or(key)
        });
        assert_eq!(
            hex::encode(public_key2.to_bytes(), false),
            expected_public_key2
        );
    }

    // zpriv -> privateKey -> publicKey
    let zpriv = account_xprv
        .to_extended_key(Prefix::ZPRV)
        .unwrap()
        .to_string();
    assert_eq!(zpriv, "zprvAdP7yPRYjmifiGyiXw7zzFRDJtvWXmEFZADVVNbKVQBRSqLu8ACvu6eFvhrnnw4QwdTD8PUVa48MguwiPTiyfn85zWx9iA5MYy4Eufu5bas");

    let deriv_path1 = DerivationPath::from_str("m/0/0").unwrap();
    {
        let private_key1 = XPrvSecp256k1::from_base58(&zpriv, Hasher::Sha256d).unwrap();
        let private_key1 = deriv_path1.iter().fold(private_key1, |key, child_num| {
            key.derive_child(child_num, Hasher::Sha256ripemd).unwrap() //_or(key)
        });
        assert_eq!(
            hex::encode(private_key1.public_key().to_bytes(), false),
            expected_public_key1
        );
    }

    let deriv_path2 = DerivationPath::from_str("m/0/2").unwrap();
    {
        let private_key2 = XPrvSecp256k1::from_base58(&zpriv, Hasher::Sha256d).unwrap();
        let private_key2 = deriv_path2.iter().fold(private_key2, |key, child_num| {
            key.derive_child(child_num, Hasher::Sha256ripemd).unwrap() //_or(key)
        });
        assert_eq!(
            hex::encode(private_key2.public_key().to_bytes(), false),
            expected_public_key2
        );
    }
}

#[test]
fn test_aptos_key() {
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");
    let xprv = XPrvEd25519::new(&seed).unwrap();

    let deriv_path = DerivationPath::from_str("m/44'/637'/0'/0'/0'").unwrap();
    let xprv = xprv
        .derive_from_path(&deriv_path, Hasher::Sha256ripemd)
        .unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "7f2634c0e2414a621e96e39c41d09021700cee12ee43328ed094c5580cd0bd6f"
    );

    let public_key = xprv.public_key();
    assert_eq!(
        hex::encode(public_key.to_bytes(), false),
        "633e5c7e355bdd484706436ce1f06fdf280bd7c2229a7f9b6489684412c6967c"
    );
}

#[test]
fn test_cardano_key() {
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let entropy = Mnemonic::parse(mnemonic).unwrap().to_entropy();
    let xprv = XPrvCardano::new(&entropy).unwrap();

    let deriv_path = DerivationPath::from_str("m/44'/637'/0'/0'/0").unwrap();
    let xprv = xprv
        .derive_from_path(&deriv_path, Hasher::Sha256ripemd)
        .unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "680113743091be93bcdab47ec2f6a2e3c710812f3f051ebb84ac70aa15a14952c8d771b5dd2726467412ed62c37d6c819c36d1dba83991a8585c31bb4790f2cde5232f0770ce99adfc7e6ec1a5270f52d6435c30ceb51415258d1eaccd28b5fe"
    );

    let public_key = xprv.public_key();
    assert_eq!(
        hex::encode(public_key.to_bytes(), false),
        "797a077d4f2cca772b45fa67ada88502000470adf7f81fcb578357a73649fc76e5232f0770ce99adfc7e6ec1a5270f52d6435c30ceb51415258d1eaccd28b5fe"
    );
}

#[test]
fn test_nano_key() {
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");
    let xprv = XPrvEd25519Blake2bNano::new(&seed).unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "d258c2521f7802b8e83c32f2cc97bd06b69747847390c5e247a3d19faa74202e"
    );
    let xpub = xprv.public_key();
    assert_eq!(
        hex::encode(xpub.to_bytes(), false),
        "9833ff5684764ca31955727966df954be334ea051ad8c285eea6e4fbaa549001"
    );

    let deriv_path = DerivationPath::from_str("m/44'/637'/0'/0'").unwrap();
    let xprv = xprv
        .derive_from_path(&deriv_path, Hasher::Sha256ripemd)
        .unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "ffd43b8b4273e69a8278b9dbb4ac724134a878adc82927e503145c935b432959"
    );

    let public_key = xprv.public_key();
    assert_eq!(
        hex::encode(public_key.to_bytes(), false),
        "37d29aff891f03abaa1ea1989cff6de0f46bd677f3ca7b3abb6e7f1c03786540"
    );
}

#[test]
fn test_waves_key() {
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");
    let xprv = XPrvCurve25519Waves::new(&seed).unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "7374826cbd731cf656c11b3fdd458084288f655c8fd4056175996655d0fda4c9"
    );
    let xpub = xprv.public_key();
    assert_eq!(
        hex::encode(xpub.to_bytes(), false),
        "b6c00ffdacb469da62062a1dc8218a733a61720ab0942ba3625194281faf7d3d"
    );
}

#[test]
fn test_zilliqa_schnorr_key() {
    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    let seed = Mnemonic::to_seed(mnemonic, "");
    let xprv = XPrvZilliqaSchnorr::new(&seed).unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "d1b2b553b053f278d510a8494ead811252b1d5ec0da4434d0997a75de92bcea9"
    );
    let xpub = xprv.public_key();
    assert_eq!(
        hex::encode(xpub.to_bytes(), false),
        "02f54cd391076f956b1cfc37cf182c18373f7c1566408c1748132cf4e782498e19"
    );

    let deriv_path = DerivationPath::from_str("m/44'/637'/0'/0'/0").unwrap();
    let xprv = xprv
        .derive_from_path(&deriv_path, Hasher::Sha256ripemd)
        .unwrap();
    assert_eq!(
        hex::encode(xprv.private_key().to_bytes(), false),
        "4fc45a32e714677a8d3fbed23a8e1afbba8decbf60d479149129342dc894d2a4"
    );

    let public_key = xprv.public_key();
    assert_eq!(
        hex::encode(public_key.to_bytes(), false),
        "03758382b7e39cf4790a6b4388254e36fa7aedd48e4595ad219687a8495c27d364"
    );
}
