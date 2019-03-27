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

public class TronAddress {
    private long nativeHandle;

    private TronAddress() {
        nativeHandle = 0;
    }

    static TronAddress createFromNative(long nativeHandle) {
        TronAddress instance = new TronAddress();
        instance.nativeHandle = nativeHandle;
        TronAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(TronAddress lhs, TronAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();

    public TronAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TronAddressPhantomReference.register(this, nativeHandle);
    }

    public TronAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TronAddressPhantomReference.register(this, nativeHandle);
    }

}

class TronAddressPhantomReference extends java.lang.ref.PhantomReference<TronAddress> {
    private static java.util.Set<TronAddressPhantomReference> references = new HashSet<TronAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<TronAddress> queue = new java.lang.ref.ReferenceQueue<TronAddress>();
    private long nativeHandle;

    private TronAddressPhantomReference(TronAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(TronAddress referent, long nativeHandle) {
        references.add(new TronAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        TronAddressPhantomReference ref = (TronAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (TronAddressPhantomReference) queue.poll()) {
            TronAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
