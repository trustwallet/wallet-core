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

public class NEOAddress {
    private long nativeHandle;

    private NEOAddress() {
        nativeHandle = 0;
    }

    static NEOAddress createFromNative(long nativeHandle) {
        NEOAddress instance = new NEOAddress();
        instance.nativeHandle = nativeHandle;
        NEOAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(NEOAddress lhs, NEOAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public NEOAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        NEOAddressPhantomReference.register(this, nativeHandle);
    }

    public NEOAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        NEOAddressPhantomReference.register(this, nativeHandle);
    }

}

class NEOAddressPhantomReference extends java.lang.ref.PhantomReference<NEOAddress> {
    private static java.util.Set<NEOAddressPhantomReference> references = new HashSet<NEOAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<NEOAddress> queue = new java.lang.ref.ReferenceQueue<NEOAddress>();
    private long nativeHandle;

    private NEOAddressPhantomReference(NEOAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(NEOAddress referent, long nativeHandle) {
        references.add(new NEOAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        NEOAddressPhantomReference ref = (NEOAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (NEOAddressPhantomReference) queue.poll()) {
            NEOAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
