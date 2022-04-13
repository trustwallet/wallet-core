// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { expect } from "chai";
import { WalletCore } from "../dist";

describe("HexCoding", () => {
  it("test encoding / decoding hex string", () => {
    var { HexCoding } = WalletCore;

    var expected = new Uint8Array([0x52, 0x8]);
    var decoded = HexCoding.decode("0x5208");

    expect(decoded).to.eql(expected);

    var encoded = HexCoding.encode(expected);
    
    expect(encoded).to.equal("0x5208");
  });
});
