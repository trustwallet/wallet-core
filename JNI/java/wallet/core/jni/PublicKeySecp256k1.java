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

public class PublicKeySecp256k1 {
    private byte[] bytes;

    private PublicKeySecp256k1() {
    }

    static PublicKeySecp256k1 createFromNative(byte[] bytes) {
        PublicKeySecp256k1 instance = new PublicKeySecp256k1();
        instance.bytes = bytes;
        return instance;
    }

    static native byte[] initWithData(byte[] data);

    public static native boolean isValid(byte[] data);
    public static native PublicKeySecp256k1 recover(byte[] signature, byte[] message);
    public native boolean isCompressed();
    public native PublicKeySecp256k1 compressed();
    public native byte[] data();
    public native String description();
    public native boolean verify(byte[] signature, byte[] message);

    public PublicKeySecp256k1(byte[] data) {
        bytes = initWithData(data);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

}
