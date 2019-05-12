import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, ARKSigner } from '../../../lib';

describe('ARKSigner', () => {

    it('test sign ARKSigner', () => {
        let privateKey = fromHexString("a76fa06ec56a720cdaa4b4764c7685833c7ffbcc03d193963c01d6890388376f");
        let amount = Long.fromNumber(1000000);
        let timestamp = 1557956100;
        let recipientId = "ARjdsayeC5q6xRo432Ru3F9Zcb73A5dfME";
        
        let input = TW.ARK.Proto.SigningInput.create({
            privateKey: privateKey,
            amount: amount,
            timestamp: timestamp,
            recipientId: recipientId
        });

        let output = ARKSigner.sign(input);
        let outputObj = JSON.parse(output.json);
        expect(outputObj.signature).to.equal("304402201622f95ee73fd645763ebefbad87f74352028cd11ca313b739e6bdef243804d902201d7897acc12bd031d90b9f850a05a195679747f049b3c15cfe40772634687224");
        expect(outputObj.senderPublicKey).to.equal("02ceb971ddabd391df62f4762ba45def3e2b36d002d8344ebbb4be9a941071c332");
    });

});