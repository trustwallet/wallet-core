import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString } from '../../Utils';
import { TW, PrivateKey, StellarPassphrase, StellarSigner, StellarPassphraseUtil } from '../../..';

describe('StellarTransactionSigner', () => {

    it('test Stellar transaction signing', () => {
        const signingInput = TW.Stellar.Proto.SigningInput.create({
            account: 'GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI',
            amount: Long.fromString('10000000'),
            destination: 'GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52',
            fee: 1000,
            sequence: Long.fromInt(2),
            passphrase: StellarPassphraseUtil.toString(StellarPassphrase.STELLAR),
            operationType: TW.Stellar.Proto.SigningInput.OperationType.PAYMENT,
            memoVoid: TW.Stellar.Proto.MemoVoid.create({}),
            privateKey: PrivateKey.createWithData(fromHexString('59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722')).data()
        });

        const sign: TW.Stellar.Proto.SigningOutput = StellarSigner.sign(signingInput);
        const signBytes = sign.signature;

        expect(signBytes).to.equal('AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAAxYC2MXoOs5v3/NT6PBn9q0uJu6u/YQle5FBa9uzteq4AAAAAAAAAAACYloAAAAAAAAAAARnfXKIAAABAocQZwTnVvGMQlpdGacWvgenxN5ku8YB8yhEGrDfEV48yDqcj6QaePAitDj/N2gxfYD9Q2pJ+ZpkQMsZZG4ACAg==');
    });

    it('test Stellar transaction signing MemoHash', () => {
        const signingInput = TW.Stellar.Proto.SigningInput.create({
            account: 'GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI',
            amount: Long.fromString('10000000'),
            destination: 'GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52',
            fee: 1000,
            sequence: Long.fromInt(2),
            passphrase: StellarPassphraseUtil.toString(StellarPassphrase.STELLAR),
            operationType: TW.Stellar.Proto.SigningInput.OperationType.PAYMENT,
            memoHash: TW.Stellar.Proto.MemoHash.create({ hash: fromHexString('315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3') }),
            privateKey: PrivateKey.createWithData(fromHexString('59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722')).data()
        });

        const sign: TW.Stellar.Proto.SigningOutput = StellarSigner.sign(signingInput);
        const signBytes = sign.signature;

        expect(signBytes).to.equal('AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAMxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAECIyh1BG+hER5W+dgHDKe49X6VEYRWIjajM4Ufq3DUG/yw7Xv1MMF4eax3U0TRi7Qwj2fio/DRD3+/Ljtvip2MD');
    });

    it('test Stellar transaction signing MemoId', () => {
        const signingInput = TW.Stellar.Proto.SigningInput.create({
            account: 'GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI',
            amount: Long.fromString('10000000'),
            destination: 'GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52',
            fee: 1000,
            sequence: Long.fromInt(2),
            passphrase: StellarPassphraseUtil.toString(StellarPassphrase.STELLAR),
            operationType: TW.Stellar.Proto.SigningInput.OperationType.PAYMENT,
            memoId: TW.Stellar.Proto.MemoId.create({ id: Long.fromString('1234567890') }),
            privateKey: PrivateKey.createWithData(fromHexString('59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722')).data()
        });

        const sign: TW.Stellar.Proto.SigningOutput = StellarSigner.sign(signingInput);
        const signBytes = sign.signature;

        expect(signBytes).to.equal('AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEAOJ8wwCizQPf6JmkCsCNZolQeqet2qN7fgLUUQlwx3TNzM0+/GJ6Qc2faTybjKy111rE60IlnfaPeMl/nyxKIB');
    });

    it('test Stellar transaction signing CreateAccount', () => {
        const signingInput = TW.Stellar.Proto.SigningInput.create({
            account: 'GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI',
            amount: Long.fromString('10000000'),
            destination: 'GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52',
            fee: 1000,
            sequence: Long.fromInt(2),
            passphrase: StellarPassphraseUtil.toString(StellarPassphrase.STELLAR),
            operationType: TW.Stellar.Proto.SigningInput.OperationType.CREATE_ACCOUNT,
            memoId: TW.Stellar.Proto.MemoId.create({ id: Long.fromString('1234567890') }),
            privateKey: PrivateKey.createWithData(fromHexString('59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722')).data()
        });

        const sign: TW.Stellar.Proto.SigningOutput = StellarSigner.sign(signingInput);
        const signBytes = sign.signature;

        expect(signBytes).to.equal('AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAAAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAmJaAAAAAAAAAAAEZ31yiAAAAQNgqNDqbe0X60gyH+1xf2Tv2RndFiJmyfbrvVjsTfjZAVRrS2zE9hHlqPQKpZkGKEFka7+1ElOS+/m/1JDnauQg=');
    });

});
