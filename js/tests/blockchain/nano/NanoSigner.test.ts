import { expect } from 'chai';
import 'mocha';

import { bufToHex, fromHexString } from '../../Utils';
import { TW, NanoSigner, PublicKey } from '../../..';

describe('NanoSigner', () => {

    it('test Nano block signing', () => {
        const signingInput = TW.Nano.Proto.SigningInput.create({
            privateKey: fromHexString('173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34'),
            linkBlock: fromHexString('491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507'),
            representative: 'xrb_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4',
            balance: '96242336390000000000000000000'
        });

        const output: TW.Nano.Proto.SigningOutput = NanoSigner.sign(signingInput);

        expect(bufToHex(output.blockHash)).to.equal('0xf9a323153daefe041efb94d69b9669c882c935530ed953bbe8a665dfedda9696');
        expect(bufToHex(output.signature)).to.equal('0xd247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09');
    });

});
