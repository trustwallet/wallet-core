import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, PrivateKey, CosmosAddress, HRP, BinanceSigner } from '../../..';

describe('EthereumTransactionSigner', () => {

    it('test sign Binance transaction', () => {
        const privateKey = PrivateKey.createWithData(fromHexString('95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832'));
        const publicKey = privateKey.getPublicKeySecp256k1(true);

        const signingInput = TW.Binance.Proto.SigningInput.create({
            chainId: 'Binance-Chain-Nile',
            accountNumber: Long.fromInt(0),
            sequence: Long.fromInt(0)
        });

        signingInput.privateKey = privateKey.data();

        const token = TW.Binance.Proto.SendOrder.Token.create({
            denom: 'BNB',
            amount: Long.fromInt(1)
        });

        const input = TW.Binance.Proto.SendOrder.Input.create({
            address: CosmosAddress.createWithPublicKey(HRP.BINANCE, publicKey).keyHash(),
        });
        input.coins.push(token);

        const output =  TW.Binance.Proto.SendOrder.Output.create({
            address: CosmosAddress.createWithString('bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5').keyHash()
        });
        output.coins.push(token);

        const sendOrder = TW.Binance.Proto.SendOrder.create({});
        sendOrder.inputs.push(input);
        sendOrder.outputs.push(output);

        signingInput.sendOrder = sendOrder;

        const sign: TW.Binance.Proto.SigningOutput = BinanceSigner.sign(signingInput);
        const signBytes = sign.encoded;
        expect(bufToHex(signBytes)).to.equal('0xb801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679');
    });

});
