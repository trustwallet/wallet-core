// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum CoinType {
    BINANCE (714),
    BITCOIN (0),
    BITCOINCASH (145),
    CALLISTO (820),
    DASH (5),
    ETHEREUM (60),
    ETHEREUMCLASSIC (61),
    GO (6060),
    ICON (74),
    LITECOIN (2),
    POA (178),
    RIPPLE (144),
    TEZOS (1729),
    THUNDERTOKEN (1001),
    TOMOCHAIN (889),
    TRON (195),
    VECHAIN (818),
    WANCHAIN (5718350),
    XDAI (700),
    ZCASH (133),
    ZCOIN (136),
    NIMIQ (242),
    STELLAR (148),
    AION (425);

    private final int value;
    CoinType(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static CoinType createFromValue(int value) {
        switch (value) {
            case 714: return CoinType.BINANCE;
            case 0: return CoinType.BITCOIN;
            case 145: return CoinType.BITCOINCASH;
            case 820: return CoinType.CALLISTO;
            case 5: return CoinType.DASH;
            case 60: return CoinType.ETHEREUM;
            case 61: return CoinType.ETHEREUMCLASSIC;
            case 6060: return CoinType.GO;
            case 74: return CoinType.ICON;
            case 2: return CoinType.LITECOIN;
            case 178: return CoinType.POA;
            case 144: return CoinType.RIPPLE;
            case 1729: return CoinType.TEZOS;
            case 1001: return CoinType.THUNDERTOKEN;
            case 889: return CoinType.TOMOCHAIN;
            case 195: return CoinType.TRON;
            case 818: return CoinType.VECHAIN;
            case 5718350: return CoinType.WANCHAIN;
            case 700: return CoinType.XDAI;
            case 133: return CoinType.ZCASH;
            case 136: return CoinType.ZCOIN;
            case 242: return CoinType.NIMIQ;
            case 148: return CoinType.STELLAR;
            default: return null;
        }
    }

    public native Purpose purpose();
    public native Curve curve();
    public native boolean validate(String address);
    public native String derivationPath();
    public native String deriveAddress(PrivateKey privateKey);
}
