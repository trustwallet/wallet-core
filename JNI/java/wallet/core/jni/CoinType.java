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
    EOS (194),
    ETHEREUM (60),
    ETHEREUMCLASSIC (61),
    GO (6060),
    ICON (74),
    LITECOIN (2),
    POA (178),
    THUNDERTOKEN (1001),
    TOMOCHAIN (889),
    TRON (195),
    VECHAIN (818),
    WANCHAIN (5718350),
    ZCOIN (136),
    ZCASH (133),
    RIPPLE (144);

    private final int value;
    CoinType(int value) {
        this.value = value;
    }
    public int value() { return value; }
}
