// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;

import java.security.InvalidParameterException;

public class Hash {
    private byte[] bytes;

    private Hash() {
    }

    static Hash createFromNative(byte[] bytes) {
        Hash instance = new Hash();
        instance.bytes = bytes;
        return instance;
    }


    public static native byte[] sha1(byte[] data);
    public static native byte[] sha256(byte[] data);
    public static native byte[] sha512(byte[] data);
    public static native byte[] keccak256(byte[] data);
    public static native byte[] keccak512(byte[] data);
    public static native byte[] sha3256(byte[] data);
    public static native byte[] sha3512(byte[] data);
    public static native byte[] ripemd(byte[] data);
    public static native byte[] blake2b(byte[] data, int size);
    public static native byte[] sha256RIPEMD(byte[] data);
    public static native byte[] sha256SHA256(byte[] data);

}
