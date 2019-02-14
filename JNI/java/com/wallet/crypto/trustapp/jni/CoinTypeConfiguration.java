// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package com.wallet.crypto.trustapp.jni;

import java.security.InvalidParameterException;

public class CoinTypeConfiguration {
    private byte[] bytes;

    private CoinTypeConfiguration() {
    }

    static CoinTypeConfiguration createFromNative(byte[] bytes) {
        CoinTypeConfiguration instance = new CoinTypeConfiguration();
        instance.bytes = bytes;
        return instance;
    }


    public static native String getSymbol(CoinType type);
    public static native int getDecimals(CoinType type);
    public static native String getTransactionURL(CoinType type, String transactionID);
    public static native String getID(CoinType type);
    public static native String getName(CoinType type);

}
