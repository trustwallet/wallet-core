// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { expect } from "chai";
import { Buffer } from "buffer";
import { WalletCore } from "../dist";

describe("Hash", () => {
  it("test hashKeccak256", () => {
    const { Hash, HexCoding } = WalletCore;

    const sha3Hash = Hash.keccak256(Buffer.from("Test keccak-256", "utf-8"));
    expect(HexCoding.encode(sha3Hash)).to.equal(
      "0x9aeb50f48121c80b2ff73ad48b5f197d940f748d936d35c992367370c1abfb18"
    );

    const sha256Hash = Hash.sha256(Buffer.from('Test hash', 'utf-8'));
    expect(HexCoding.encode(sha256Hash)).to.equal('0xf250fc8f40aeea3297c0158ec1bfa07b503805f2a822530bd63c50625bb9376b');
  });
});
