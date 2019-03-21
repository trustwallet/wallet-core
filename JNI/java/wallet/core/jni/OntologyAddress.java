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

public class OntologyAddress {
    private long nativeHandle;

    private OntologyAddress() {
        nativeHandle = 0;
    }

    static OntologyAddress createFromNative(long nativeHandle) {
        OntologyAddress instance = new OntologyAddress();
        instance.nativeHandle = nativeHandle;
        OntologyAddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithPublicKey(PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(OntologyAddress lhs, OntologyAddress rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native byte[] keyHash();

    public OntologyAddress(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        OntologyAddressPhantomReference.register(this, nativeHandle);
    }

    public OntologyAddress(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        OntologyAddressPhantomReference.register(this, nativeHandle);
    }

    public OntologyAddress(PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        OntologyAddressPhantomReference.register(this, nativeHandle);
    }

}

class OntologyAddressPhantomReference extends java.lang.ref.PhantomReference<OntologyAddress> {
    private static java.util.Set<OntologyAddressPhantomReference> references = new HashSet<OntologyAddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<OntologyAddress> queue = new java.lang.ref.ReferenceQueue<OntologyAddress>();
    private long nativeHandle;

    private OntologyAddressPhantomReference(OntologyAddress referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(OntologyAddress referent, long nativeHandle) {
        references.add(new OntologyAddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        OntologyAddressPhantomReference ref = (OntologyAddressPhantomReference) queue.poll();
        for (; ref != null; ref = (OntologyAddressPhantomReference) queue.poll()) {
            OntologyAddress.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
