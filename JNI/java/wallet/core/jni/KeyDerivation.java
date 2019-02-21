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

public class KeyDerivation {
    private byte[] bytes;

    private KeyDerivation() {
    }

    static KeyDerivation createFromNative(byte[] bytes) {
        KeyDerivation instance = new KeyDerivation();
        instance.bytes = bytes;
        return instance;
    }


    public static native byte[] scrypt(String password, byte[] salt, long n, int r, int p, int keyLength);
    public static native byte[] pbkdf2256(String password, byte[] salt, int iterations, int keyLength);
    public static native byte[] pbkdf2512(String password, byte[] salt, int iterations, int keyLength);

}
