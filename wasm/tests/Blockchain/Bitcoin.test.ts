// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { TW } from "../../dist";
import Long = require("long");

describe("Bitcoin", () => {
  it("test Bitcoin SigningInput / SigningOutput", () => {
    assert.isNotNull(TW.Bitcoin.Proto.SigningInput);
    assert.isNotNull(TW.Binance.Proto.SigningOutput);
  });

  it("test Bitcoin sign BRC20 Transfer", () => {
    const { AnySigner, BitcoinScript, PrivateKey, PublicKey, PublicKeyType, HexCoding, Hash, AnyAddress, CoinType, Curve } = globalThis.core;

    const privateKeyData = HexCoding.decode("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    const fullAmount = 26400
    const minerFee = 3000
    const brcInscribeAmount = 7000
    const dustSatoshis = 546
    const forFeeAmount = fullAmount - brcInscribeAmount - minerFee
    const txIdInscription = HexCoding.decode("7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca").reverse()
    const txIDForFees = HexCoding.decode("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1").reverse()

    const privateKey = PrivateKey.createWithData(privateKeyData)
    const publicKey = privateKey.getPublicKeySecp256k1(true)
    const pubKeyHash = Hash.ripemd(Hash.sha256(publicKey.data()))
    const bobPubkey = PublicKey.createWithData(HexCoding.decode("02f453bb46e7afc8796a9629e89e07b5cb0867e9ca340b571e7bcc63fc20c43f2e"), PublicKeyType.secp256k1)
    const bobPubkeyHash = Hash.ripemd(Hash.sha256(bobPubkey.data()))

    const p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(pubKeyHash)
    const outputP2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(bobPubkeyHash)

    const utxo0 = TW.Bitcoin.Proto.UnspentTransaction.create({
      script: p2wpkh.data(),
      amount: new Long(dustSatoshis),
      variant: TW.Bitcoin.Proto.TransactionVariant.P2WPKH,
      outPoint: TW.Bitcoin.Proto.OutPoint.create({
        hash: txIdInscription,
        index: 0,
      })
    })
    const utxo1 = TW.Bitcoin.Proto.UnspentTransaction.create({
      script: p2wpkh.data(),
      amount: new Long(forFeeAmount),
      variant: TW.Bitcoin.Proto.TransactionVariant.P2WPKH,
      outPoint: TW.Bitcoin.Proto.OutPoint.create({
        hash: txIDForFees,
        index: 1,
      })
    })

    const utxoPlan0 = TW.Bitcoin.Proto.UnspentTransaction.create({
      script: outputP2wpkh.data(),
      amount: new Long(dustSatoshis),
      variant: TW.Bitcoin.Proto.TransactionVariant.P2WPKH
    })
    const utxoPlan1 = TW.Bitcoin.Proto.UnspentTransaction.create({
      script: p2wpkh.data(),
      amount: new Long(forFeeAmount - minerFee),
      variant: TW.Bitcoin.Proto.TransactionVariant.P2WPKH
    })

    const input = TW.Bitcoin.Proto.SigningInput.create({
      isItBrcOperation: true,
      privateKey: [privateKeyData],
      utxo: [utxo0, utxo1],
      plan: TW.Bitcoin.Proto.TransactionPlan.create({
        utxos: [utxoPlan0, utxoPlan1]
      })
    })

    const encoded = TW.Bitcoin.Proto.SigningInput.encode(input).finish();
    const outputData = AnySigner.sign(encoded, CoinType.bitcoin);
    const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);
    assert.equal(
        HexCoding.encode(output.encoded),
        "0x02000000000102ca3edda74a46877efa5364ab85947e148508713910ada23e147ea28926dc46700000000000ffffffffb11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790100000000ffffffff022202000000000000160014e891850afc55b64aa8247b2076f8894ebdf889015834000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d024830450221008798393eb0b7390217591a8c33abe18dd2f7ea7009766e0d833edeaec63f2ec302200cf876ff52e68dbaf108a3f6da250713a9b04949a8f1dcd1fb867b24052236950121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb0248304502210096bbb9d1f0596d69875646689e46f29485e8ceccacde9d0025db87fd96d3066902206d6de2dd69d965d28df3441b94c76e812384ab9297e69afe3480ee4031e1b2060121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000"
    );
    assert.equal(output.transactionId, "3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7");
  });
});
