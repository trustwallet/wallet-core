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

public class StellarAddress {
    private long nativeHandle;

    private StellarAddress() {
        nativeHandle = 0;
    }

    static StellarAddress createFromNative(long nativeHandle) {
        StellarAddress instance = new StellarAddress();
        instance.nativeHandle = nativeHandle;
        StellarAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(StellarAddress lhs, StellarAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public StellarAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        StellarAddressPhantomReference.register(this, nativeHandle);
    }

    public StellarAddress(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        StellarAddressPhantomReference.register(this, nativeHandle);
    }

    public StellarAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        StellarAddressPhantomReference.register(this, nativeHandle);
    }

}

class StellarAddressPhantomReference extends java.lang.ref.PhantomReference<StellarAddress> {
    private static java.util.Set<StellarAddressPhantomReference> references = new HashSet<StellarAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<StellarAddress> queue = new java.lang.ref.ReferenceQueue<StellarAddress>();
    private long nativeHandle;

    private StellarAddressPhantomReference(StellarAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(StellarAddress referent, long nativeHandle) {
        references.add(new StellarAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        StellarAddressPhantomReference ref = (StellarAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (StellarAddressPhantomReference) queue.poll()) {
            StellarAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
