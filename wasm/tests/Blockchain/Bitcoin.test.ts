// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { TW } from "../../dist";

describe("Bitcoin", () => {
  it("test Bitcoin SigningInput / SigningOutput", () => {
    assert.isNotNull(TW.Bitcoin.Proto.SigningInput);
    assert.isNotNull(TW.Binance.Proto.SigningOutput);
  });

  it("test Bitcoin sign arbitrary message, validate (recover)", () => {
    const { PrivateKey, PublicKey, AnyAddress, Hash, Curve, CoinType, HexCoding } = globalThis.core;

    // constants
    const messagePrefix = "Bitcoin Signed Message:\n";
    const coin = CoinType.bitcoin;

    // prepare input data
    const privateKey = PrivateKey.createWithData(HexCoding.decode('afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5'));
    const publicKey = privateKey.getPublicKeySecp256k1(true);
    assert.equal(HexCoding.encode(publicKey.data()), "0x0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    const address = AnyAddress.createWithPublicKey(publicKey, coin);
    assert.equal(address.description(), "bc1qten42eesehw0ktddcp0fws7d3ycsqez3f7d5yt");
    const message = "Hello, world!";
    const fullMessage = messagePrefix + message;
    const messageHash = Hash.sha256SHA256(fullMessage);

    // Sign message
    const signature = privateKey.sign(messageHash, Curve.secp256k1);
    assert.equal(HexCoding.encode(signature), '0xb2a6444952eafe03226a4cd8e252de7ae594eba44996754c02ac086c5b9b48263a83488857e655940ccfb140d82df954fbc2748735720616efdad09f758b985900');

    // Verify siganture
    const publicKeyRecovered = PublicKey.recover(signature, messageHash).compressed();
    assert.equal(HexCoding.encode(publicKeyRecovered.data()), "0x0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    assert.equal(HexCoding.encode(publicKeyRecovered.data()), HexCoding.encode(publicKey.data()));
  });
});
