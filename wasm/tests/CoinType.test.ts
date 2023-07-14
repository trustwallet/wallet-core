// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";

describe("CoinType", () => {
  it("test raw value", () => {
    const { CoinType } = globalThis.core;

    assert.equal(CoinType.bitcoin.value, 0);
    assert.equal(CoinType.litecoin.value, 2);
    assert.equal(CoinType.dogecoin.value, 3);
    assert.equal(CoinType.ethereum.value, 60);
    assert.equal(CoinType.binance.value, 714);
    assert.equal(CoinType.cosmos.value, 118);
    assert.equal(CoinType.solana.value, 501);
  });

  it("test CoinTypeExt methods", () => {
    const { CoinType, CoinTypeExt, Blockchain, Purpose, Curve, Derivation } = globalThis.core;

    assert.equal(CoinTypeExt.blockchain(CoinType.solana), Blockchain.solana);
    assert.equal(CoinTypeExt.purpose(CoinType.solana), Purpose.bip44);
    assert.equal(CoinTypeExt.curve(CoinType.solana), Curve.ed25519);
    assert.isTrue(CoinTypeExt.validate(CoinType.solana, "Bxp8yhH9zNwxyE4UqxP7a7hgJ5xTZfxNNft7YJJ2VRjT"))
    assert.equal(CoinTypeExt.derivationPath(CoinType.solana), "m/44'/501'/0'");
    assert.equal(CoinTypeExt.derivationPathWithDerivation(CoinType.solana, Derivation.solanaSolana), "m/44'/501'/0'/0'");
  });

  it("test deriveAddress", () => {
    const { CoinType, CoinTypeExt, PrivateKey, HexCoding } = globalThis.core;

    const data = HexCoding.decode("8778cc93c6596387e751d2dc693bbd93e434bd233bc5b68a826c56131821cb63");
    const key = PrivateKey.createWithData(data);
    const addr = CoinTypeExt.deriveAddress(CoinType.solana, key);
    assert.equal(addr, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
  });

  it("test deriveAddressFromPublicKey", () => {
    const { CoinType, CoinTypeExt, PublicKey, PublicKeyType, HexCoding } = globalThis.core;

    const data = HexCoding.decode("044516c4aa5352035e1bb5be132694e1389a4ac37d32e5e717d35ee4c4dfab513226a9d14ea37a55962ad3644a08e2ce551b4495beabb9b09e688c7b92eba18acc");
    const key = PublicKey.createWithData(data, PublicKeyType.secp256k1Extended);
    const addr = CoinTypeExt.deriveAddressFromPublicKey(CoinType.ethereum, key);
    assert.equal(addr, "0x996891c410FB76C19DBA72C6f6cEFF2d9DD069b1");
  });

  it("test deriveAddressFromPublicKeyAndDerivation", () => {
    const { CoinType, CoinTypeExt, Derivation, PublicKey, PublicKeyType, HexCoding } = globalThis.core;

    const data = HexCoding.decode("0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
    const key = PublicKey.createWithData(data, PublicKeyType.secp256k1);
    const addr = CoinTypeExt.deriveAddressFromPublicKeyAndDerivation(CoinType.bitcoin, key, Derivation.bitcoinSegwit);
    assert.equal(addr, "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4");
  });
});
