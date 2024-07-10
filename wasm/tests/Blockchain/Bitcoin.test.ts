// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { TW } from "../../dist";
import Long = require("long");

describe("Bitcoin", () => {
  it("test Bitcoin SigningInput / SigningOutput", () => {
    assert.isNotNull(TW.Bitcoin.Proto.SigningInput);
    assert.isNotNull(TW.Binance.Proto.SigningOutput);
  });

  // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7
  it("test Bitcoin sign BRC20 Transfer", () => {
    const { AnySigner, BitcoinSigHashType, PrivateKey, HexCoding, CoinType } = globalThis.core;
    const Proto = TW.BitcoinV2.Proto;

    const privateKeyData = HexCoding.decode("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    const dustSatoshis = new Long(546);
    const txIdInscription = HexCoding.decode("7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca").reverse();
    const txIdForFees = HexCoding.decode("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1").reverse();

    const privateKey = PrivateKey.createWithData(privateKeyData);
    const publicKey = privateKey.getPublicKeySecp256k1(true);
    const bobAddress = "bc1qazgc2zhu2kmy42py0vs8d7yff67l3zgpwfzlpk";

    // Now spend just created `7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca` reveal output.
    const utxo0 = Proto.Input.create({
      outPoint: Proto.OutPoint.create({
        hash: txIdInscription,
        vout: 0,
      }),
      value: dustSatoshis,
      sighashType: BitcoinSigHashType.all.value,
      scriptBuilder: Proto.Input.InputBuilder.create({
        p2wpkh: Proto.PublicKeyOrHash.create({
          pubkey: publicKey.data()
        })
      })
    });

    // UTXO to cover fee.
    const utxo1 = Proto.Input.create({
      outPoint: Proto.OutPoint.create({
        hash: txIdForFees,
        vout: 1,
      }),
      value: new Long(16_400),
      sighashType: BitcoinSigHashType.all.value,
      scriptBuilder: Proto.Input.InputBuilder.create({
        p2wpkh: Proto.PublicKeyOrHash.create({
          pubkey: publicKey.data()
        })
      })
    });

    const out0 = Proto.Output.create({
      value: dustSatoshis,
      toAddress: bobAddress,
    });

    // Change/return transaction. Set it explicitly.
    const changeOut = Proto.Output.create({
      value: new Long(13_400),
      builder: Proto.Output.OutputBuilder.create({
        p2wpkh: Proto.PublicKeyOrHash.create({
          pubkey: publicKey.data()
        })
      })
    });

    const signingInput = Proto.SigningInput.create({
      version: Proto.TransactionVersion.V2,
      privateKeys: [privateKeyData],
      inputs: [utxo0, utxo1],
      outputs: [out0, changeOut],
      inputSelector: Proto.InputSelector.UseAll,
      chainInfo: Proto.ChainInfo.create({
        p2pkhPrefix: 0,
        p2shPrefix: 5,
      }),
      fixedDustThreshold: dustSatoshis,
    });

    const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
      signingV2: signingInput,
    });

    const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
    const outputData = AnySigner.sign(encoded, CoinType.bitcoin);
    const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);

    assert.equal(output.error, TW.Common.Proto.SigningError.OK);
    assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
    assert.equal(
        HexCoding.encode(output.signingResultV2!.encoded),
        "0x02000000000102ca3edda74a46877efa5364ab85947e148508713910ada23e147ea28926dc46700000000000ffffffffb11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790100000000ffffffff022202000000000000160014e891850afc55b64aa8247b2076f8894ebdf889015834000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d024830450221008798393eb0b7390217591a8c33abe18dd2f7ea7009766e0d833edeaec63f2ec302200cf876ff52e68dbaf108a3f6da250713a9b04949a8f1dcd1fb867b24052236950121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb0248304502210096bbb9d1f0596d69875646689e46f29485e8ceccacde9d0025db87fd96d3066902206d6de2dd69d965d28df3441b94c76e812384ab9297e69afe3480ee4031e1b2060121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000"
    );
    assert.equal(
        HexCoding.encode(output.signingResultV2!.txid),
        "0x3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7"
    );
  });

  // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
  it("test Bitcoin sign BRC20 Commit", () => {
    const { AnySigner, BitcoinSigHashType, PrivateKey, HexCoding, CoinType } = globalThis.core;
    const Proto = TW.BitcoinV2.Proto;

    const privateKeyData = HexCoding.decode("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    const dustAmount = new Long(546);
    const txId = HexCoding.decode("8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008").reverse();

    const privateKey = PrivateKey.createWithData(privateKeyData);
    const publicKey = privateKey.getPublicKeySecp256k1(true);

    const utxo0 = Proto.Input.create({
      outPoint: Proto.OutPoint.create({
        hash: txId,
        vout: 1,
      }),
      value: new Long(26_400),
      sighashType: BitcoinSigHashType.all.value,
      scriptBuilder: Proto.Input.InputBuilder.create({
        p2wpkh: Proto.PublicKeyOrHash.create({
          pubkey: publicKey.data()
        })
      })
    });

    const out0 = Proto.Output.create({
      value: new Long(7_000),
      builder: Proto.Output.OutputBuilder.create({
        brc20Inscribe: Proto.Output.OutputBrc20Inscription.create({
          inscribeTo: publicKey.data(),
          ticker: "oadf",
          transferAmount: "20",
        })
      })
    });

    // Change/return transaction. Set it explicitly.
    const changeOut = Proto.Output.create({
      value: new Long(16_400),
      builder: Proto.Output.OutputBuilder.create({
        p2wpkh: Proto.PublicKeyOrHash.create({
          pubkey: publicKey.data()
        })
      })
    });

    const signingInput = Proto.SigningInput.create({
      version: Proto.TransactionVersion.V2,
      privateKeys: [privateKeyData],
      inputs: [utxo0],
      outputs: [out0, changeOut],
      inputSelector: Proto.InputSelector.UseAll,
      chainInfo: Proto.ChainInfo.create({
        p2pkhPrefix: 0,
        p2shPrefix: 5,
      }),
      fixedDustThreshold: dustAmount,
    });

    const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
      signingV2: signingInput,
    });

    const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
    const outputData = AnySigner.sign(encoded, CoinType.bitcoin);
    const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);

    assert.equal(output.error, TW.Common.Proto.SigningError.OK);
    assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
    assert.equal(
      HexCoding.encode(output.signingResultV2!.encoded),
      "0x02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000"
    );
    assert.equal(
      HexCoding.encode(output.signingResultV2!.txid),
      "0x797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1"
    );
  });

  // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca
  it("test Bitcoin sign BRC20 Reveal", () => {
    const { AnySigner, BitcoinSigHashType, PrivateKey, HexCoding, CoinType } = globalThis.core;
    const Proto = TW.BitcoinV2.Proto;

    const privateKeyData = HexCoding.decode("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    const dustAmount = new Long(546);
    const txIdCommit = HexCoding.decode("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1").reverse();

    const privateKey = PrivateKey.createWithData(privateKeyData);
    const publicKey = privateKey.getPublicKeySecp256k1(true);

    // Now spend just created `797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1` commit output.
    const utxo0 = Proto.Input.create({
      outPoint: Proto.OutPoint.create({
        hash: txIdCommit,
        vout: 0,
      }),
      value: new Long(7_000),
      sighashType: BitcoinSigHashType.all.value,
      scriptBuilder: Proto.Input.InputBuilder.create({
        brc20Inscribe: Proto.Input.InputBrc20Inscription.create({
          inscribeTo: publicKey.data(),
          ticker: "oadf",
          transferAmount: "20",
        }),
      }),
    });

    const out0 = Proto.Output.create({
      value: dustAmount,
      builder: Proto.Output.OutputBuilder.create({
        p2wpkh: Proto.PublicKeyOrHash.create({
          pubkey: publicKey.data()
        })
      })
    });

    const signingInput = Proto.SigningInput.create({
      version: Proto.TransactionVersion.V2,
      privateKeys: [privateKeyData],
      inputs: [utxo0],
      outputs: [out0],
      inputSelector: Proto.InputSelector.UseAll,
      chainInfo: Proto.ChainInfo.create({
        p2pkhPrefix: 0,
        p2shPrefix: 5,
      }),
      dangerousUseFixedSchnorrRng: true,
      fixedDustThreshold: dustAmount,
    });

    const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
      signingV2: signingInput,
    });

    const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
    const outputData = AnySigner.sign(encoded, CoinType.bitcoin);
    const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);

    assert.equal(output.error, TW.Common.Proto.SigningError.OK);
    assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
    assert.equal(
        HexCoding.encode(output.signingResultV2!.encoded),
        "0x02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d03406a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe35b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000"
    );
    assert.equal(
        HexCoding.encode(output.signingResultV2!.txid),
        "0x7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca"
    );
  });
});
