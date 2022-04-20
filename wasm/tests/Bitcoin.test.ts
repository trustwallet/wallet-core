// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { expect } from "chai";
import { TW, WalletCore } from "../dist";

describe("Bitcoin", () => {
  it("test Bitcoin SigningInput / SigningOutput", () => {
    expect(TW.Bitcoin.Proto.SigningInput).not.null;
    expect(TW.Binance.Proto.SigningOutput).not.null;
  });
});
