import { expect } from 'chai';
import 'mocha';

import { fromHexString, bufToHex } from '../../Utils';
import { BitcoinScript } from '../../../lib';

describe('BitcoinScript', () => {

    it('test buildPayToPublicKeyHash', () => {
        const data = fromHexString('cf5007e19af3641199f21f3fa54dff2fa2627471');
        const script = BitcoinScript.buildPayToPublicKeyHash(data);
        expect(bufToHex(script.data())).to.equal('0x76a914cf5007e19af3641199f21f3fa54dff2fa262747188ac');
    });

    it('test constructorBuild', () => {
        const pk = fromHexString('cf5007e19af3641199f21f3fa54dff2fa2627471');
        const script = BitcoinScript.buildPayToPublicKeyHash(pk);
        const scriptHash = script.data();
        expect(bufToHex(scriptHash)).to.equal('0x76a914cf5007e19af3641199f21f3fa54dff2fa262747188ac');
        expect(bufToHex(script.scriptHash())).to.equal('0xc470d22e69a2a967f2cec0cd5a5aebb955cdd395');
    });

    it('test redeemScript', () => {
        const data = fromHexString('cf5007e19af3641199f21f3fa54dff2fa2627471');
        const script = BitcoinScript.buildPayToPublicKeyHash(data);
        const scriptHash = script.scriptHash();
        const scriptScript = BitcoinScript.buildPayToScriptHash(scriptHash);
        expect(bufToHex(scriptScript.data())).to.equal('0xa914c470d22e69a2a967f2cec0cd5a5aebb955cdd39587');
    });

});
