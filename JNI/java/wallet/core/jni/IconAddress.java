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

public class IconAddress {
    private long nativeHandle;

    private IconAddress() {
        nativeHandle = 0;
    }

    static IconAddress createFromNative(long nativeHandle) {
        IconAddress instance = new IconAddress();
        instance.nativeHandle = nativeHandle;
        IconAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithKeyHash(byte[] keyHash, IconAddressType type);
    static native long nativeCreateWithPublicKey(PublicKey publicKey, IconAddressType type);
    static native void nativeDelete(long handle);

    public static native boolean equals(IconAddress lhs, IconAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public IconAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        IconAddressPhantomReference.register(this, nativeHandle);
    }

    public IconAddress(byte[] keyHash, IconAddressType type) {
        nativeHandle = nativeCreateWithKeyHash(keyHash, type);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        IconAddressPhantomReference.register(this, nativeHandle);
    }

    public IconAddress(PublicKey publicKey, IconAddressType type) {
        nativeHandle = nativeCreateWithPublicKey(publicKey, type);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        IconAddressPhantomReference.register(this, nativeHandle);
    }

}

class IconAddressPhantomReference extends java.lang.ref.PhantomReference<IconAddress> {
    private static java.util.Set<IconAddressPhantomReference> references = new HashSet<IconAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<IconAddress> queue = new java.lang.ref.ReferenceQueue<IconAddress>();
    private long nativeHandle;

    private IconAddressPhantomReference(IconAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(IconAddress referent, long nativeHandle) {
        references.add(new IconAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        IconAddressPhantomReference ref = (IconAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (IconAddressPhantomReference) queue.poll()) {
            IconAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
