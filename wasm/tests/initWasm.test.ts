// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
