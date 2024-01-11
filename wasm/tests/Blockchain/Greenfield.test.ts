// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { TW } from "../../dist";
import Long = require("long");

describe("Greenfield", () => {

    // Successfully broadcasted: https://greenfieldscan.com/tx/ED8508F3C174C4430B8EE718A6D6F0B02A8C516357BE72B1336CF74356529D19
    it("test signing transfer tx", () => {
        const { HexCoding, AnySigner, CoinType } = globalThis.core;

        const msgSend = TW.Greenfield.Proto.Message.create({
            sendCoinsMessage: TW.Greenfield.Proto.Message.Send.create({
                fromAddress: "0xA815ae0b06dC80318121745BE40e7F8c6654e9f3",
                toAddress: "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0",
                amounts: [TW.Greenfield.Proto.Amount.create({
                    amount: "1234500000000000",
                    denom: "BNB",
                })]
            }),
        });
        const input = TW.Greenfield.Proto.SigningInput.create({
            signingMode: TW.Greenfield.Proto.SigningMode.Eip712,
            encodingMode: TW.Greenfield.Proto.EncodingMode.Protobuf,
            accountNumber: new Long(15952),
            cosmosChainId: "greenfield_5600-1",
            ethChainId: "5600",
            sequence: new Long(0),
            mode: TW.Greenfield.Proto.BroadcastMode.SYNC,
            memo: "Trust Wallet test memo",
            messages: [msgSend],
            fee: TW.Greenfield.Proto.Fee.create({
                amounts: [TW.Greenfield.Proto.Amount.create({
                    amount: "6000000000000",
                    denom: "BNB",
                })],
                gas: new Long(1200),
            }),
            privateKey: HexCoding.decode(
                "825d2bb32965764a98338139412c7591ed54c951dd65504cd8ddaeaa0fea7b2a"
            ),
        });

        const encoded = TW.Greenfield.Proto.SigningInput.encode(input).finish();

        const outputData = AnySigner.sign(encoded, CoinType.greenfield);
        const output = TW.Greenfield.Proto.SigningOutput.decode(outputData);
        assert.equal(
            output.serialized,
            "{\"mode\":\"BROADCAST_MODE_SYNC\",\"tx_bytes\":\"CqwBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4QTgxNWFlMGIwNmRDODAzMTgxMjE3NDVCRTQwZTdGOGM2NjU0ZTlmMxIqMHg4ZGJENmM3RWRlOTA2NDZhNjFCYmM2NDk4MzFiN2MyOThCRmQzN0EwGhcKA0JOQhIQMTIzNDUwMDAwMDAwMDAwMBIWVHJ1c3QgV2FsbGV0IHRlc3QgbWVtbxJzClYKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAhm/mQgs8vzaqBLW66HrqQNv86PYTBgXyElU1OiuKD/sEgUKAwjIBRIZChQKA0JOQhINNjAwMDAwMDAwMDAwMBCwCRpBwbRb1qEAWwaqVfmp1Mn7iMi7wwV/oPi2J2eW9NBIdNoky+ZL+uegS/kY+funCOrqVZ+Kbol9/djAV+bQaNUB0xw=\"}"
        );
    });
});
