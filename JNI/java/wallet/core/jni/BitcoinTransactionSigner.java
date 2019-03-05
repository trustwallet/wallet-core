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

public class BitcoinTransactionSigner {
    private long nativeHandle;

    private BitcoinTransactionSigner() {
        nativeHandle = 0;
    }

    static BitcoinTransactionSigner createFromNative(long nativeHandle) {
        BitcoinTransactionSigner instance = new BitcoinTransactionSigner();
        instance.nativeHandle = nativeHandle;
        BitcoinTransactionSignerPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate(wallet.core.jni.proto.Bitcoin.SigningInput input);
    static native void nativeDelete(long handle);

    public native wallet.core.jni.proto.Bitcoin.TransactionPlan plan();
    public native wallet.core.jni.proto.Common.Result sign();

    public BitcoinTransactionSigner(wallet.core.jni.proto.Bitcoin.SigningInput input) {
        nativeHandle = nativeCreate(input);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        BitcoinTransactionSignerPhantomReference.register(this, nativeHandle);
    }

}

class BitcoinTransactionSignerPhantomReference extends java.lang.ref.PhantomReference<BitcoinTransactionSigner> {
    private static java.util.Set<BitcoinTransactionSignerPhantomReference> references = new HashSet<BitcoinTransactionSignerPhantomReference>();
    private static java.lang.ref.ReferenceQueue<BitcoinTransactionSigner> queue = new java.lang.ref.ReferenceQueue<BitcoinTransactionSigner>();
    private long nativeHandle;

    private BitcoinTransactionSignerPhantomReference(BitcoinTransactionSigner referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(BitcoinTransactionSigner referent, long nativeHandle) {
        references.add(new BitcoinTransactionSignerPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        BitcoinTransactionSignerPhantomReference ref = (BitcoinTransactionSignerPhantomReference) queue.poll();
        for (; ref != null; ref = (BitcoinTransactionSignerPhantomReference) queue.poll()) {
            BitcoinTransactionSigner.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
