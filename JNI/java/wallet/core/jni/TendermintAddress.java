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
import java.util.HashSet;

public class TendermintAddress {
    private long nativeHandle;

    private TendermintAddress() {
        nativeHandle = 0;
    }

    static TendermintAddress createFromNative(long nativeHandle) {
        TendermintAddress instance = new TendermintAddress();
        instance.nativeHandle = nativeHandle;
        TendermintAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithKeyHash(HRP hrp, byte[] keyHash);
    static native long nativeCreateWithPublicKey(HRP hrp, PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(TendermintAddress lhs, TendermintAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native HRP hrp();
    public native byte[] keyHash();

    public TendermintAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TendermintAddressPhantomReference.register(this, nativeHandle);
    }

    public TendermintAddress(HRP hrp, byte[] keyHash) {
        nativeHandle = nativeCreateWithKeyHash(hrp, keyHash);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TendermintAddressPhantomReference.register(this, nativeHandle);
    }

    public TendermintAddress(HRP hrp, PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(hrp, publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        TendermintAddressPhantomReference.register(this, nativeHandle);
    }

}

class TendermintAddressPhantomReference extends java.lang.ref.PhantomReference<TendermintAddress> {
    private static java.util.Set<TendermintAddressPhantomReference> references = new HashSet<TendermintAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<TendermintAddress> queue = new java.lang.ref.ReferenceQueue<TendermintAddress>();
    private long nativeHandle;

    private TendermintAddressPhantomReference(TendermintAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(TendermintAddress referent, long nativeHandle) {
        references.add(new TendermintAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        TendermintAddressPhantomReference ref = (TendermintAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (TendermintAddressPhantomReference) queue.poll()) {
            TendermintAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
