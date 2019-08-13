import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, IOSTSigner } from '../../..';

describe('IOSTSigner', () => {

    it('test sign IOSTSigner', () => {
        const secKeyBytes = fromHexString('63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4');
        const accountBuilder = TW.IOST.Proto.AccountInfo.create({
            name: 'myname',
            activeKey: secKeyBytes
        });

        const transactionBuilder = TW.IOST.Proto.Transaction.create({
            time: Long.fromString('1550137587000000000'),
            expiration: Long.fromString('1550137587000000000').add(Long.fromInt(1000000000).multiply(300)),
            gasRatio: 1.0,
            gasLimit: 1000000.0,
            chainId: 1024
        });

        const amountLimitBuilder = TW.IOST.Proto.AmountLimit.create({
            token: '*',
            value: 'unlimited'
        });
        transactionBuilder.amountLimit.push(amountLimitBuilder);

        const signInput = TW.IOST.Proto.SigningInput.create({
            transactionTemplate: transactionBuilder,
            account: accountBuilder,
            transferAmount: '10',
            transferDestination: 'admin',
            transferMemo: ''
        });

        const outPut = IOSTSigner.sign(signInput);

        expect(bufToHex(((outPut as TW.IOST.Proto.SigningOutput).transaction as TW.IOST.Proto.Transaction).publisherSigs[0].signature as Uint8Array, false)).to.equal(
            'e8ce15214bad39683021c15dd318e963da8541fd8f3d8484df5042b4ea7fdafb7f46505b85841367d6e1736c7d3b433ca72089b88a23f43661dfb0429a10cb03');
    });

});
