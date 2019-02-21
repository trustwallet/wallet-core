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

public class Base58 {
    private byte[] bytes;

    private Base58() {
    }

    static Base58 createFromNative(byte[] bytes) {
        Base58 instance = new Base58();
        instance.bytes = bytes;
        return instance;
    }


    public static native String encode(byte[] data);
    public static native String encodeNoCheck(byte[] data);
    public static native byte[] decode(String string);
    public static native byte[] decodeNoCheck(String string);

}
