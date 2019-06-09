import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString} from '../../Utils';
import { TW, AeternitySigner } from '../../../lib';

describe('AeternitySigner', () => {

    it('test sign AeternitySigner', () => {
        const privateKey = fromHexString("4646464646464646464646464646464646464646464646464646464646464646");

        const input = TW.Aeternity.Proto.SigningInput.create({
            amount: Long.fromNumber(10),
            fee: Long.fromNumber(2e13),
            fromAddress: "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw",
            nonce: Long.fromNumber(49),
            payload: "Hello World",
            privateKey: privateKey,
            toAddress: "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v",
            ttl: Long.fromNumber(82757),
        });

        const output = AeternitySigner.sign(input);

        expect(output.signature).to.equal("sg_2dlw1eTrh79Yri5+urBSwVMJ86dSvCVtWc/nxIJrhIehxLzbtEdddjNyGJFc700p09KSd01oVTrpoCfiFsFvB3kDla0=");
        expect(output.encoded).to.equal("tx_+KkLAfhCuEDZ2XDV5OuHv1iuLn66sFLBUwnzp1K8JW1Zz+fEgmuEh6HEvNu0R112M3IYkVzvTSnT0pJ3TWhVOumgJ+IWwW8HuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkDZqNSg==");
    });

});
