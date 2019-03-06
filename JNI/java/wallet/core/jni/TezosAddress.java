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
import java.util.HashSet;

public class TezosAddress {
    private long nativeHandle;

    private TezosAddress() {
        nativeHandle = 0;
    }

    static TezosAddress createFromNative(long nativeHandle) {
        TezosAddress instance = new TezosAddress();
        instance.nativeHandle = nativeHandle;
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);

    public static native boolean isValid(byte[] data);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public TezosAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TezosAddressPhantomReference.register(this, nativeHandle);
    }

    public TezosAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TezosAddressPhantomReference.register(this, nativeHandle);
    }

}

