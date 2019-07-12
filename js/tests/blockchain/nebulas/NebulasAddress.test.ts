import { expect } from 'chai';
import 'mocha';

import { fromHexString } from '../../Utils';
import { NebulasAddress, PublicKey, PublicKeyType, PrivateKey } from '../../../lib';

describe('NebulasAddress', () => {

    it('test address from String', () => {
        const address = NebulasAddress.createWithString("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
        expect(address.description()).to.equal('n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY');
        const address2 = NebulasAddress.createWithString("n1zUNqeBPvsyrw5zxp9mKcDdLTjuaEL7s39");
        expect(address2.description()).to.equal('n1zUNqeBPvsyrw5zxp9mKcDdLTjuaEL7s39');
    });
    it('test address from PublicKey', () => {
        const data = fromHexString('d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b');
        const privateKey = PrivateKey.createWithData(fromHexString('0xd2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b'));
        const pubkey = privateKey.getPublicKeySecp256k1(false);
        const address = NebulasAddress.createWithPublicKey(pubkey);
        expect(address.description()).to.equal('n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY');
    });

});
