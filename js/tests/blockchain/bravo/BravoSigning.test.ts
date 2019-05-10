import { expect, AssertionError } from 'chai';
import 'mocha';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, BravoSigner, Hash } from '../../../lib';

describe('BravoSigning', () => {

    it('bravo transaction signing', () => {
        const bravoSigningInput: TW.Bravo.Proto.SigningInput = TW.Bravo.Proto.SigningInput.create({
            chainId: new Uint8Array(32),
            sender: 'alice',
            recipient: 'bob',
            amount: 90.0,
            testnet: false,
            memo: 'Eva',
            referenceBlockId: fromHexString('0000086bf9e7704509aa41311a66fa0a1b479c6b'),
            referenceBlockTime: 1552464180,
            privateKey: Hash.sha256(fromHexString('A'))
        });

        const result: TW.Proto.Result = BravoSigner.sign(bravoSigningInput);

        if (!result.success) {
            console.log('Error signing:', result.error);
        }

        expect(result.success).to.be.true;
        expect(result.objects).to.have.length(1);
        expect(result.objects[0].value).to.not.be.undefined;

        const signingOutput: TW.Bravo.Proto.SigningOutput = TW.Bravo.Proto.SigningOutput.decode(result.objects[0].value as Uint8Array);

        let jsonObj;
        try {
            jsonObj = JSON.parse(signingOutput.jsonEncoded);
        } catch (e) {
            throw new AssertionError('Error parsing json:', e);
        }
        
        const signatures = jsonObj.signatures;
        expect(signatures).to.not.be.null;
        expect(signatures).to.not.be.undefined;

        const signatureValue: string = signatures[0] as string;
        expect(signatureValue).to.not.be.null;
        expect(signatureValue).to.not.be.undefined;

        expect(signatureValue).to.equal('1f05b3dddfcbca5748b02562645fe7c3f01044b78983ce673f84c230b2dc97beed19b2e8462f1e45f2ac7600c2900f9f90510efe0891141ad2c6b1ae33b21bcace');
    });

});
