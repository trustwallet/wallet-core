// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { TW } from "../../dist";

describe("Sui", () => {
  it("test sign Sui", () => {
    const { PrivateKey, HexCoding, AnySigner, AnyAddress, CoinType } = globalThis.core;
    const txDataInput = TW.Sui.Proto.SigningInput.create({
      signDirectMessage: TW.Sui.Proto.SignDirect.create({
        unsignedTxMsg: "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA"
      }),
      privateKey: HexCoding.decode(
              "0x3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266",
          )
    });
    const input = TW.Sui.Proto.SigningInput.encode(txDataInput).finish();
    const outputData = AnySigner.sign(input, CoinType.sui);
    const output = TW.Sui.Proto.SigningOutput.decode(outputData);
    assert.equal(output.signature, "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==")
  });

  it("test sign Sui from a TS SDK v2 raw JSON", () => {
    const { HexCoding, AnySigner, CoinType } = globalThis.core;
    // The version 2 serialization emitted by `Transaction.toJSON()` of `@mysten/sui` 2.x,
    // the SDK release that replaces the removed JSON-RPC client with `SuiGrpcClient`.
    const rawJson = JSON.stringify({
      version: 2,
      // Must match the address derived from the private key below, or signing is rejected.
      sender: "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2",
      expiration: null,
      gasData: {
        budget: "28457880",
        price: "750",
        owner: null,
        payment: [
          {
            objectId: "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
            version: "486126455",
            digest: "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
          }
        ]
      },
      inputs: [{ Pure: { bytes: "AMqaOwAAAAA=" } }],
      commands: [{ SplitCoins: { coin: { GasCoin: true }, amounts: [{ Input: 0 }] } }]
    });
    const txDataInput = TW.Sui.Proto.SigningInput.create({
      rawJson: rawJson,
      privateKey: HexCoding.decode(
              "0x7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff",
          )
    });
    const input = TW.Sui.Proto.SigningInput.encode(txDataInput).finish();
    const outputData = AnySigner.sign(input, CoinType.sui);
    const output = TW.Sui.Proto.SigningOutput.decode(outputData);
    assert.equal(output.error, 0);
    assert.equal(output.signature, "AI1PXL87wq4i3c2SICbFCDUB8l8CW8TACA00tCdy4qWjjGKqSUNj/J4YxFMjS5OJ1P2ieTcozHEb2FdXx/9u0A6F69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==")
  });
});
