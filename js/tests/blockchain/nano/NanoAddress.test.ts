import { expect } from 'chai';
import 'mocha';

import { fromHexString } from '../../Utils';
import { NanoAddress, PublicKey, PublicKeyType } from '../../../lib';

describe('NanoAddress', () => {

    it('test address from PublicKey', () => {
        const pubkey = PublicKey.createWithData(fromHexString('5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50'), PublicKeyType.ED25519BLAKE2B);
        const address = NanoAddress.createWithPublicKey(pubkey);

        expect(address.description()).to.equal('nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg');
    });

});
