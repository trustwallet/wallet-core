// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import {assert} from "chai";
import {TW} from "../../dist";
import Long = require("long");

describe("Ripple", () => {
    it("test sign XRP payment", () => {
        const {PrivateKey, HexCoding, AnySigner, CoinType} = globalThis.core;

        const privateKey = PrivateKey.createWithData(
            HexCoding.decode("0xa5576c0f63da10e584568c8d134569ff44017b0a249eb70657127ae04f38cc77")
        );
        const txDataInput = TW.Ripple.Proto.SigningInput.create({
            fee: new Long(10),
            sequence: 32_268_248,
            lastLedgerSequence: 32_268_269,
            account: "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq",
            privateKey: privateKey.data(),
            opPayment: {
                amount: new Long(10),
                destination: "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF",
            },
        });
        const input = TW.Ripple.Proto.SigningInput.encode(txDataInput).finish();
        const outputData = AnySigner.sign(input, CoinType.xrp);
        const output = TW.Ripple.Proto.SigningOutput.decode(outputData);

        assert.equal(output.error, TW.Common.Proto.SigningError.OK);
        // https://testnet.xrpl.org/transactions/A202034796F37F38D1D20F2025DECECB1623FC801F041FC694199C0D0E49A739
        assert.equal(
            HexCoding.encode(output.encoded),
            "0x12000022000000002401ec5fd8201b01ec5fed61400000000000000a68400000000000000a732103d13e1152965a51a4a9fd9a8b4ea3dd82a4eba6b25fcad5f460a2342bb650333f74463044022037d32835c9394f39b2cfd4eaf5b0a80e0db397ace06630fa2b099ff73e425dbc02205288f780330b7a88a1980fa83c647b5908502ad7de9a44500c08f0750b0d9e8481144c55f5a78067206507580be7bb2686c8460adff983148132e4e20aecf29090ac428a9c43f230a829220d"
        );
    });
});
