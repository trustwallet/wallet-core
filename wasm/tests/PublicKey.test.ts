// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";

describe("PublicKey", () => {

  it("test sign, verify and recover", () => {
    const { PrivateKey, PublicKey, Curve, HexCoding } = globalThis.core;
    
    const privKeyData = 'afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5';
    const messageDigest = HexCoding.decode('0001020304050607080910111213141519171819202122232425262728293031');

    const privkey = PrivateKey.createWithData(HexCoding.decode(privKeyData));

    // Sign the message
    const signature = privkey.sign(messageDigest, Curve.secp256k1);

    // Verify the obtained signature
    const pubkey = privkey.getPublicKeySecp256k1(true);
    assert.isTrue(pubkey.verify(signature, messageDigest));

    // Additionally, recover the pubkey from the signature
    const pubKeyRecovered = PublicKey.recover(signature, messageDigest).compressed();
    assert.equal(HexCoding.encode(pubKeyRecovered.data()), '0x0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1');
    assert.equal(HexCoding.encode(pubKeyRecovered.data()), HexCoding.encode(pubkey.data()));
  });
});
