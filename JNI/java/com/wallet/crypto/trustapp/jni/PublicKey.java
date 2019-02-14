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

public class PublicKey {
    private byte[] bytes;

    private PublicKey() {
    }

    static PublicKey createFromNative(byte[] bytes) {
        PublicKey instance = new PublicKey();
        instance.bytes = bytes;
        return instance;
    }

    static native byte[] initWithData(byte[] data);

    public static native boolean isValid(byte[] data);
    public static native PublicKey recover(byte[] signature, byte[] message);
    public native boolean isCompressed();
    public native PublicKey compressed();
    public native byte[] data();
    public native String description();
    public native boolean verify(byte[] signature, byte[] message);

    public PublicKey(byte[] data) {
        bytes = initWithData(data);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

}
