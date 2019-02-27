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

public class RippleAddress {
    private long nativeHandle;

    private RippleAddress() {
        nativeHandle = 0;
    }

    static RippleAddress createFromNative(long nativeHandle) {
        RippleAddress instance = new RippleAddress();
        instance.nativeHandle = nativeHandle;
        RippleAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(RippleAddress lhs, RippleAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public RippleAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        RippleAddressPhantomReference.register(this, nativeHandle);
    }

    public RippleAddress(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        RippleAddressPhantomReference.register(this, nativeHandle);
    }

    public RippleAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        RippleAddressPhantomReference.register(this, nativeHandle);
    }

}

class RippleAddressPhantomReference extends java.lang.ref.PhantomReference<RippleAddress> {
    private static java.util.Set<RippleAddressPhantomReference> references = new HashSet<RippleAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<RippleAddress> queue = new java.lang.ref.ReferenceQueue<RippleAddress>();
    private long nativeHandle;

    private RippleAddressPhantomReference(RippleAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(RippleAddress referent, long nativeHandle) {
        references.add(new RippleAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        RippleAddressPhantomReference ref = (RippleAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (RippleAddressPhantomReference) queue.poll()) {
            RippleAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
