import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, PrivateKey, WavesSigner } from '../../../lib';

describe('WavesSigner', () => {

    it('test Waves transaction signing', () => {
        const signingInput = TW.Waves.Proto.SigningInput.create({
                amount: Long.fromString('100000000'),
                asset: 'DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq',
                attachment: fromHexString('68656c6c6f'),
                fee: Long.fromString('100000'),
                feeAsset: 'DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq',
                privateKey: fromHexString('68b7a9adb4a655b205f43dac413803785921e22cd7c4d05857b203a62621075f'),
                timestamp: Long.fromString('1559146613'),
                to: '3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx',
        });
        const sign: TW.Waves.Proto.SigningOutput = WavesSigner.sign(signingInput);
        const signBytes = bufToHex(sign.signature);

        expect(signBytes).to.equal('0x5d6a77b1fd9b53d9735cd2543ba94215664f2b07d6c7befb081221fcd49f5b6ad6b9ac108582e8d3e74943bdf35fd80d985edf4b4de1fb1c5c427e84d0879f8f');
    });
});
