import { expect } from 'chai';
import 'mocha';

import { CoinType, CoinTypeUtil, Purpose, Curve } from '../../lib';

describe('CoinType', () => {

    it('test coin Purpose', () => {
        expect(Purpose.BIP84).to.equal(CoinTypeUtil.purpose(CoinType.BITCOIN));
    });

    it('test coin Curve', () => {
        expect(Curve.SECP256K1).to.equal(CoinTypeUtil.curve(CoinType.BITCOIN));
    });

});
