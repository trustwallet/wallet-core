// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


import { WalletCore } from '../WalletCore';

export enum HRP {
    UNKNOWN = 0,
    BINANCE = 1,
    BITCOIN = 2,
    BITCOINCASH = 3,
    COINEX = 4,
    COSMOS = 5,
    DIGIBYTE = 6,
    GROESTLCOIN = 7,
    LITECOIN = 8,
    MONACOIN = 9,
    QTUM = 10,
    SIMPLELEDGER = 11,
    TERRA = 12,
    VIACOIN = 13,
    ZILLIQA = 14
}

export class HRPUtil {

    public static toString(hrp: HRP): string {
        switch (hrp) {
            case HRP.UNKNOWN: return '';
            case HRP.BINANCE: return 'bnb';
            case HRP.BITCOIN: return 'bc';
            case HRP.BITCOINCASH: return 'bitcoincash';
            case HRP.COINEX: return 'cettest';
            case HRP.COSMOS: return 'cosmos';
            case HRP.DIGIBYTE: return 'dgb';
            case HRP.GROESTLCOIN: return 'grs';
            case HRP.LITECOIN: return 'ltc';
            case HRP.MONACOIN: return 'mona';
            case HRP.QTUM: return 'qc';
            case HRP.SIMPLELEDGER: return 'bitcoincash';
            case HRP.TERRA: return 'terra';
            case HRP.VIACOIN: return 'via';
            case HRP.ZILLIQA: return 'zil';
            default: return '';
        }
    }

}
