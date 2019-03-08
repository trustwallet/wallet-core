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

public class TezosAddress {
    private long nativeHandle;

    private TezosAddress() {
        nativeHandle = 0;
    }

    static TezosAddress createFromNative(long nativeHandle) {
        TezosAddress instance = new TezosAddress();
        instance.nativeHandle = nativeHandle;
        TezosAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(TezosAddress lhs, TezosAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public TezosAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TezosAddressPhantomReference.register(this, nativeHandle);
    }

    public TezosAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TezosAddressPhantomReference.register(this, nativeHandle);
    }

}

class TezosAddressPhantomReference extends java.lang.ref.PhantomReference<TezosAddress> {
    private static java.util.Set<TezosAddressPhantomReference> references = new HashSet<TezosAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<TezosAddress> queue = new java.lang.ref.ReferenceQueue<TezosAddress>();
    private long nativeHandle;

    private TezosAddressPhantomReference(TezosAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(TezosAddress referent, long nativeHandle) {
        references.add(new TezosAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        TezosAddressPhantomReference ref = (TezosAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (TezosAddressPhantomReference) queue.poll()) {
            TezosAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
