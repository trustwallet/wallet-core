// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { TW, WalletCore } from "../../dist";

describe("Ethereum", () => {

  it("test address", () => {
    const { PrivateKey, HexCoding, AnyAddress, CoinType, Curve } = WalletCore;
    const data = HexCoding.decode("727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06");

    assert.isTrue(PrivateKey.isValid(data, Curve.secp256k1));

    const key = new PrivateKey.createWithData(data);
    const pubKey = key.getPublicKeySecp256k1(false);

    assert.equal(HexCoding.encode(pubKey.data()), "0x043182a24fdefe5711d735a434e983bf32a63fd99d214d63936b312643c325c6e33545c4aaff6b923544044d363d73668ec8724b7e62b54d17d49879405cf20648")

    const address = new AnyAddress.createWithPublicKey(pubKey, CoinType.smartChain);
    
    assert.equal(address.description(), "0xf3d468DBb386aaD46E92FF222adDdf872C8CC064");

    key.delete();
    pubKey.delete();
    address.delete();
  });

  it("test signing Ethereum tx", () => {
    const { HexCoding, AnySigner, CoinType } = WalletCore;
    const input = TW.Ethereum.Proto.SigningInput.create({
      toAddress: "0x3535353535353535353535353535353535353535",
      chainId: Buffer.from("01", "hex"),
      nonce: Buffer.from("09", "hex"),
      gasPrice: Buffer.from("04a817c800", "hex"),
      gasLimit: Buffer.from("5208", "hex"),
      transaction: TW.Ethereum.Proto.Transaction.create({
        transfer: TW.Ethereum.Proto.Transaction.Transfer.create({
          amount: Buffer.from("0de0b6b3a7640000", "hex"),
        }),
      }),
      privateKey: HexCoding.decode("4646464646464646464646464646464646464646464646464646464646464646"),
    });


    const encoded = TW.Ethereum.Proto.SigningInput.encode(input).finish();
    assert.equal(HexCoding.encode(encoded),
      "0x0a0101120109220504a817c8002a025208422a3078333533353335333533353335333533353335333533353335333533353335333533353335333533354a204646464646464646464646464646464646464646464646464646464646464646520c0a0a0a080de0b6b3a7640000"
    );

    const outputData = AnySigner.sign(
      encoded,
      CoinType.ethereum
    );
    const output = TW.Ethereum.Proto.SigningOutput.decode(outputData);
    assert.equal(HexCoding.encode(output.encoded),
      "0xf86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83"
    );
  });
});
