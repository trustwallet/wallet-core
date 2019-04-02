// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.wallet.crypto.trustapp.jni;


public enum ChainID {
    ETHEREUM (1),
    GO (60),
    POA (99),
    CALLISTO (820),
    ETHEREUMCLASSIC (61),
    VECHAIN (74),
    THUNDERTOKEN (18),
    TOMOCHAIN (88);

    private final int value;
    ChainID(int value) {
        this.value = value;
    }
    public int value() { return value; }
}
