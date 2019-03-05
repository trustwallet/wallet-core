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

public class BitcoinCashAddress {
    private byte[] bytes;

    private BitcoinCashAddress() {
    }

    static BitcoinCashAddress createFromNative(byte[] bytes) {
        BitcoinCashAddress instance = new BitcoinCashAddress();
        instance.bytes = bytes;
        return instance;
    }

    static native byte[] initWithString(String string);
    static native byte[] initWithData(byte[] data);
    static native byte[] initWithPublicKey(PublicKeySecp256k1 publicKey);

    public static native boolean equals(BitcoinCashAddress lhs, BitcoinCashAddress rhs);
    public static native boolean isValid(byte[] data);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] data();
    public native BitcoinAddress legacyAddress();

    public BitcoinCashAddress(String string) {
        bytes = initWithString(string);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

    public BitcoinCashAddress(byte[] data) {
        bytes = initWithData(data);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

    public BitcoinCashAddress(PublicKeySecp256k1 publicKey) {
        bytes = initWithPublicKey(publicKey);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

}
