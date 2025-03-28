// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("PrivateKey", () => {

  it("test sign without curve", () => {
    const { PrivateKey, HexCoding, Curve } = globalThis.core;

    const data = HexCoding.decode("0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79");

    const key = PrivateKey.createWithData(data, Curve.starkex);
    const digest = HexCoding.decode("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");
    const signature = key.sign(digest);

    assert.equal(
      HexCoding.encode(signature),
      "0x061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a"
    );

    key.delete();
  });
});
