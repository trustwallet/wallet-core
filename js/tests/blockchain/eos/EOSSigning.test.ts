import { expect, AssertionError } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, BravoSigner, Hash, CoinType, CoinTypeUtil, EOSSigner } from '../../../lib';

describe('EOSSigning', () => {

    it('test valid addresses', () => {
        expect(CoinTypeUtil.validate(CoinType.EOS, 'EOS6Vm7RWMS1KKAM9kDXgggpu4sJkFMEpARhmsWA84tk4P22m29AV')).to.be.true;
        expect(CoinTypeUtil.validate(CoinType.EOS, 'PUB_R1_6pQRUVU5vdneRnmjSiZPsvu3zBqcptvg6iK2Vz4vKo4ugnzow3')).to.be.true;
    });

    it('test invalid addresses', () => {
        let addr = [
            'abc',
            '65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF',
            'EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT',
            'PUB_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe',
            'PUB_K1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe',
            'PUB_K1_65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF'
        ];

        addr.forEach((value) => {
            expect(CoinTypeUtil.validate(CoinType.EOS, value)).to.be.false;
        });
    });

    function getAssetBuilder(amt: Long, decimal: number, sym: string): TW.EOS.Proto.Asset {
        return TW.EOS.Proto.Asset.create({
            amount: amt,
            decimals: decimal,
            symbol: sym
        });
    }

    it('eos transaction signing', () => {
        const signingInput: TW.EOS.Proto.SigningInput = TW.EOS.Proto.SigningInput.create({
            chainId: fromHexString('cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f'),
            referenceBlockId: fromHexString('000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144'),
            referenceBlockTime: 1554209118,
            currency: 'token',
            sender: 'token',
            recipient: 'eosio',
            memo: 'my second transfer',
            asset: getAssetBuilder(Long.fromInt(300000), 4, 'TKN'),
            privateKey: Hash.sha256(Buffer.from('A')),
            privateKeyType: TW.EOS.Proto.KeyType.MODERNK1
        });

        const result = EOSSigner.sign(signingInput);
        expect(result.success).to.be.true;
        expect(result.objects).to.have.length(1);

        const signingOutput: TW.EOS.Proto.SigningOutput = TW.EOS.Proto.SigningOutput.decode(result.objects[0].value as Uint8Array);
        const jsonObj = JSON.parse(signingOutput.jsonEncoded);
        expect(jsonObj).to.not.be.null;

        const signatures = jsonObj.signatures;
        expect(signatures).to.not.be.null;

        const signatureValue: string = signatures[0] as string;
        expect(signatureValue).to.not.be.null;
        expect(signatureValue).to.not.be.undefined;

        expect(signatureValue).to.equal('SIG_K1_KfCdjsrTnx5cBpbA5cUdHZAsRYsnC9uKzuS1shFeqfMCfdZwX4PBm9pfHwGRT6ffz3eavhtkyNci5GoFozQAx8P8PBnDmj');
    });

    it('eos test failures', () => {
        const goodInput: TW.EOS.Proto.SigningInput = TW.EOS.Proto.SigningInput.create({
            chainId: fromHexString('cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f'),
            referenceBlockId: fromHexString('000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144'),
            referenceBlockTime: 1554209118,
            currency: 'token',
            sender: 'token',
            recipient: 'eosio',
            memo: 'my second transfer',
            asset: getAssetBuilder(Long.fromInt(300000), 4, 'TKN'),
            privateKey: Hash.sha256(Buffer.from('A')),
            privateKeyType: TW.EOS.Proto.KeyType.MODERNK1
        });

        let badinput = goodInput;
        badinput.asset = getAssetBuilder(Long.fromInt(300000), 19, 'TKN');
        let result = EOSSigner.sign(badinput);
        expect(result.success).to.be.false;

        badinput = goodInput;
        badinput.asset = getAssetBuilder(Long.fromInt(300000), 4, 'xyz');
        result = EOSSigner.sign(badinput);
        expect(result.success).to.be.false;

        badinput = goodInput;
        badinput.recipient = 'A'.repeat(15)
        result = EOSSigner.sign(badinput)
        expect(result.success).to.be.false;

        badinput = goodInput;
        badinput.referenceBlockId = fromHexString('0000086bf9e7704509aa41311a66fa0a1b479c');
        result = EOSSigner.sign(badinput);
        expect(result.success).to.be.false;
    });
});
