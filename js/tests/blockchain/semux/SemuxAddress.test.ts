import { expect } from 'chai';
import 'mocha';

import { fromHexString } from '../../Utils';
import { SemuxAddress, PublicKey, PublicKeyType } from '../../..';

describe('SemuxAddress', () => {

    it('test address from PublicKey', () => {
        const pubkey = PublicKey.createWithData(fromHexString('b72dc8ebc9f53d21837dc96483da08765ea11f25c1bd4c3cb49318c944d67b9b'), PublicKeyType.ED25519);
        const address = SemuxAddress.createWithPublicKey(pubkey);

        expect(address.description()).to.equal('0x0680a919c78faa59b127014b6181979ae0a62dbd');
    });

});
