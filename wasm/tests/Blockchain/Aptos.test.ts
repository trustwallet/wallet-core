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

describe("Aptos", () => {
  it("test sign aptos", () => {
    const { PrivateKey, HexCoding, AnySigner, AnyAddress, CoinType } = globalThis.core;
    const txDataInput = TW.Aptos.Proto.SigningInput.create({
      chainId: 33,
      sender: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
      transfer: TW.Aptos.Proto.TransferMessage.create({
        amount: new Long(1000),
        to: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
      }),
      sequenceNumber: new Long(99),
      expirationTimestampSecs: new Long(3664390082),
      gasUnitPrice: new Long(100),
      maxGasAmount: new Long(3296766),
      privateKey: PrivateKey.createWithData(
          HexCoding.decode(
              "0x5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec",
          ),
      ).data(),
    });
    const input = TW.Aptos.Proto.SigningInput.encode(txDataInput).finish();
    const outputData = AnySigner.sign(input, CoinType.aptos);
    const output = TW.Aptos.Proto.SigningOutput.decode(outputData);
    assert.equal(HexCoding.encode(output.encoded), "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01")
    assert.equal(HexCoding.encode(output.authenticator!.signature), "0x5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01")
    assert.equal(HexCoding.encode(output.rawTxn), "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021")
  });
});
