// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package com.wallet.crypto.trustapp.jni;


public enum EthereumChainID {
    ETHEREUM (1),
    GO (60),
    POA (99),
    CALLISTO (820),
    ETHEREUMCLASSIC (61),
    VECHAIN (74),
    THUNDERTOKEN (18),
    TOMOCHAIN (88);

    private final int value;
    EthereumChainID(int value) {
        this.value = value;
    }
    public int value() { return value; }
}
