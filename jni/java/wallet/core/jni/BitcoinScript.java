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

public class BitcoinScript {
    private long nativeHandle;

    static BitcoinScript createFromNative(long nativeHandle) {
        BitcoinScript instance = new BitcoinScript();
        instance.nativeHandle = nativeHandle;
        BitcoinScriptPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate();
    static native long nativeCreateWithData(byte[] data);
    static native long nativeCreateCopy(BitcoinScript script);
    static native void nativeDelete(long handle);

    public static native boolean equals(BitcoinScript lhs, BitcoinScript rhs);
    public static native BitcoinScript buildPayToPublicKeyHash(byte[] hash);
    public static native BitcoinScript buildPayToScriptHash(byte[] scriptHash);
    public static native BitcoinScript buildPayToWitnessPubkeyHash(byte[] hash);
    public static native BitcoinScript buildPayToWitnessScriptHash(byte[] scriptHash);
    public static native BitcoinScript buildForAddress(String address);
    public native int size();
    public native byte[] data();
    public native byte[] scriptHash();
    public native boolean isPayToScriptHash();
    public native boolean isPayToWitnessScriptHash();
    public native boolean isWitnessProgram();
    public native byte[] matchPayToPubkey();
    public native byte[] matchPayToPubkeyHash();
    public native byte[] matchPayToScriptHash();
    public native byte[] matchPayToWitnessPublicKeyHash();
    public native byte[] matchPayToWitnessScriptHash();
    public native byte[] encode();

    public BitcoinScript() {
        nativeHandle = nativeCreate();
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        BitcoinScriptPhantomReference.register(this, nativeHandle);
    }

    public BitcoinScript(byte[] data) {
        nativeHandle = nativeCreateWithData(data);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        BitcoinScriptPhantomReference.register(this, nativeHandle);
    }

    public BitcoinScript(BitcoinScript script) {
        nativeHandle = nativeCreateCopy(script);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        BitcoinScriptPhantomReference.register(this, nativeHandle);
    }

}

class BitcoinScriptPhantomReference extends java.lang.ref.PhantomReference<BitcoinScript> {
    private static java.util.Set<BitcoinScriptPhantomReference> references = new HashSet<BitcoinScriptPhantomReference>();
    private static java.lang.ref.ReferenceQueue<BitcoinScript> queue = new java.lang.ref.ReferenceQueue<BitcoinScript>();
    private long nativeHandle;

    private BitcoinScriptPhantomReference(BitcoinScript referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(BitcoinScript referent, long nativeHandle) {
        references.add(new BitcoinScriptPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        BitcoinScriptPhantomReference ref = (BitcoinScriptPhantomReference) queue.poll();
        for (; ref != null; ref = (BitcoinScriptPhantomReference) queue.poll()) {
            BitcoinScript.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
