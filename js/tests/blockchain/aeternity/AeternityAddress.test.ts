import {expect} from 'chai';
import 'mocha';

import {fromHexString} from '../../Utils';
import {PublicKey, PublicKeyType, AeternityAddress} from '../../../lib';

describe('AeternityAddress', () => {

    it('test address from PublicKey', () => {
        const pubkey = PublicKey.createWithData(fromHexString('ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148'), PublicKeyType.ED25519);
        const address = AeternityAddress.createWithPublicKey(pubkey);

        expect(address.description()).to.equal('ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw');
    });

    it('test address from String', () => {
        const address = AeternityAddress.createWithString("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");
        expect(address.description()).to.equal('ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw');
    });

});
