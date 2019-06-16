import { expect } from 'chai';
import 'mocha';

import { bufToHex, fromHexString } from '../../Utils';
import { TW, NebulasSigner, PrivateKey } from '../../../lib';

describe('NebulasSigner', () => {
    it('test Nebulas block signing', () => {
        const signingInput = TW.Nebulas.Proto.SigningInput.create({
            chainId: fromHexString('0x1'),  //1
            fromAddress: 'n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY',
            nonce: fromHexString('0x7'),    //7
            gasPrice: fromHexString('0x0f4240'), //1000000
            gasLimit: fromHexString('0x030d40'), //200000
            toAddress: "n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17",
            amount: fromHexString("0x98a7d9b8314c0000"),    //11000000000000000000
            timestamp: fromHexString("0x5cfc84ca"), //1560052938
            payload: new Uint8Array([10,6,98,105,110,97,114,121]),
            privateKey: PrivateKey.createWithData(fromHexString("0xd2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b")).data()
        });
        const output: TW.Nebulas.Proto.SigningOutput = NebulasSigner.sign(signingInput);

        expect(output.alg==1);
        expect(bufToHex(output.sign)).to.equal('0xf53f4a9141ff8e462b094138eccd8c3a5d7865f9e9ab509626c78460a9e0b0fc35f7ed5ba1795ceb81a5e46b7580a6f7fb431d44fdba92515399cf6a8e47e71500');
    });

});
