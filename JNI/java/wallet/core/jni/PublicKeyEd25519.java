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

public class PublicKeyEd25519 {
    private byte[] bytes;

    private PublicKeyEd25519() {
    }

    static PublicKeyEd25519 createFromNative(byte[] bytes) {
        PublicKeyEd25519 instance = new PublicKeyEd25519();
        instance.bytes = bytes;
        return instance;
    }

    static native byte[] initWithData(byte[] data);

    public static native boolean isValid(byte[] data);
    public native byte[] data();
    public native String description();
    public native boolean verify(byte[] signature, byte[] message);

    public PublicKeyEd25519(byte[] data) {
        bytes = initWithData(data);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

}
