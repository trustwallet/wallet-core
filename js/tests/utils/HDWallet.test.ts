import { expect } from 'chai';
import 'mocha';

import { bufToHex } from '../Utils';
import { HDWallet, CoinType, Purpose, HDVersion } from '../../lib';

describe('Bech32Address', () => {

    const words = 'ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal';
    const password = 'TREZOR';

    it('test seed', () => {
        const wallet = HDWallet.createWithMnemonic(words, password);
        expect(bufToHex(wallet.seed())).to.equal('0x7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29');
    });

    it('test seed no password', () => {
        const wallet = HDWallet.createWithMnemonic(words, '');
        expect(bufToHex(wallet.seed())).to.equal('0x354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d');
    });

    it('test derive Ethereum', () => {
        const wallet = HDWallet.createWithMnemonic(words, password);

        const key = wallet.getKeyForCoin(CoinType.ETHEREUM);

        const publicKey = key.getPublicKeySecp256k1(false);
        const publicKeyData = bufToHex(publicKey.data());

        expect(publicKeyData).to.equal('0x0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011');
    });

    it('test derive Bitcoin', () => {
        const wallet = HDWallet.createWithMnemonic(words, password);

        const key = wallet.getKeyForCoin(CoinType.BITCOIN);

        const publicKey = key.getPublicKeySecp256k1(false);
        const publicKeyData = publicKey.data();

        expect(bufToHex(publicKeyData)).to.equal('0x047ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b9414ec07cda7b1c9a84c2e04ea2746c21afacc5e91b47427c453c3f1a4a3e983ce5');
    });

    it('test extended keys', () => {
        const words = 'abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about';
        const wallet = HDWallet.createWithMnemonic(words, '');

        const xprv = wallet.getExtendedPrivateKey(Purpose.BIP44, CoinType.BITCOIN, HDVersion.XPRV);
        const xpub = wallet.getExtendedPublicKey(Purpose.BIP44, CoinType.BITCOIN, HDVersion.XPUB);

        expect(xprv).to.equal('xprv9xpXFhFpqdQK3TmytPBqXtGSwS3DLjojFhTGht8gwAAii8py5X6pxeBnQ6ehJiyJ6nDjWGJfZ95WxByFXVkDxHXrqu53WCRGypk2ttuqncb');
        expect(xpub).to.equal('xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj');

        const yprv = wallet.getExtendedPrivateKey(Purpose.BIP49, CoinType.BITCOIN, HDVersion.YPRV);
        const ypub = wallet.getExtendedPublicKey(Purpose.BIP49, CoinType.BITCOIN, HDVersion.YPUB);
        expect(yprv).to.equal('yprvAHwhK6RbpuS3dgCYHM5jc2ZvEKd7Bi61u9FVhYMpgMSuZS613T1xxQeKTffhrHY79hZ5PsskBjcc6C2V7DrnsMsNaGDaWev3GLRQRgV7hxF');
        expect(ypub).to.equal('ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP');

        const zprv = wallet.getExtendedPrivateKey(Purpose.BIP84, CoinType.BITCOIN, HDVersion.ZPRV);
        const zpub = wallet.getExtendedPublicKey(Purpose.BIP84, CoinType.BITCOIN, HDVersion.ZPUB);
        expect(zprv).to.equal('zprvAdG4iTXWBoARxkkzNpNh8r6Qag3irQB8PzEMkAFeTRXxHpbF9z4QgEvBRmfvqWvGp42t42nvgGpNgYSJA9iefm1yYNZKEm7z6qUWCroSQnE');
        expect(zpub).to.equal('zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs');
    });

    it('test PublicKey from X', () => {
        const xpub = 'xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj';
        const xpubAddr2 = HDWallet.getPublicKeyFromExtended(xpub, CoinType.BITCOIN, HDVersion.XPUB, HDVersion.XPRV, 0, 2);
        const xpubAddr9 = HDWallet.getPublicKeyFromExtended(xpub, CoinType.BITCOIN, HDVersion.XPUB, HDVersion.XPRV, 0, 9);

        expect(bufToHex(xpubAddr2.data()), '0x0338994349b3a804c44bbec55c2824443ebb9e475dfdad14f4b1a01a97d42751b3');
        expect(bufToHex(xpubAddr9.data()), '0x03786c1d274f2c804ff9a57d8e7289c281d4aef15e17187ad9f9c3722d81a6ae66');
    });

    it('test init HDWallet', () => {
        const hdWallet1 = HDWallet.create(160, '');
        expect(hdWallet1.mnemonic()).to.not.null;
        const wordList1 = hdWallet1.mnemonic().split(' ');
        expect(HDWallet.isValid(wordList1.join(' '))).to.be.true;

        const hdWallet2 = HDWallet.create(160, '');
        expect(hdWallet2.mnemonic()).to.not.null;
        const wordList2 = hdWallet2.mnemonic().split(' ');
        expect(HDWallet.isValid(wordList2.join(' '))).to.be.true;
    });

    it('test init HDWallet with phrase', () => {
        const hdWallet = HDWallet.createWithMnemonic(words, '');
        expect(hdWallet.mnemonic()).to.equal(words);
    });

});
