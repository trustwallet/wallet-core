// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import 'mocha'
import { expect } from 'chai'
import { Buffer } from 'buffer'
import { TW, CoinType } from '../dist'

describe('Wallet Core types tests', () => {

    it('test CoinType.ethereum', () => {
        const coin = CoinType.ethereum;
        expect(coin).to.equal(60)
        expect(CoinType.id(coin)).to.equal('ethereum')
        expect(CoinType.name(coin)).to.equal('Ethereum')
        expect(CoinType.slip44(coin)).to.equal(60)
        expect(CoinType.symbol(coin)).to.equal('ETH')
        expect(CoinType.decimals(coin)).to.equal(18)
        expect(CoinType.derivationPath(coin)).to.equal(`m/44'/60'/0'/0/0`)
    })

    it('test CoinType.bsc', () => {
        const coin = CoinType.bsc;
        expect(coin).to.equal(10000714)
        expect(CoinType.id(coin)).to.equal('bsc')
        expect(CoinType.name(coin)).to.equal('Smart Chain Legacy')
        expect(CoinType.slip44(coin)).to.equal(714)
        expect(CoinType.symbol(coin)).to.equal('BNB')
        expect(CoinType.decimals(coin)).to.equal(18)
        expect(CoinType.derivationPath(coin)).to.equal(`m/44'/714'/0'/0/0`)
    })

    it('test CoinType.smartchain', () => {
        const coin = CoinType.smartchain;
        expect(coin).to.equal(20000714)
        expect(CoinType.id(coin)).to.equal('smartchain')
        expect(CoinType.name(coin)).to.equal('Smart Chain')
        expect(CoinType.slip44(coin)).to.equal(714)
        expect(CoinType.symbol(coin)).to.equal('BNB')
        expect(CoinType.decimals(coin)).to.equal(18)
        expect(CoinType.derivationPath(coin)).to.equal(`m/44'/60'/0'/0/0`)
    })

    it('test Ethereum encoding SigningInput', () => {
        const input = TW.Ethereum.Proto.SigningInput.create({
            toAddress: '0x3535353535353535353535353535353535353535',
            chainId: Buffer.from('01', 'hex'),
            nonce: Buffer.from('09', 'hex'),
            gasPrice: Buffer.from('04a817c800', 'hex'),
            gasLimit: Buffer.from('5208', 'hex'),
            transaction: TW.Ethereum.Proto.Transaction.create({
                transfer: TW.Ethereum.Proto.Transaction.Transfer.create({
                    amount: Buffer.from('0de0b6b3a7640000', 'hex')
                })
            }),
            privateKey: Buffer.from('4646464646464646464646464646464646464646464646464646464646464646', 'hex')
        });

        const encoded = TW.Ethereum.Proto.SigningInput.encode(input).finish()
        expect(Buffer.from(encoded).toString('hex')).to.equal("0a01011201091a0504a817c800220252082a2a307833353335333533353335333533353335333533353335333533353335333533353335333533353335322046464646464646464646464646464646464646464646464646464646464646463a0c0a0a0a080de0b6b3a7640000")
    })

    it('test Bitcoin / Bitcoin SigningInput', () => {
        expect(TW.Bitcoin.Proto.SigningInput).not.null;
        expect(TW.Binance.Proto.SigningInput).not.null;
    })
})
