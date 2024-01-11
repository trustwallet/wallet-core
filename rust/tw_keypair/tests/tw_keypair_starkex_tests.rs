// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_hash::H512;
use tw_keypair::tw::{Curve, PrivateKey, PublicKeyType};

#[test]
fn test_starkex_tw_private_key() {
    let privkey_bytes =
        hex::decode("058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe").unwrap();
    let pubkey_bytes =
        hex::decode("02a4c7332c55d6c1c510d24272d1db82878f2302f05b53bcc38695ed5f78fffd").unwrap();

    let privkey = PrivateKey::new(privkey_bytes.clone()).unwrap();
    assert_eq!(privkey.key().into_vec(), privkey_bytes);

    let public = privkey
        .get_public_key_by_type(PublicKeyType::Starkex)
        .unwrap();
    assert_eq!(public.to_bytes(), pubkey_bytes);
}

#[test]
fn test_starkex_tw_private_key_sign() {
    let privkey_bytes =
        hex::decode("0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79").unwrap();
    let hash_to_sign =
        hex::decode("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76").unwrap();

    let privkey = PrivateKey::new(privkey_bytes).unwrap();
    let actual = privkey.sign(&hash_to_sign, Curve::Starkex).unwrap();
    let expected = H512::from("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a");
    assert_eq!(actual, expected.into_vec());
}
