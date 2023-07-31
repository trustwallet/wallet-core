// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    assert.equal(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
  });

  it("test address from public key TheOpenNetwork", () => {
    const { PublicKey, PublicKeyType, HexCoding, AnyAddress, CoinType } = globalThis.core;
    let publicKey = PublicKey.createWithData(HexCoding.decode("f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41"), PublicKeyType.ed25519);
    let address = AnyAddress.createWithPublicKey(publicKey, CoinType.ton);
    assert.equal(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
  });

  it("test address from raw string TheOpenNetwork", () => {
    const { AnyAddress, CoinType } = globalThis.core;
    let addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3";
    let address = AnyAddress.createWithString(addressString, CoinType.ton);
    assert.equal(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
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
    assert.equal(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
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
        walletVersion: TW.TheOpenNetwork.Proto.WalletVersion.WALLET_V4_R2,
        dest: "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q",
        amount: new Long(10),
        sequenceNumber: 6,
        mode: (TW.TheOpenNetwork.Proto.SendMode.PAY_FEES_SEPARATELY | TW.TheOpenNetwork.Proto.SendMode.IGNORE_ACTION_PHASE_ERRORS),
        expireAt: 1671132440,
        bounceable: true
    });

    let input = TW.TheOpenNetwork.Proto.SigningInput.create({
        transfer: transfer,
        privateKey: PrivateKey.createWithData(privateKeyData).data(),
    });

    const encoded = TW.TheOpenNetwork.Proto.SigningInput.encode(input).finish();
    let outputData = AnySigner.sign(encoded, CoinType.ton);
    let output = TW.TheOpenNetwork.Proto.SigningOutput.decode(outputData);

    // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
    let expectedString = "te6ccgICAAQAAQAAALAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAA";

    assert.equal(output.encoded, expectedString)
  });

  it("test jetton transfer TheOpenNetwork", () => {
    const { PrivateKey, HexCoding, CoinType, AnySigner } = globalThis.core;

    let privateKeyData = HexCoding.decode("c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee");

    let transferData = TW.TheOpenNetwork.Proto.Transfer.create({
        walletVersion: TW.TheOpenNetwork.Proto.WalletVersion.WALLET_V4_R2,
        dest: "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja",
        amount: new Long(100 * 1000 * 1000),
        sequenceNumber: 1,
        mode: (TW.TheOpenNetwork.Proto.SendMode.PAY_FEES_SEPARATELY | TW.TheOpenNetwork.Proto.SendMode.IGNORE_ACTION_PHASE_ERRORS),
        expireAt: 1787693046,
        comment: "test comment",
        bounceable: true,
    });

    let jettonTransfer = TW.TheOpenNetwork.Proto.JettonTransfer.create({
      transfer: transferData,
      jettonAmount: new Long(500 * 1000 * 1000),
      toOwner: "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8",
      responseAddress: "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk",
      forwardAmount: new Long(1)
    })

    let input = TW.TheOpenNetwork.Proto.SigningInput.create({
        jettonTransfer: jettonTransfer,
        privateKey: PrivateKey.createWithData(privateKeyData).data(),
    });

    const encoded = TW.TheOpenNetwork.Proto.SigningInput.encode(input).finish();
    let outputData = AnySigner.sign(encoded, CoinType.ton);
    let output = TW.TheOpenNetwork.Proto.SigningOutput.decode(outputData);

    // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
    let expectedString = "te6ccgICAAQAAQAAARgAAAFFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKgwAAQGcaIWVosi1XnveAmoG9y0/mPeNUqUu7GY76mdbRAaVeNeDOPDlh5M3BEb26kkc6XoYDekV60o2iOobN+TGS76jBSmpoxdqjgf2AAAAAQADAAIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEAAwDKD4p+pQAAAAAAAAAAQdzWUAgAC4GWcwteHQM+mcQoV+aZ+myCd1jYqUiawhCzuxMdEzkAFoogyrkCban+t9HUgVPCw1YfEGQ6ktObBRqKfkWPiWVCAgAAAAB0ZXN0IGNvbW1lbnQ=";

    assert.equal(output.encoded, expectedString)
  });

  
});


