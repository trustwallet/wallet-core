// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum HRP {
    UNKNOWN (0),
    BITCOIN (1),
    LITECOIN (2),
    BITCOINCASH (3),
    BINANCE (4),
    BINANCETEST (5);

    private final int value;
    HRP(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static HRP createFromValue(int value) {
        switch (value) {
            case 0: return HRP.UNKNOWN;
            case 1: return HRP.BITCOIN;
            case 2: return HRP.LITECOIN;
            case 3: return HRP.BITCOINCASH;
            case 4: return HRP.BINANCE;
            case 5: return HRP.BINANCETEST;
            default: return null;
        }
    }


    public String toString() {
        switch (this) {
        case UNKNOWN: return "";
        case BITCOIN: return "bc";
        case LITECOIN: return "ltc";
        case BITCOINCASH: return "bitcoincash";
        case BINANCE: return "bnb";
        case BINANCETEST: return "tbnb";
        default: return "";
        }
    }
}
