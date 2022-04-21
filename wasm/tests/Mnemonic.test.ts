// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { WalletCore } from "../dist";

describe("Mnemonic", () => {
  it("test isValid", () => {
    const { Mnemonic } = WalletCore;

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
    const { Mnemonic } = WalletCore;

    assert.isTrue(Mnemonic.isValidWord("credit"));

    assert.isFalse(Mnemonic.isValidWord("di"));
    assert.isFalse(Mnemonic.isValidWord("cr"));
    assert.isFalse(Mnemonic.isValidWord("hybridous"));
  });

  it("test suggest", () => {
    const { Mnemonic } = WalletCore;

    assert.equal(Mnemonic.suggest("air"), "air airport");
    assert.equal(Mnemonic.suggest("rob"), "robot robust");
  });
});
