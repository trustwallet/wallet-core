// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("Mnemonic", () => {

  it("test isValid", () => {
    const { Mnemonic } = globalThis.core;

    assert.isTrue(
      Mnemonic.isValid(
        "credit expect life fade cover suit response wash pear what skull force"
      )
    );
    assert.isFalse(
      Mnemonic.isValid(
        "ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow"
      )
    );
  });

  it("test isValidWord", () => {
    const { Mnemonic } = globalThis.core;

    assert.isTrue(Mnemonic.isValidWord("credit"));

    assert.isFalse(Mnemonic.isValidWord("di"));
    assert.isFalse(Mnemonic.isValidWord("cr"));
    assert.isFalse(Mnemonic.isValidWord("hybridous"));
  });

  it("test suggest", () => {
    const { Mnemonic } = globalThis.core;

    assert.equal(Mnemonic.suggest("air"), "air airport");
    assert.equal(Mnemonic.suggest("rob"), "robot robust");
  });
});
