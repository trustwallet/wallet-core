import { expect } from 'chai';
import 'mocha';

import { fromHexString, bufToHex } from '../../Utils';
import { EthereumSigner, TW, PrivateKey } from '../../../lib';

describe('EthereumTransactionSigner', () => {

    it('test ethereum transaction signing', () => {

        const signingInput = TW.Ethereum.Proto.SigningInput.create({
            privateKey: PrivateKey.createWithData(fromHexString('0x4646464646464646464646464646464646464646464646464646464646464646')).data(),
            toAddress: '0x3535353535353535353535353535353535353535',
            chainId: fromHexString('0x1'),
            nonce: fromHexString('0x9'),
            gasPrice: fromHexString('0x04a817c800'),
            gasLimit: fromHexString('0x5208'),
            amount: fromHexString('0x0de0b6b3a7640000'),
        });

        const sign: TW.Ethereum.Proto.SigningOutput = EthereumSigner.sign(signingInput);

        expect(bufToHex(sign.v)).to.equal('0x25');
        expect(bufToHex(sign.r)).to.equal('0x28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276');
        expect(bufToHex(sign.s)).to.equal('0x67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83');
    });

});
