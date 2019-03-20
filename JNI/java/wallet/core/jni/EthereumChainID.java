// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum EthereumChainID {
    ETHEREUM (1),
    GO (60),
    POA (99),
    CALLISTO (820),
    ETHEREUMCLASSIC (61),
    VECHAIN (74),
    THUNDERTOKEN (18),
    TOMOCHAIN (88),
    XDAI (100);

    private final int value;
    EthereumChainID(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static EthereumChainID createFromValue(int value) {
        switch (value) {
            case 1: return EthereumChainID.ETHEREUM;
            case 60: return EthereumChainID.GO;
            case 99: return EthereumChainID.POA;
            case 820: return EthereumChainID.CALLISTO;
            case 61: return EthereumChainID.ETHEREUMCLASSIC;
            case 74: return EthereumChainID.VECHAIN;
            case 18: return EthereumChainID.THUNDERTOKEN;
            case 88: return EthereumChainID.TOMOCHAIN;
            case 100: return EthereumChainID.XDAI;
            default: return null;
        }
    }

}
