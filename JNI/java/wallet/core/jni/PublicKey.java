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

public class PublicKey {
    private long nativeHandle;

    private PublicKey() {
        nativeHandle = 0;
    }

    static PublicKey createFromNative(long nativeHandle) {
        PublicKey instance = new PublicKey();
        instance.nativeHandle = nativeHandle;
        PublicKeyPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithData(byte[] data);
    static native void nativeDelete(long handle);

    public static native boolean isValid(byte[] data);
    public static native PublicKey recover(byte[] signature, byte[] message);
    public native boolean isCompressed();
    public native PublicKey compressed();
    public native PublicKey uncompressed();
    public native byte[] data();
    public native String description();
    public native boolean verify(byte[] signature, byte[] message);

    public PublicKey(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        PublicKeyPhantomReference.register(this, nativeHandle);
    }

}

class PublicKeyPhantomReference extends java.lang.ref.PhantomReference<PublicKey> {
    private static java.util.Set<PublicKeyPhantomReference> references = new HashSet<PublicKeyPhantomReference>();
    private static java.lang.ref.ReferenceQueue<PublicKey> queue = new java.lang.ref.ReferenceQueue<PublicKey>();
    private long nativeHandle;

    private PublicKeyPhantomReference(PublicKey referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(PublicKey referent, long nativeHandle) {
        references.add(new PublicKeyPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        PublicKeyPhantomReference ref = (PublicKeyPhantomReference) queue.poll();
        for (; ref != null; ref = (PublicKeyPhantomReference) queue.poll()) {
            PublicKey.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
