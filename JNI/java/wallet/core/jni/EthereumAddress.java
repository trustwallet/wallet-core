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

public class EthereumAddress {
    private long nativeHandle;

    private EthereumAddress() {
        nativeHandle = 0;
    }

    static EthereumAddress createFromNative(long nativeHandle) {
        EthereumAddress instance = new EthereumAddress();
        instance.nativeHandle = nativeHandle;
        EthereumAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithKeyHash(byte[] keyHash);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(EthereumAddress lhs, EthereumAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public EthereumAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        EthereumAddressPhantomReference.register(this, nativeHandle);
    }

    public EthereumAddress(byte[] keyHash) {
        nativeHandle = nativeCreateWithKeyHash(keyHash);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        EthereumAddressPhantomReference.register(this, nativeHandle);
    }

    public EthereumAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        EthereumAddressPhantomReference.register(this, nativeHandle);
    }

}

class EthereumAddressPhantomReference extends java.lang.ref.PhantomReference<EthereumAddress> {
    private static java.util.Set<EthereumAddressPhantomReference> references = new HashSet<EthereumAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<EthereumAddress> queue = new java.lang.ref.ReferenceQueue<EthereumAddress>();
    private long nativeHandle;

    private EthereumAddressPhantomReference(EthereumAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(EthereumAddress referent, long nativeHandle) {
        references.add(new EthereumAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        EthereumAddressPhantomReference ref = (EthereumAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (EthereumAddressPhantomReference) queue.poll()) {
            EthereumAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
