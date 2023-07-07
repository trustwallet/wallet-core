import {TW} from "../../dist";
import {assert} from "chai";
import Long = require("long");

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

    it("test signTransferNft", () => {
        const { AnySigner, CoinType, HexCoding } = globalThis.core;

        const privateKeyData = HexCoding.decode("d09831a668db6b36ffb747600cb1cd3e3d34f36e1e6feefc11b5f988719b7557a7029ab80d3e6fe4180ad07a59ddf742ea9730f3c4145df6365fa4ae2ee49c3392e19444caf461567727b7fefec40a3763bdb6ce5e0e8c05f5e340355a8fef4528dfe7502cfbda49e38f5a0021962d52dc3dee82834a23abb6750981799b75577d1ed9af9853707f0ef74264274e71b2f12e86e3c91314b6efa75ef750d9711b84cedd742ab873ef2f9566ad20b3fc702232c6d2f5d83ff425019234037d1e58");
        const fromAddress = "addr1qy5eme9r6frr0m6q2qpncg282jtrhq5lg09uxy2j0545hj8rv7v2ntdxuv6p4s3eq4lqzg39lewgvt6fk5kmpa0zppesufzjud"
        const toAddress = "addr1qy9wjfn6nd8kak6dd8z53u7t5wt9f4lx0umll40px5hnq05avwcsq5r3ytdp36wttzv4558jaq8lvhgqhe3y8nuf5xrquju7z4"
        // 1.20249 ADA. Amount locked by the NFT.
        const nftInputAmount = new Long(1202490);

        const tokenAmount = TW.Cardano.Proto.TokenAmount.create({
            policyId: "219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5e",
            assetName: "coolcatssociety4567",
            amount: HexCoding.decode("1"),
        });
        const utxo1 = TW.Cardano.Proto.TxInput.create({
            outPoint: TW.Cardano.Proto.OutPoint.create({
                txHash: HexCoding.decode("aba499ec2f23529e70bb256ceaffcc6274a882cf02f29e5670c75ee980d7c2b8"),
                outputIndex: new Long(0),
            }),
            address: fromAddress,
            amount: nftInputAmount,
            tokenAmount: [tokenAmount],
        });

        const utxo2 = TW.Cardano.Proto.TxInput.create({
            outPoint: TW.Cardano.Proto.OutPoint.create({
                txHash: HexCoding.decode("ee414d635b3bc67831907354d274a31174664777c57c21ae923b9459e5644840"),
                outputIndex: new Long(0),
            }),
            address: fromAddress,
            amount: new Long(1000000),
        });

        const utxo3 = TW.Cardano.Proto.TxInput.create({
            outPoint: TW.Cardano.Proto.OutPoint.create({
                txHash: HexCoding.decode("6a7221dcc28353ed69b733391ffeb984a34c1e72293af111d59f9ddfa8639167"),
                outputIndex: new Long(0),
            }),
            address: fromAddress,
            amount: new Long(2000000),
        });

        const transferMessage = TW.Cardano.Proto.Transfer.create({
            toAddress: toAddress,
            changeAddress: fromAddress,
            amount: nftInputAmount,
            tokenAmount: TW.Cardano.Proto.TokenBundle.create({
                token: [tokenAmount],
            }),
        });

        const input = TW.Cardano.Proto.SigningInput.create({
            utxos: [utxo1, utxo2, utxo3],
            privateKey: [privateKeyData],
            ttl: new Long(89130965),
            transferMessage: transferMessage,
        })

        const encoded = TW.Cardano.Proto.SigningInput.encode(input).finish();
        const outputData = AnySigner.sign(encoded, CoinType.cardano);
        const output = TW.Cardano.Proto.SigningOutput.decode(outputData);
        assert.equal(
            HexCoding.encode(output.encoded),
            "0x83a400838258206a7221dcc28353ed69b733391ffeb984a34c1e72293af111d59f9ddfa863916700825820aba499ec2f23529e70bb256ceaffcc6274a882cf02f29e5670c75ee980d7c2b800825820ee414d635b3bc67831907354d274a31174664777c57c21ae923b9459e5644840000182825839010ae9267a9b4f6edb4d69c548f3cba39654d7e67f37ffd5e1352f303e9d63b100507122da18e9cb58995a50f2e80ff65d00be6243cf89a186821a0012593aa1581c219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5ea153636f6f6c63617473736f6369657479343536370182583901299de4a3d24637ef4050033c214754963b829f43cbc311527d2b4bc8e36798a9ada6e3341ac239057e012225fe5c862f49b52db0f5e208731a002b1525021a0002b19b031a055007d5a1008182582088bd26e8656fa7dead846c3373588f0192da5bfb90bf5d3fb877decfb3b3fd085840da8656aca0dacc57d4c2d957fc7dff03908f6dcf60c48f1e40b3006e2fd0cfacfa4c24fa02e35a310572526586d4ce0d30bf660ba274c8efd507848cbe177d09f6"
        );
    });
});
