import {TW} from "../../dist";
import {assert} from "chai";

describe("Cardano", () => {
    it("test outputMinAdaAmount", () => {
        const { Cardano } = globalThis.core;

        const output = TW.Cardano.Proto.TxOutput.create()
        const outputData = TW.Cardano.Proto.TxOutput.encode(output).finish()
        const coinsPerUtxoByte = "4310"
        const toAddress = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"

        const actual = Cardano.outputMinAdaAmount(toAddress, outputData, coinsPerUtxoByte)
        assert.equal(actual, "969750")
    });
});
