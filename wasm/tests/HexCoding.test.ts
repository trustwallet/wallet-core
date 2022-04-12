// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { expect } from "chai";
import * as WalletCore from "../dist/wallet-core";

describe("HexCoding", () => {
  it("test encoding / decoding hex string", () => {
    var { HexCoding } = WalletCore;

    var decoded = HexCoding.decode("0x5208");

    expect(decoded.get(0)).to.equal(0x52);
    expect(decoded.get(1)).to.equal(0x08);

    decoded.push_back(0x01);

    var encoded = HexCoding.encode(decoded);
    expect(encoded).to.equal("0x520801");

  });
});
