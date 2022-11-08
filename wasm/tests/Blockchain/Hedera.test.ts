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

describe("Hedera", () => {

  it("test address", () => {
    const { PrivateKey, HexCoding, AnyAddress, CoinType, Curve } = globalThis.core;
    const address = AnyAddress.createWithString("0.0.48694347", CoinType.hedera);
    assert.equal(address.description(), "0.0.48694347");
    assert.equal(AnyAddress.isValid("0.0.48694347", CoinType.hedera), true);
    assert.equal(AnyAddress.isValid("0.0.a", CoinType.hedera), false);
    address.delete();
  });

  it("test sign simple transfer Hedera", () => {
    const { PrivateKey, HexCoding, AnySigner, AnyAddress, CoinType } = globalThis.core;
    const transferMsg = TW.Hedera.Proto.TransferMessage.create({
      from: "0.0.48694347",
      to: "0.0.48462050",
      amount: new Long(100000000)
    })

    const transactionID = TW.Hedera.Proto.TransactionID.create({
        accountID: "0.0.48694347",
        transactionValidStart: TW.Hedera.Proto.Timestamp.create({
          seconds: new Long(1667222879),
          nanos: 749068449
        })
    })

    const transactionBody = TW.Hedera.Proto.TransactionBody.create({
      memo: "",
      nodeAccountID: "0.0.9",
      transactionFee: new Long(100000000),
      transactionValidDuration: new Long(120),
      transfer: transferMsg,
      transactionID: transactionID
    })

    const txDataInput = TW.Hedera.Proto.SigningInput.create({
      privateKey: PrivateKey.createWithData(
          HexCoding.decode(
              "0xe87a5584c0173263e138db689fdb2a7389025aaae7cb1a18a1017d76012130e8",
          ),
      ).data(),
      body: transactionBody
    });
    const input = TW.Hedera.Proto.SigningInput.encode(txDataInput).finish();
    const outputData = AnySigner.sign(input, CoinType.hedera);
    const output = TW.Hedera.Proto.SigningOutput.decode(outputData);
    assert.equal(HexCoding.encode(output.encoded), "0x0a440a150a0c08df9aff9a0610a1c197e502120518cb889c17120218091880c2d72f22020878721e0a1c0a0c0a0518e2f18d17108084af5f0a0c0a0518cb889c1710ff83af5f12660a640a205d3a70d08b2beafb72c7a68986b3ff819a306078b8c359d739e4966e82e6d40e1a40612589c3b15f1e3ed6084b5a3a5b1b81751578cac8d6c922f31731b3982a5bac80a22558b2197276f5bae49b62503a4d39448ceddbc5ef3ba9bee4c0f302f70c")
  });
});
