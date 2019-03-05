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

public class PrivateKeyEd25519 {
    private long nativeHandle;

    static PrivateKeyEd25519 createFromNative(long nativeHandle) {
        PrivateKeyEd25519 instance = new PrivateKeyEd25519();
        instance.nativeHandle = nativeHandle;
        PrivateKeyEd25519PhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate();
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateCopy(PrivateKeyEd25519 key);
    static native void nativeDelete(long handle);

    public static native boolean isValid(byte[] data);
    public native byte[] data();
    public native PublicKeyEd25519 getPublicKey();
    public native byte[] sign(byte[] digest);

    public PrivateKeyEd25519() {
        nativeHandle = nativeCreate();
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeyEd25519PhantomReference.register(this, nativeHandle);
    }

    public PrivateKeyEd25519(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeyEd25519PhantomReference.register(this, nativeHandle);
    }

    public PrivateKeyEd25519(PrivateKeyEd25519 key) {
        nativeHandle = nativeCreateCopy(key);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeyEd25519PhantomReference.register(this, nativeHandle);
    }

}

class PrivateKeyEd25519PhantomReference extends java.lang.ref.PhantomReference<PrivateKeyEd25519> {
    private static java.util.Set<PrivateKeyEd25519PhantomReference> references = new HashSet<PrivateKeyEd25519PhantomReference>();
    private static java.lang.ref.ReferenceQueue<PrivateKeyEd25519> queue = new java.lang.ref.ReferenceQueue<PrivateKeyEd25519>();
    private long nativeHandle;

    private PrivateKeyEd25519PhantomReference(PrivateKeyEd25519 referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(PrivateKeyEd25519 referent, long nativeHandle) {
        references.add(new PrivateKeyEd25519PhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        PrivateKeyEd25519PhantomReference ref = (PrivateKeyEd25519PhantomReference) queue.poll();
        for (; ref != null; ref = (PrivateKeyEd25519PhantomReference) queue.poll()) {
            PrivateKeyEd25519.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
