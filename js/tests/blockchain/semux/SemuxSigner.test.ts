import { expect } from 'chai';
import 'mocha';

import { bufToHex, fromHexString } from '../../Utils';
import { TW, SemuxSigner, PublicKey } from '../../..';
import * as Long from "long";

describe('SemuxSigner', () => {

    it('test Semux transaction signing', () => {
        const signingInput = TW.Semux.Proto.SigningInput.create({
            privateKey: fromHexString('7ea3e3e2ce1e2c4e7696f09a252a1b9d58948bc942c0b42092080a896c43649f'),
            destination: "0xdb7cadb25fdcdd546fb0268524107582c3f8999c",
            value: Long.fromString("123456789"),
            fee: Long.fromString("5000000"),
            nonce: Long.fromString("123"),
            timestamp: Long.fromString("1523028482000")
        });

        const output: TW.Semux.Proto.SigningOutput = SemuxSigner.sign(signingInput);

        expect(bufToHex(output.encoded)).to.equal('0x20e3b076d3d634b9c88b4b2ab281ffd7c440e0eeccf157f2a7cc09c3b7885958c738000114db7cadb25fdcdd546fb0268524107582c3f8999c00000000075bcd1500000000004c4b40000000000000007b000001629b9257d0006064f22068b0e9d103aaae81c099d1d59a44c7ec022550ab8dcccd28104a2a79d27c9dc9a277da765bd5bde2667af78a67a99aa33bf6e352e36546d0285526210e057f987e38f88037e8019cbb774dda106fc051fc4a6320a00294fe1866d08442');
    });

});
