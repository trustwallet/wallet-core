import { expect } from 'chai';
import 'mocha';
import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, BitcoinTransactionSigner } from '../../..';

describe('BitcoinSigner', () => {

    it('test sign P2WPKH', () => {
        const signerBuilder = TW.Bitcoin.Proto.SigningInput.create({
            amount: Long.fromString('335790000'),
            hashType: 0x01,
            toAddress: '1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx',
            changeAddress: '1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU',
            byteFee: Long.fromString('1')
        });

        // Setting Private Keys
        const utxoKey0 = fromHexString('bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866');
        signerBuilder.privateKey.push(utxoKey0);
        const utxoKey1 = fromHexString('619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9');
        signerBuilder.privateKey.push(utxoKey1);

        // Redeem scripts
        const outpoint0 = TW.Bitcoin.Proto.OutPoint.create({
            hash: fromHexString('fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f'),
            index: 0,
            sequence: Long.MAX_UNSIGNED_VALUE.toInt(),
        });

        const utxo0 = TW.Bitcoin.Proto.UnspentTransaction.create({
            amount: Long.fromString('625000000'),
            outPoint: outpoint0,
            script: fromHexString('2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac'),
        });

        signerBuilder.utxo.push(utxo0);

        const outpoint1 = TW.Bitcoin.Proto.OutPoint.create({
            hash: fromHexString('ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a'),
            index: 1,
            sequence: Long.MAX_UNSIGNED_VALUE.toInt()
        });

        const utxo1 = TW.Bitcoin.Proto.UnspentTransaction.create({
            amount: Long.fromString('600000000'),
            outPoint: outpoint1,
            script: fromHexString('00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1')
        });

        signerBuilder.utxo.push(utxo1);

        const signer = BitcoinTransactionSigner.create(signerBuilder);
        const result = signer.sign();
        expect(result.success).to.be.true;
        expect(result.objects).to.have.length(1);

        const output = TW.Bitcoin.Proto.SigningOutput.decode(result.objects[0].value as Uint8Array);
        expect(output).to.not.be.null;

        let signedTransaction = output.transaction;
        expect(signedTransaction).to.not.be.null;
        expect(signedTransaction).to.not.be.undefined;
        signedTransaction = signedTransaction as TW.Bitcoin.Proto.Transaction;

        expect(signedTransaction.version).to.equal(1);
        expect(signedTransaction.inputs as TW.Bitcoin.Proto.ITransactionInput).to.have.length(1);
        expect(signedTransaction.outputs as TW.Bitcoin.Proto.ITransactionOutput).to.have.length(2);

        const encoded = output.encoded;
        expect(bufToHex(encoded)).to.equal('0x0100000001fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100b6006eb0fe2da8cbbd204f702b1ffdb1e29c49f3de51c4983d420bf9f9125635022032a195b153ccb2c4978333b4aad72aaa7e6a0b334a14621d5d817a42489cb0d301ffffffff02b0bf0314000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088acaefd3c11000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000');
    });

    it('test sign P2PKH', () => {
        const signerBuilder = TW.Bitcoin.Proto.SigningInput.create({
            amount: Long.fromString('55000'),
            hashType: 0x01,
            toAddress: '1GDCMHsTLBkawQXP8dqcZtr8zGgb4XpCug',
            changeAddress: '1CSR6tXqngr1CfwVF23V4bQotttJmzXqpf',
            byteFee: Long.fromString('10')
        });

        // Setting Private Keys
        const utxoKey0 = fromHexString('bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866');
        signerBuilder.privateKey.push(utxoKey0);
        const utxoKey1 = fromHexString('619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9');
        signerBuilder.privateKey.push(utxoKey1);

        // Redeem scripts
        const outpoint0 = TW.Bitcoin.Proto.OutPoint.create({
            hash: fromHexString('fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f'),
            index: 0,
            sequence: Long.MAX_UNSIGNED_VALUE.toInt(),
        });

        const utxo0 = TW.Bitcoin.Proto.UnspentTransaction.create({
            amount: Long.fromString('30000'),
            outPoint: outpoint0,
            script: fromHexString('2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac'),
        });

        signerBuilder.utxo.push(utxo0);

        const outpoint1 = TW.Bitcoin.Proto.OutPoint.create({
            hash: fromHexString('ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a'),
            index: 1,
            sequence: Long.MAX_UNSIGNED_VALUE.toInt()
        });

        const utxo1 = TW.Bitcoin.Proto.UnspentTransaction.create({
            amount: Long.fromString('45000'),
            outPoint: outpoint1,
            script: fromHexString('00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1')
        });

        signerBuilder.utxo.push(utxo1);

        const signer = BitcoinTransactionSigner.create(signerBuilder);
        const result = signer.sign();
        expect(result.success).to.be.true;
        expect(result.objects).to.have.length(1);

        const output = TW.Bitcoin.Proto.SigningOutput.decode(result.objects[0].value as Uint8Array);
        expect(output).to.not.be.null;

        let signedTransaction = output.transaction;
        expect(signedTransaction).to.not.be.null;
        expect(signedTransaction).to.not.be.undefined;
        signedTransaction = signedTransaction as TW.Bitcoin.Proto.Transaction;

        expect(signedTransaction.version).to.equal(1);
        expect(signedTransaction.inputs as TW.Bitcoin.Proto.ITransactionInput).to.have.length(2);
        expect(signedTransaction.outputs as TW.Bitcoin.Proto.ITransactionOutput).to.have.length(2);

        const encoded = output.encoded;
        expect(bufToHex(encoded)).to.equal('0x01000000000102fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100991ea84c8f22cbcbdee114a687b31bc80fca181161adc354e37b16b0f4664a6f022016e34b232524a1296a636026f8bb1f5f3635d88bf936532aae70a499c52f77d201ffffffffef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a0100000000ffffffff02d8d60000000000001976a914a6d85a488bb777a540f24bf777d30d1486036f6188ac843f0000000000001976a9147d77e6cfb05a9cfc123824279f6caf8b66ac267688ac000247304402200ebd8fe637d7344984dd173d4a3089c4fc03a51117ee0363d04c714f033b33cf02204e2831939fff068068cc08fe35d84950f244fd2fe39795d839bfb8795484cc230121025476c2e83188368da1ff3e292e7acafcdb3566bb0ad253f62fc70f07aeee635700000000');
    });

});
