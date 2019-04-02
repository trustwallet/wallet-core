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

public class NimiqAddress {
    private long nativeHandle;

    private NimiqAddress() {
        nativeHandle = 0;
    }

    static NimiqAddress createFromNative(long nativeHandle) {
        NimiqAddress instance = new NimiqAddress();
        instance.nativeHandle = nativeHandle;
        NimiqAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(NimiqAddress lhs, NimiqAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public NimiqAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        NimiqAddressPhantomReference.register(this, nativeHandle);
    }

    public NimiqAddress(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        NimiqAddressPhantomReference.register(this, nativeHandle);
    }

    public NimiqAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        NimiqAddressPhantomReference.register(this, nativeHandle);
    }

}

class NimiqAddressPhantomReference extends java.lang.ref.PhantomReference<NimiqAddress> {
    private static java.util.Set<NimiqAddressPhantomReference> references = new HashSet<NimiqAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<NimiqAddress> queue = new java.lang.ref.ReferenceQueue<NimiqAddress>();
    private long nativeHandle;

    private NimiqAddressPhantomReference(NimiqAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(NimiqAddress referent, long nativeHandle) {
        references.add(new NimiqAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        NimiqAddressPhantomReference ref = (NimiqAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (NimiqAddressPhantomReference) queue.poll()) {
            NimiqAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
