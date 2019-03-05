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

public class TezosAddress {
    private byte[] bytes;

    private TezosAddress() {
    }

    static TezosAddress createFromNative(byte[] bytes) {
        TezosAddress instance = new TezosAddress();
        instance.bytes = bytes;
        return instance;
    }

    static native byte[] initWithString(String string);
    static native byte[] initWithPublicKey(PublicKey publicKey);

    public static native String description(TezosAddress address);
    public static native boolean isValid(byte[] data);
    public static native boolean isValidString(String string);
    public static native byte[] data(TezosAddress address);

    public TezosAddress(String string) {
        bytes = initWithString(string);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

    public TezosAddress(PublicKey publicKey) {
        bytes = initWithPublicKey(publicKey);
        if (bytes == null) {
            throw new InvalidParameterException();
        }
    }

}
