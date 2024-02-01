// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";

describe("AES", () => {

  it("test decrypting", () => {
    const { AES, HexCoding, AESPaddingMode } = globalThis.core;

    const key = HexCoding.decode(
      "5caa3a74154cee16bd1b570a1330be46e086474ac2f4720530662ef1a469662c"
    );
    const iv = HexCoding.decode("89ef1d6728bac2f1dcde2ef9330d2bb8");
    const cipher = HexCoding.decode(
      "1b3db3674de082d65455eba0ae61cfe7e681c8ef1132e60c8dbd8e52daf18f4fea42cc76366c83351dab6dca52682ff81f828753f89a21e1cc46587ca51ccd353914ffdd3b0394acfee392be6c22b3db9237d3f717a3777e3577dd70408c089a4c9c85130a68c43b0a8aadb00f1b8a8558798104e67aa4ff027b35d4b989e7fd3988d5dcdd563105767670be735b21c4"
    );

    const decrypted = AES.decryptCBC(key, cipher, iv, AESPaddingMode.pkcs7);

    assert.equal(Buffer.from(decrypted).toString(), 
      '{"id":1554098597199736,"jsonrpc":"2.0","method":"wc_sessionUpdate","params":[{"approved":false,"chainId":null,"accounts":null}]}'
    );
  });

  it("test encrypting", () => {
    const { AES, HexCoding, AESPaddingMode } = globalThis.core;

    const key = HexCoding.decode(
      "bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96"
    );
    const iv = HexCoding.decode("5b3a1a561e395d7ad7fe9c92abdacd17");
    const plain = Buffer.from('{"jsonrpc":"2.0","id":1554343834752446,"error":{"code":-32000,"message":"Session Rejected"}}');

    const encrypted = AES.encryptCBC(key, plain, iv, AESPaddingMode.pkcs7);

    assert.equal(HexCoding.encode(encrypted),
      '0x6a93788fcd6d266d06ccff35d1ed328d634605a7f2734f1519256b9950d86d6ca34fe4a13ff0ed513025b49427e6fe15268c84d6dfad6c0c8a21abc9306a5308f545b08d946a2a24b7cd18526bcefd6d9740db9b8e21f4511df148d9b9b03ad9'
    );
  });
});
