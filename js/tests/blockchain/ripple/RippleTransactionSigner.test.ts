import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { bufToHex, fromHexString } from '../../Utils';
import { TW, PrivateKey, RippleSigner } from '../../..';

describe('RippleTransactionSigner', () => {

    it('test Ripple transaction signing', () => {
        const signingInput = TW.Ripple.Proto.SigningInput.create({
            account: 'rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF',
            amount: Long.fromString('29000000'),
            destination: 'rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF',
            fee: Long.fromInt(200000),
            sequence: 1,
            privateKey: PrivateKey.createWithData(fromHexString('ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764')).data()
        });

        const sign: TW.Ripple.Proto.SigningOutput = RippleSigner.sign(signingInput);
        const signBytes = sign.encoded;

        expect(bufToHex(signBytes)).equal('0x12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc72337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb8337e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d');
    });

});
