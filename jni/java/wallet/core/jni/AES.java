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

public class AES {
    private byte[] bytes;

    private AES() {
    }

    static AES createFromNative(byte[] bytes) {
        AES instance = new AES();
        instance.bytes = bytes;
        return instance;
    }


    public static native byte[] cbcencrypt(byte[] key, byte[] data, byte[] iv);
    public static native byte[] cbcdecrypt(byte[] key, byte[] data, byte[] iv);
    public static native byte[] ctrencrypt(byte[] key, byte[] data, byte[] iv);
    public static native byte[] ctrdecrypt(byte[] key, byte[] data, byte[] iv);

}
