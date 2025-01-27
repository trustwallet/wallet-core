// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { TW } from "../../dist";
import Long = require("long");

describe("Zcash", () => {
    // Successfully broadcasted: https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
    it("test Zcash sign Sapling", () => {
        const { AnySigner, BitcoinSigHashType, HexCoding, CoinType } = globalThis.core;
        const Proto = TW.BitcoinV2.Proto;

        const privateKeyData = HexCoding.decode("a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559");
        const dustAmount = new Long(546);
        const utxoTxId = HexCoding.decode("3a19dd44032dfed61bfca5ba5751aab8a107b30609cbd5d70dc5ef09885b6853").reverse();
        const sapplingBranchId = HexCoding.decode("bb09b876");

        const utxo0 = Proto.Input.create({
            outPoint: {
                hash: utxoTxId,
                vout: 0,
            },
            value: new Long(494_000),
            sighashType: BitcoinSigHashType.all.value,
            receiverAddress: "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz",
        });

        const out0 = Proto.Output.create({
            value: new Long(488_000),
            toAddress: "t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS",
        });

        const signingInput = Proto.SigningInput.create({
            builder: {
                version: Proto.TransactionVersion.UseDefault,
                inputs: [utxo0],
                outputs: [out0],
                inputSelector: Proto.InputSelector.SelectDescending,
                fixedDustThreshold: dustAmount,
                // Set ZCash specific extra parameters.
                zcashExtraData: {
                    branchId: sapplingBranchId,
                },
            },
            privateKeys: [privateKeyData],
            chainInfo: {
                p2pkhPrefix: 184,
                p2shPrefix: 189,
            },
        });

        const legacySigningInput = TW.Bitcoin.Proto.SigningInput.create({
            signingV2: signingInput,
            coinType: CoinType.zcash.value,
        });

        const encoded = TW.Bitcoin.Proto.SigningInput.encode(legacySigningInput).finish();
        const outputData = AnySigner.sign(encoded, CoinType.zcash);
        const output = TW.Bitcoin.Proto.SigningOutput.decode(outputData);

        assert.equal(output.error, TW.Common.Proto.SigningError.OK);
        assert.equal(output.signingResultV2!.error, TW.Common.Proto.SigningError.OK);
        assert.equal(
            HexCoding.encode(output.signingResultV2!.encoded),
            "0x0400008085202f890153685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a000000006b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6ffffffff0140720700000000001976a91449964a736f3713d64283fd0018626ba50091c7e988ac00000000000000000000000000000000000000"
        );
        assert.equal(
            HexCoding.encode(output.signingResultV2!.txid),
            "0xec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256"
        );
    });
});
