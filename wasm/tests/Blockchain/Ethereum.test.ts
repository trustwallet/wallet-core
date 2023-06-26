// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { TW } from "../../dist";

describe("Ethereum", () => {

  it("test address", () => {
    const { PrivateKey, HexCoding, AnyAddress, CoinType, Curve } = globalThis.core;

    const data = HexCoding.decode("727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06");

    assert.isTrue(PrivateKey.isValid(data, Curve.secp256k1));

    const key = PrivateKey.createWithData(data);
    const pubKey = key.getPublicKeySecp256k1(false);

    assert.equal(
      HexCoding.encode(pubKey.data()),
      "0x043182a24fdefe5711d735a434e983bf32a63fd99d214d63936b312643c325c6e33545c4aaff6b923544044d363d73668ec8724b7e62b54d17d49879405cf20648"
    );

    const address = AnyAddress.createWithPublicKey(pubKey, CoinType.smartChain);

    assert.equal(address.description(), "0xf3d468DBb386aaD46E92FF222adDdf872C8CC064");

    key.delete();
    pubKey.delete();
    address.delete();
  });

  it("test signing transfer tx", () => {
    const { HexCoding, AnySigner, CoinType } = globalThis.core;;
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
      privateKey: HexCoding.decode(
        "4646464646464646464646464646464646464646464646464646464646464646"
      ),
    });

    const encoded = TW.Ethereum.Proto.SigningInput.encode(input).finish();
    assert.equal(
      HexCoding.encode(encoded),
      "0x0a0101120109220504a817c8002a025208422a3078333533353335333533353335333533353335333533353335333533353335333533353335333533354a204646464646464646464646464646464646464646464646464646464646464646520c0a0a0a080de0b6b3a7640000"
    );

    const outputData = AnySigner.sign(encoded, CoinType.ethereum);
    const output = TW.Ethereum.Proto.SigningOutput.decode(outputData);
    assert.equal(
      HexCoding.encode(output.encoded),
      "0xf86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83"
    );
  });

  it("test signing eip1559 erc20 transfer tx", () => {
    const { HexCoding, AnySigner, CoinType } = globalThis.core;;

    const input = TW.Ethereum.Proto.SigningInput.create({
      toAddress: "0x6b175474e89094c44da98b954eedeac495271d0f",
      chainId: Buffer.from("01", "hex"),
      nonce: Buffer.from("00", "hex"),
      txMode: TW.Ethereum.Proto.TransactionMode.Enveloped,
      maxInclusionFeePerGas: Buffer.from("0077359400", "hex"),
      maxFeePerGas: Buffer.from("00b2d05e00", "hex"),
      gasLimit: Buffer.from("0130B9", "hex"),
      transaction: TW.Ethereum.Proto.Transaction.create({
        erc20Transfer: TW.Ethereum.Proto.Transaction.ERC20Transfer.create({
          to: "0x5322b34c88ed0691971bf52a7047448f0f4efc84",
          amount: Buffer.from("1bc16d674ec80000", "hex"),
        }),
      }),
      privateKey: HexCoding.decode(
        "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151"
      ),
    });

    const encoded = TW.Ethereum.Proto.SigningInput.encode(input).finish();
    const outputData = AnySigner.sign(encoded, CoinType.ethereum);
    const output = TW.Ethereum.Proto.SigningOutput.decode(outputData);
    assert.equal(
      HexCoding.encode(output.encoded),
      "0x02f8b00180847735940084b2d05e00830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000c080a0adfcfdf98d4ed35a8967a0c1d78b42adb7c5d831cf5a3272654ec8f8bcd7be2ea011641e065684f6aa476f4fd250aa46cd0b44eccdb0a6e1650d658d1998684cdf"
    );
  });

  it("test signing personal message", () => {
    const { HexCoding, Hash, PrivateKey, Curve } = globalThis.core;
    const message = Buffer.from("Some data");
    const prefix = Buffer.from("\x19Ethereum Signed Message:\n" + message.length);
    const hash = Hash.keccak256(Buffer.concat([prefix, message]));

    assert.equal(HexCoding.encode(hash), "0x1da44b586eb0729ff70a73c326926f6ed5a25f5b056e7f47fbc6e58d86871655");

    var key = PrivateKey.createWithData(HexCoding.decode("1fcb84974220eb76e619d7208e1446ae9c0f755e97fb220a8f61c7dc03a0dfce"));

    const signature = key.sign(hash, Curve.secp256k1);

    assert.equal(HexCoding.encode(signature), "0x58156c371347613642e94b66abc4ced8e36011fb3233f5372371aa5ad321671b1a10c0b88f47ce543fd4c455761f5fbf8f61d050f57dcba986640011da794a9000");

    key.delete();
  });

  it("test signing EIP712 message", () => {
    const { EthereumAbi, HexCoding, Hash, PrivateKey, Curve } = globalThis.core;;

    const key = PrivateKey.createWithData(Hash.keccak256(Buffer.from("cow")));
    const message = {
      types: {
        EIP712Domain: [
          { name: "name", type: "string" },
          { name: "version", type: "string" },
          { name: "chainId", type: "uint256" },
          { name: "verifyingContract", type: "address" },
        ],
        Person: [
          { name: "name", type: "string" },
          { name: "wallet", type: "address" },
        ],
        Mail: [
          { name: "from", type: "Person" },
          { name: "to", type: "Person" },
          { name: "contents", type: "string" },
        ],
      },
      primaryType: "Mail",
      domain: {
        name: "Ether Mail",
        version: "1",
        chainId: "0x01",
        verifyingContract: "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC",
      },
      message: {
        from: {
          name: "Cow",
          wallet: "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826",
        },
        to: {
          name: "Bob",
          wallet: "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB",
        },
        contents: "Hello, Bob!",
      },
    };

    const hash = EthereumAbi.encodeTyped(JSON.stringify(message));
    const signature = key.sign(hash, Curve.secp256k1);
  
    assert.equal(HexCoding.encode(hash), "0xbe609aee343fb3c4b28e1df9e632fca64fcfaede20f02e86244efddf30957bd2");
    assert.equal(HexCoding.encode(signature), "0x4355c47d63924e8a72e509b65029052eb6c299d53a04e167c5775fd466751c9d07299936d304c153f6443dfa05f40ff007d72911b6f72307f996231605b9156201");

    key.delete();
  });
});
