// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package wallet.core.java;

import com.google.protobuf.MessageLite;
import com.google.protobuf.Parser;

import wallet.core.jni.CoinType;

public class AnySigner {
    public static <T extends MessageLite> T sign(MessageLite input, CoinType coin, Parser<T> parser) throws Exception {
        byte[] data = input.toByteArray();
        byte[] outputData = nativeSign(data, coin.value());
        T output = parser.parseFrom(outputData);
        outputData = null;
        return output;
    }
    public static native byte[] nativeSign(byte[] data, int coin);

    public static native String signJSON(String json, byte[] key, int coin);

    public static native boolean supportsJSON(int coin);

    public static <T extends MessageLite> T plan(MessageLite input, CoinType coin, Parser<T> parser) throws Exception {
        byte[] data = input.toByteArray();
        byte[] outputData = nativePlan(data, coin.value());
        T output = parser.parseFrom(outputData);
        outputData = null;
        return output;
    }
    public static native byte[] nativePlan(byte[] data, int coin);
}
