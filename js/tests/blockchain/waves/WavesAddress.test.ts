import { expect } from 'chai';
import 'mocha';

import { bufToHex, fromHexString } from '../../Utils';
import { PrivateKey, WavesAddress, PublicKey, PublicKeyType } from '../../../lib';

describe('WavesAddress', () => {

    it('test address from PrivateKey', () => {
        const key = PrivateKey.createWithData(fromHexString('9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a'));
        const pubkey = key.getPublicKeyCurve25519();
        const address = WavesAddress.createWithPublicKey(pubkey);

        expect(bufToHex(pubkey.data())).to.equal('0x559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d'.toLowerCase());
        expect(address.description()).to.equal('3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds');
    });

    it('test address from PublicKey', () => {
        const pubkey = PublicKey.createWithData(fromHexString('559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d'), PublicKeyType.CURVE25519);
        const address = WavesAddress.createWithPublicKey(pubkey);

        expect(address.description()).to.equal('3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds');
    });

});
