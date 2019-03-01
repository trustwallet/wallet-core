// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;

import java.security.InvalidParameterException;

public class UInt256 implements Comparable<UInt256> {
    private byte[] bytes;

    private UInt256() {
    }

    static UInt256 createFromNative(byte[] bytes) {
        UInt256 instance = new UInt256();
        instance.bytes = bytes;
        return instance;
    }

    static native byte[] initWithData(byte[] data);
    static native byte[] initWithString(String string);
    static native byte[] initWithUInt32(int value);
    static native byte[] initWithUInt64(long value);

    public static native UInt256 zero();
    public static native UInt256 one();
    public static native boolean equals(UInt256 lhs, UInt256 rhs);
    public static native boolean less(UInt256 lhs, UInt256 rhs);
    public native boolean isZero();
    public native int uint32Value();
    public native long uint64Value();
    public native byte[] data();
    public native String description();
    public native String format(int decimals);
    public native int compareTo(UInt256 other);

    public UInt256(byte[] data) {
        bytes = initWithData(data);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

    public UInt256(String string) {
        bytes = initWithString(string);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

    public UInt256(int value) {
        bytes = initWithUInt32(value);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

    public UInt256(long value) {
        bytes = initWithUInt64(value);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

}
