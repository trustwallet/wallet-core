// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { TW } from "../../dist";

describe("Sui", () => {
  it("test sign Sui", () => {
    const { PrivateKey, HexCoding, AnySigner, AnyAddress, CoinType } = globalThis.core;
    const txDataInput = TW.Sui.Proto.SigningInput.create({
      signDirectMessage: TW.Sui.Proto.SignDirect.create({
        unsignedTxMsg: "AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA"
      }),
      privateKey: HexCoding.decode(
              "0x3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266",
          )
    });
    const input = TW.Sui.Proto.SigningInput.encode(txDataInput).finish();
    const outputData = AnySigner.sign(input, CoinType.sui);
    const output = TW.Sui.Proto.SigningOutput.decode(outputData);
    assert.equal(output.signature, "AIYRmHDpQesfAx3iWBCMwInf3MZ56ZQGnPWNtECFjcSq0ssAgjRW6GLnFCX24tfDNjSm9gjYgoLmn1No15iFJAtqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==")
  });
});
