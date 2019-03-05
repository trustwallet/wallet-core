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

public class PrivateKeySecp256k1 {
    private long nativeHandle;

    static PrivateKeySecp256k1 createFromNative(long nativeHandle) {
        PrivateKeySecp256k1 instance = new PrivateKeySecp256k1();
        instance.nativeHandle = nativeHandle;
        PrivateKeySecp256k1PhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate();
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateCopy(PrivateKeySecp256k1 key);
    static native void nativeDelete(long handle);

    public static native boolean isValid(byte[] data);
    public native byte[] data();
    public native PublicKeySecp256k1 getPublicKey(boolean compressed);
    public native byte[] sign(byte[] digest);
    public native byte[] signAsDER(byte[] digest);

    public PrivateKeySecp256k1() {
        nativeHandle = nativeCreate();
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeySecp256k1PhantomReference.register(this, nativeHandle);
    }

    public PrivateKeySecp256k1(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeySecp256k1PhantomReference.register(this, nativeHandle);
    }

    public PrivateKeySecp256k1(PrivateKeySecp256k1 key) {
        nativeHandle = nativeCreateCopy(key);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PrivateKeySecp256k1PhantomReference.register(this, nativeHandle);
    }

}

class PrivateKeySecp256k1PhantomReference extends java.lang.ref.PhantomReference<PrivateKeySecp256k1> {
    private static java.util.Set<PrivateKeySecp256k1PhantomReference> references = new HashSet<PrivateKeySecp256k1PhantomReference>();
    private static java.lang.ref.ReferenceQueue<PrivateKeySecp256k1> queue = new java.lang.ref.ReferenceQueue<PrivateKeySecp256k1>();
    private long nativeHandle;

    private PrivateKeySecp256k1PhantomReference(PrivateKeySecp256k1 referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(PrivateKeySecp256k1 referent, long nativeHandle) {
        references.add(new PrivateKeySecp256k1PhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        PrivateKeySecp256k1PhantomReference ref = (PrivateKeySecp256k1PhantomReference) queue.poll();
        for (; ref != null; ref = (PrivateKeySecp256k1PhantomReference) queue.poll()) {
            PrivateKeySecp256k1.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
