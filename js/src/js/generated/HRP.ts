// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


export enum HRP {
    UNKNOWN = 0,
    BITCOIN = 1,
    LITECOIN = 2,
    BITCOINCASH = 3,
    BINANCE = 4,
    BINANCETEST = 5,
    COSMOS = 6,
    GROESTLCOIN = 7
}

export class HRPUtil {

    public static toString(hrp: HRP): string {
        switch (hrp) {
            case HRP.UNKNOWN: return '';
            case HRP.BITCOIN: return 'bc';
            case HRP.LITECOIN: return 'ltc';
            case HRP.BITCOINCASH: return 'bitcoincash';
            case HRP.BINANCE: return 'bnb';
            case HRP.BINANCETEST: return 'tbnb';
            case HRP.COSMOS: return 'cosmos';
            case HRP.GROESTLCOIN: return 'grs';
            default: return '';
        }
    }

}
