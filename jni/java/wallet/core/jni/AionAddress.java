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

public class AionAddress {
    private long nativeHandle;

    private AionAddress() {
        nativeHandle = 0;
    }

    static AionAddress createFromNative(long nativeHandle) {
        AionAddress instance = new AionAddress();
        instance.nativeHandle = nativeHandle;
        AionAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(AionAddress lhs, AionAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();

    public AionAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        AionAddressPhantomReference.register(this, nativeHandle);
    }

    public AionAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        AionAddressPhantomReference.register(this, nativeHandle);
    }

}

class AionAddressPhantomReference extends java.lang.ref.PhantomReference<AionAddress> {
    private static java.util.Set<AionAddressPhantomReference> references = new HashSet<AionAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<AionAddress> queue = new java.lang.ref.ReferenceQueue<AionAddress>();
    private long nativeHandle;

    private AionAddressPhantomReference(AionAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(AionAddress referent, long nativeHandle) {
        references.add(new AionAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        AionAddressPhantomReference ref = (AionAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (AionAddressPhantomReference) queue.poll()) {
            AionAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
