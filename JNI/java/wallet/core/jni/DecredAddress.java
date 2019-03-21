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

public class DecredAddress {
    private long nativeHandle;

    private DecredAddress() {
        nativeHandle = 0;
    }

    static DecredAddress createFromNative(long nativeHandle) {
        DecredAddress instance = new DecredAddress();
        instance.nativeHandle = nativeHandle;
        DecredAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(DecredAddress lhs, DecredAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();

    public DecredAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        DecredAddressPhantomReference.register(this, nativeHandle);
    }

    public DecredAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        DecredAddressPhantomReference.register(this, nativeHandle);
    }

}

class DecredAddressPhantomReference extends java.lang.ref.PhantomReference<DecredAddress> {
    private static java.util.Set<DecredAddressPhantomReference> references = new HashSet<DecredAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<DecredAddress> queue = new java.lang.ref.ReferenceQueue<DecredAddress>();
    private long nativeHandle;

    private DecredAddressPhantomReference(DecredAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(DecredAddress referent, long nativeHandle) {
        references.add(new DecredAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        DecredAddressPhantomReference ref = (DecredAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (DecredAddressPhantomReference) queue.poll()) {
            DecredAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
