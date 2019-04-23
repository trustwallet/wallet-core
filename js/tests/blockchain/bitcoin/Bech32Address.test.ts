import { expect } from 'chai';
import 'mocha';

import { fromHexString } from '../../Utils';
import { PublicKey, Bech32Address, HRP } from '../../../lib';

describe('Bech32Address', () => {

    it('test from public', () => {
        const data = fromHexString('0x02f1e733ed6030cc569c4323a34b17e192d58107d9ffbce71c8420b779f484dba1');
        const publicKey = PublicKey.createWithData(data);
        const address = Bech32Address.createWithPublicKey(HRP.BITCOIN, publicKey);
        expect(address.description()).to.equal('bc1qrq6gs660qewd282en83n6em9s4rlslj3cd2wmg');
    });

});
