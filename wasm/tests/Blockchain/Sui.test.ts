// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
});
