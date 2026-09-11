// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { TW } from "../../dist";
import Long = require("long");

describe("TheOpenNetwork", () => {
  it("test address from private key TheOpenNetwork", () => {
    const { PrivateKey, HexCoding, AnyAddress, CoinType, Curve } = globalThis.core;
    let data = HexCoding.decode("63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8");
    let privateKey = PrivateKey.createWithData(data);

    assert.isTrue(PrivateKey.isValid(data, Curve.ed25519));

    let publicKey = privateKey.getPublicKeyEd25519();
    let address = AnyAddress.createWithPublicKey(publicKey, CoinType.ton)
    
    assert.equal(publicKey.description(), "f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41");
    assert.equal(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV");
  });

  it("test address from public key TheOpenNetwork", () => {
    const { PublicKey, PublicKeyType, HexCoding, AnyAddress, CoinType } = globalThis.core;
    let publicKey = PublicKey.createWithData(HexCoding.decode("f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41"), PublicKeyType.ed25519);
    let address = AnyAddress.createWithPublicKey(publicKey, CoinType.ton);
    assert.equal(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV");
  });

  it("test address from raw string TheOpenNetwork", () => {
    const { AnyAddress, CoinType } = globalThis.core;
    let addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3";
    let address = AnyAddress.createWithString(addressString, CoinType.ton);
    assert.equal(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV");
  });

  it("test address invalid hex TheOpenNetwork", () => {
    const { AnyAddress, CoinType } = globalThis.core;
    let addressString = "0:yahoo3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3";
    let valid = AnyAddress.isValid(addressString, CoinType.ton);
    assert.isFalse(valid);
  });

  it("test address invalid workchain id TheOpenNetwork", () => {
    const { AnyAddress, CoinType } = globalThis.core;
    let addressString = "a:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3";
    let valid = AnyAddress.isValid(addressString, CoinType.ton);
    assert.isFalse(valid);
  });
  
  it("test address from user friendly string TheOpenNetwork", () => {
    const { AnyAddress, CoinType } = globalThis.core;
    let addressString = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q";
    let address = AnyAddress.createWithString(addressString, CoinType.ton);
    assert.equal(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV");
  });

  it("test address from user friendly invalid base64 decoding TheOpenNetwork", () => {
    const { AnyAddress, CoinType } = globalThis.core;
    let addressString = "MwCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsors=#";
    let valid = AnyAddress.isValid(addressString, CoinType.ton);
    assert.isFalse(valid);
  });

  it("test sign TheOpenNetwork", () => {
    const { PrivateKey, HexCoding, CoinType, AnySigner } = globalThis.core;

    let privateKeyData = HexCoding.decode("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0");

    let transfer = TW.TheOpenNetwork.Proto.Transfer.create({
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q",
        amount: Buffer.from("0A", "hex"), // 10
        mode: (TW.TheOpenNetwork.Proto.SendMode.PAY_FEES_SEPARATELY | TW.TheOpenNetwork.Proto.SendMode.IGNORE_ACTION_PHASE_ERRORS),
        bounceable: true,
    });

    let input = TW.TheOpenNetwork.Proto.SigningInput.create({
        messages: [transfer],
        privateKey: PrivateKey.createWithData(privateKeyData).data(),
        sequenceNumber: 6,
        expireAt: 1671132440,
        walletVersion: TW.TheOpenNetwork.Proto.WalletVersion.WALLET_V4_R2,
    });

    const encoded = TW.TheOpenNetwork.Proto.SigningInput.encode(input).finish();
    let outputData = AnySigner.sign(encoded, CoinType.ton);
    let output = TW.TheOpenNetwork.Proto.SigningOutput.decode(outputData);

    // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
    let expectedString = "te6cckEBBAEArQABRYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4MAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAgFiYgAzffHi4B365BPJfIJk/F+URKU1UekJ6g4QK02ypVb22YhQAAAAAAAAAAAAAAAAAQMAAA08Nzs=";

    assert.equal(output.encoded, expectedString)
  });

  it("test jetton transfer TheOpenNetwork", () => {
    const { PrivateKey, HexCoding, CoinType, AnySigner } = globalThis.core;

    let privateKeyData = HexCoding.decode("c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee");

    let jettonTransfer = TW.TheOpenNetwork.Proto.JettonTransfer.create({
        jettonAmount: Buffer.from("1DCD6500", "hex"), // 500 * 1000 * 1000,
        toOwner: "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8",
        responseAddress: "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk",
        forwardAmount: Buffer.from("01", "hex"), // 1
    });

    let transfer = TW.TheOpenNetwork.Proto.Transfer.create({
        dest: "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja",
        amount: Buffer.from("05F5E100", "hex"), // 100 * 1000 * 1000,
        mode: (TW.TheOpenNetwork.Proto.SendMode.PAY_FEES_SEPARATELY | TW.TheOpenNetwork.Proto.SendMode.IGNORE_ACTION_PHASE_ERRORS),
        comment: "test comment",
        bounceable: true,
        jettonTransfer: jettonTransfer,
    });

    let input = TW.TheOpenNetwork.Proto.SigningInput.create({
        messages: [transfer],
        privateKey: PrivateKey.createWithData(privateKeyData).data(),
        sequenceNumber: 1,
        expireAt: 1787693046,
        walletVersion: TW.TheOpenNetwork.Proto.WalletVersion.WALLET_V4_R2,
    });

    const encoded = TW.TheOpenNetwork.Proto.SigningInput.encode(input).finish();
    let outputData = AnySigner.sign(encoded, CoinType.ton);
    let output = TW.TheOpenNetwork.Proto.SigningOutput.decode(outputData);

    // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
    let expectedString = "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c=";

    assert.equal(output.encoded, expectedString)
  });

  
});


