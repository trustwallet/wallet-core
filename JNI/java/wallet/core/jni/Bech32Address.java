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

public class Bech32Address {
    private long nativeHandle;

    private Bech32Address() {
        nativeHandle = 0;
    }

    static Bech32Address createFromNative(long nativeHandle) {
        Bech32Address instance = new Bech32Address();
        instance.nativeHandle = nativeHandle;
        Bech32AddressPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreateWithString(String string);
    static native long nativeCreateWithData(HRP hrp, byte[] data);
    static native long nativeCreateWithPublicKey(HRP hrp, PublicKey publicKey);
    static native void nativeDelete(long handle);

    public static native boolean equals(Bech32Address lhs, Bech32Address rhs);
    public static native boolean isValidString(String string);
    public native String description();
    public native HRP hrp();
    public native byte[] witnessProgram();

    public Bech32Address(String string) {
        nativeHandle = nativeCreateWithString(string);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        Bech32AddressPhantomReference.register(this, nativeHandle);
    }

    public Bech32Address(HRP hrp, byte[] data) {
        nativeHandle = nativeCreateWithData(hrp, data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        Bech32AddressPhantomReference.register(this, nativeHandle);
    }

    public Bech32Address(HRP hrp, PublicKey publicKey) {
        nativeHandle = nativeCreateWithPublicKey(hrp, publicKey);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        Bech32AddressPhantomReference.register(this, nativeHandle);
    }

}

class Bech32AddressPhantomReference extends java.lang.ref.PhantomReference<Bech32Address> {
    private static java.util.Set<Bech32AddressPhantomReference> references = new HashSet<Bech32AddressPhantomReference>();
    private static java.lang.ref.ReferenceQueue<Bech32Address> queue = new java.lang.ref.ReferenceQueue<Bech32Address>();
    private long nativeHandle;

    private Bech32AddressPhantomReference(Bech32Address referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(Bech32Address referent, long nativeHandle) {
        references.add(new Bech32AddressPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        Bech32AddressPhantomReference ref = (Bech32AddressPhantomReference) queue.poll();
        for (; ref != null; ref = (Bech32AddressPhantomReference) queue.poll()) {
            Bech32Address.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
