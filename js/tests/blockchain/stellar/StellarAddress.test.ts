import { expect } from 'chai';
import 'mocha';

import { bufToHex, fromHexString } from '../../Utils';
import { PrivateKey, StellarAddress, PublicKey } from '../../../lib';

describe('StellarAddress', () => {

    it('test address from PrivateKey', () => {
        const key = PrivateKey.createWithData(fromHexString('59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722'));
        const pubkey = key.getPublicKeyEd25519();
        const address = StellarAddress.createWithPublicKey(pubkey);

        expect(bufToHex(pubkey.data())).to.equal('0x0109A966BCAACC103E38896BAAE3F8C2F06C21FD47DD4F864FF0D33F9819DF5CA2'.toLowerCase());
        expect(address.description()).to.equal('GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI');
    });

    it('test address from PublicKey', () => {
        const pubkey = PublicKey.createWithData(fromHexString('0103E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D'));
        const address = StellarAddress.createWithPublicKey(pubkey);

        expect(address.description()).to.equal('GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ');
    });

});
