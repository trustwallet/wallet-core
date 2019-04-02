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

public class PrivateKey {
    private long nativeHandle;

    static PrivateKey createFromNative(long nativeHandle) {
        PrivateKey instance = new PrivateKey();
        instance.nativeHandle = nativeHandle;
        PrivateKeyPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate();
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateCopy(PrivateKey key);
    static native void nativeDelete(long handle);

    public static native boolean isValid(byte[] data);
    public native byte[] data();
    public native PublicKey getPublicKeySecp256k1(boolean compressed);
    public native PublicKey getPublicKeyNist256p1();
    public native PublicKey getPublicKeyEd25519();
    public native byte[] sign(byte[] digest, Curve curve);
    public native byte[] signAsDER(byte[] digest, Curve curve);

    public PrivateKey() {
        nativeHandle = nativeCreate();
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeyPhantomReference.register(this, nativeHandle);
    }

    public PrivateKey(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeyPhantomReference.register(this, nativeHandle);
    }

    public PrivateKey(PrivateKey key) {
        nativeHandle = nativeCreateCopy(key);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeyPhantomReference.register(this, nativeHandle);
    }

}

class PrivateKeyPhantomReference extends java.lang.ref.PhantomReference<PrivateKey> {
    private static java.util.Set<PrivateKeyPhantomReference> references = new HashSet<PrivateKeyPhantomReference>();
    private static java.lang.ref.ReferenceQueue<PrivateKey> queue = new java.lang.ref.ReferenceQueue<PrivateKey>();
    private long nativeHandle;

    private PrivateKeyPhantomReference(PrivateKey referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(PrivateKey referent, long nativeHandle) {
        references.add(new PrivateKeyPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        PrivateKeyPhantomReference ref = (PrivateKeyPhantomReference) queue.poll();
        for (; ref != null; ref = (PrivateKeyPhantomReference) queue.poll()) {
            PrivateKey.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
