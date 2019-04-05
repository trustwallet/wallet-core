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

public class WanchainAddress {
    private long nativeHandle;

    private WanchainAddress() {
        nativeHandle = 0;
    }

    static WanchainAddress createFromNative(long nativeHandle) {
        WanchainAddress instance = new WanchainAddress();
        instance.nativeHandle = nativeHandle;
        WanchainAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithKeyHash(byte[] keyHash);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(WanchainAddress lhs, WanchainAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public WanchainAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        WanchainAddressPhantomReference.register(this, nativeHandle);
    }

    public WanchainAddress(byte[] keyHash) {
        nativeHandle = nativeCreateWithKeyHash(keyHash);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        WanchainAddressPhantomReference.register(this, nativeHandle);
    }

    public WanchainAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        WanchainAddressPhantomReference.register(this, nativeHandle);
    }

}

class WanchainAddressPhantomReference extends java.lang.ref.PhantomReference<WanchainAddress> {
    private static java.util.Set<WanchainAddressPhantomReference> references = new HashSet<WanchainAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<WanchainAddress> queue = new java.lang.ref.ReferenceQueue<WanchainAddress>();
    private long nativeHandle;

    private WanchainAddressPhantomReference(WanchainAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(WanchainAddress referent, long nativeHandle) {
        references.add(new WanchainAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        WanchainAddressPhantomReference ref = (WanchainAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (WanchainAddressPhantomReference) queue.poll()) {
            WanchainAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
