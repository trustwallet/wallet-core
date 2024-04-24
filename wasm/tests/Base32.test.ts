// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import { assert } from "chai";
import { Buffer } from "buffer";

describe("Base32", () => {

  it("test decrypting", () => {
    const { Base32 } = globalThis.core;

    const decoded = Base32.decode("JBSWY3DPK5XXE3DE");

    assert.equal(Buffer.from(decoded).toString(), "HelloWorld");
  });

  it("test decrypting with alphabet", () => {
    const { Base32 } = globalThis.core;

    const decoded = Base32.decodeWithAlphabet(
      "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i",
      "abcdefghijklmnopqrstuvwxyz234567"
    );

    assert.equal(
      Buffer.from(decoded).toString(),
      "7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy"
    );
  });

  it("test encrypting", () => {
    const { Base32 } = globalThis.core;

    const encoded = Base32.encode(Buffer.from("HelloWorld"));

    assert.equal(encoded, "JBSWY3DPK5XXE3DE");
  });

  it("test encrypting with alphabet", () => {
    const { Base32 } = globalThis.core;

    const encoded = Base32.encodeWithAlphabet(
      Buffer.from("7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy"),
      "abcdefghijklmnopqrstuvwxyz234567"
    );

    assert.equal(
      encoded,
      "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i"
    );
  });
});
