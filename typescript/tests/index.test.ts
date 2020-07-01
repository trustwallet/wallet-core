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
        expect(CoinType.symbol(coin)).to.equal('ETH')
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
            amount: Buffer.from('0de0b6b3a7640000', 'hex'),
            privateKey: Buffer.from('4646464646464646464646464646464646464646464646464646464646464646', 'hex')
        });

        const encoded = TW.Ethereum.Proto.SigningInput.encode(input).finish()
        expect(Buffer.from(encoded).toString('hex')).to.equal("0a01011201091a0504a817c800220252082a2a30783335333533353335333533353335333533353335333533353335333533353335333533353335333532080de0b6b3a764000042204646464646464646464646464646464646464646464646464646464646464646")
    })
})
