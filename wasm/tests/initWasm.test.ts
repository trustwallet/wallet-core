// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { initWasm } from "../dist";

describe("Module", () => {
  it("load test", (done) => {
    initWasm().then((WalletCore) => {
      assert.isDefined(WalletCore);
      assert.isNotNull(WalletCore);
      done();
    });
  }).timeout(5000);
});
