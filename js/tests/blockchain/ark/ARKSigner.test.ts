import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, ARKSigner } from '../../..';

describe('ARKSigner', () => {

    it('test sign ARKSigner', () => {
        let privateKey = fromHexString("d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712");

        let input = TW.ARK.Proto.SigningInput.create({
            type: TW.ARK.Proto.TransactionType.Transfer,
            privateKey: privateKey,
            amount: Long.fromNumber(123123123),
            fee: Long.fromNumber(10000000),
            timestamp: 67447770,
            toAddress: "ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU"
        });

        let output = ARKSigner.sign(input);

        expect(bufToHex(output.signature)).to.equal("0x304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3");
        expect(output.encoded).to.equal("{\"amount\":123123123,\"asset\":{},\"fee\":10000000,\"id\":\"219b1cc99ec804df02230a9e913ccb45edb7819f22328e3cd15030174a8c4167\",\"recipientId\":\"ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU\",\"senderPublicKey\":\"034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192\",\"signature\":\"304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3\",\"timestamp\":67447770,\"type\":0}");
    });

});
