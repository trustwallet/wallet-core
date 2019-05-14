import { expect } from 'chai';
import 'mocha';

import { CoinType, CoinTypeUtil, HDWallet } from '../../lib';

describe('CoinAddressDerivation', () => {

    it('test derive addresses from phrase', () => {
        const wallet = HDWallet.createWithMnemonic('shoot island position soft burden budget tooth cruel issue economy destroy above', '');

        for (let i = 0; i < 4; ++i) {
            for (const coin in CoinType) {
                const coinType = parseInt(coin, 10);
                if (coinType) {
                    const privateKey = wallet.getKeyForCoin(coinType);
                    const address = CoinTypeUtil.deriveAddress(coinType, privateKey);
                    runDerivationChecks(coinType, address);
                }
            }
        }
    });

    const runDerivationChecks = (coin: CoinType, address: string) => {
        switch (coin) {
            case CoinType.BINANCE: expect('bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw').to.equal(address); break;
            case CoinType.BITCOIN: expect('bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d').to.equal(address); break;
            case CoinType.BITCOINCASH: expect('bitcoincash:qpzl3jxkzgvfd9flnd26leud5duv795fnv7vuaha70').to.equal(address); break;
            case CoinType.CALLISTO: expect('0x3E6FFC80745E6669135a76F4A7ce6BCF02436e04').to.equal(address); break;
            case CoinType.DASH: expect('XqHiz8EXYbTAtBEYs4pWTHh7ipEDQcNQeT').to.equal(address); break;
            case CoinType.ELLAISM: expect('0x1Ae593CAdb8BAC47D0394617396650474c303F9C').to.equal(address); break;
            case CoinType.ETHEREUM: expect('0x8f348F300873Fd5DA36950B2aC75a26584584feE').to.equal(address); break;
            case CoinType.ETHEREUMCLASSIC: expect('0x078bA3228F3E6C08bEEac9A005de0b7e7089aD1c').to.equal(address); break;
            case CoinType.ETHERSOCIAL: expect('0x182dd55D97C1F8D3781CDaDFC275948Ac38Ea1fe').to.equal(address); break;
            case CoinType.GOCHAIN: expect('0x5940ce4A14210d4Ccd0ac206CE92F21828016aC2').to.equal(address); break;
            case CoinType.GROESTLCOIN: expect('grs1qexwmshts5pdpeqglkl39zyl6693tmfwp0cue4j').to.equal(address); break;
            case CoinType.ICON: expect('hx18b380b53c23dc4ee9f6666bc20d1be02f3fe106').to.equal(address); break;
            case CoinType.LITECOIN: expect('ltc1qhd8fxxp2dx3vsmpac43z6ev0kllm4n53t5sk0u').to.equal(address); break;
            case CoinType.ONTOLOGY: expect('AHKTnybvnWo3TeY8uvNXekvYxMrXogUjeT').to.equal(address); break;
            case CoinType.POANETWORK: expect('0xe8a3e8bE17E172B6926130eAfB521e9D2849aca9').to.equal(address); break;
            case CoinType.XRP: expect('rPwE3gChNKtZ1mhH3Ko8YFGqKmGRWLWXV3').to.equal(address); break;
            case CoinType.TEZOS: expect('tz1acnY9VbMagps26Kj3RfoGRWD9nYG5qaRX').to.equal(address); break;
            case CoinType.THUNDERTOKEN: expect('0x4b92b3ED6d8b24575Bf5ce4C6a86ED261DA0C8d7').to.equal(address); break;
            case CoinType.TOMOCHAIN: expect('0xC74b6D8897cBa9A4b659d43fEF73C9cA852cE424').to.equal(address); break;
            case CoinType.TRON: expect('TQ5NMqJjhpQGK7YJbESKtNCo86PJ89ujio').to.equal(address); break;
            case CoinType.VECHAIN: expect('0x1a553275dF34195eAf23942CB7328AcF9d48c160').to.equal(address); break;
            case CoinType.WANCHAIN: expect('0xd5CA90B928279fe5d06144136A25dEd90127Ac15').to.equal(address); break;
            case CoinType.XDAI: expect('0x364d0551599B97EAf997bc06c8c40Aaf73124402').to.equal(address); break;
            case CoinType.ZCASH: expect('t1YYnByMzdGhQv3W3rnjHMrJs6HH4Y231gy').to.equal(address); break;
            case CoinType.ZCOIN: expect('aEd5XFChyXobvEics2ppAqgK3Bgusjxtik').to.equal(address); break;
            case CoinType.NIMIQ: expect('NQ76 7AVR EHDA N05U X7SY XB14 XJU7 8ERV GM6H').to.equal(address); break;
            case CoinType.STELLAR: expect('GA3H6I4C5XUBYGVB66KXR27JV5KS3APSTKRUWOIXZ5MVWZKVTLXWKZ2P').to.equal(address); break;
            case CoinType.AION: expect('0xa0629f34c9ea4757ad0b275628d4d02e3db6c9009ba2ceeba76a5b55fb2ca42e').to.equal(address); break;
            case CoinType.NANO: expect('nano_39gsbcishxn3n7wd17ono4otq5wazwzusqgqigztx73wbrh5jwbdbshfnumc').to.equal(address); break;
            case CoinType.NEO: expect('AUa3rzbGJe7MbHf8Kra9em8oaLBL1MDYKF').to.equal(address); break;
            case CoinType.THETA: expect('0x0d1fa20c218Fec2f2C55d52aB267940485fa5DA4').to.equal(address); break;
            case CoinType.COSMOS: expect('cosmos142j9u5eaduzd7faumygud6ruhdwme98qsy2ekn').to.equal(address); break;
            case CoinType.DECRED: expect('DsidJiDGceqHTyqiejABy1ZQ3FX4SiWZkYG').to.equal(address); break;
            case CoinType.DOGECOIN: expect('DJRFZNg8jkUtjcpo2zJd92FUAzwRjitw6f').to.equal(address); break;
            case CoinType.KIN: expect('GBL3MT2ICHHM5OJ2QJ44CAHGDK6MLPINVXBKOKLHGBWQDVRWTWQ7U2EA').to.equal(address); break;
            case CoinType.VIACOIN: expect('via1qnmsgjd6cvfprnszdgmyg9kewtjfgqflz67wwhc').to.equal(address); break;
            case CoinType.IOST: expect('EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin').to.equal(address); break;
            case CoinType.LUX: expect('LYL6SZG8S6dyXRFT8Bw4FHUoVef3cWCoPi').to.equal(address); break;
            case CoinType.QTUM: expect('QhceuaTdeCZtcxmVc6yyEDEJ7Riu5gWFoF').to.equal(address); break;
            case CoinType.NULS: expect('NsdtNvsfmPerWk4BhcapHTB3LptF8Sbe').to.equal(address); break;
            case CoinType.ZILLIQA: expect('0xDdb41006F7B6FA8e5FBF06A71c01F789FeBC66e8').to.equal(address); break;
            case CoinType.SEMUX: expect('0xfe604170382452f77bc922bc19eb4b53504b09c2').to.equal(address); break;
        }
    };

});
