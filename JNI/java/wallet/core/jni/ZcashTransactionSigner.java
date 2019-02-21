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

public class ZcashTransactionSigner {
    private long nativeHandle;

    private ZcashTransactionSigner() {
        nativeHandle = 0;
    }

    static ZcashTransactionSigner createFromNative(long nativeHandle) {
        ZcashTransactionSigner instance = new ZcashTransactionSigner();
        instance.nativeHandle = nativeHandle;
        ZcashTransactionSignerPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate(wallet.core.jni.proto.Bitcoin.SigningInput input);
    static native long nativeCreateWithPlan(wallet.core.jni.proto.Bitcoin.SigningInput input, wallet.core.jni.proto.Bitcoin.TransactionPlan plan);
    static native void nativeDelete(long handle);

    public native wallet.core.jni.proto.Bitcoin.TransactionPlan plan();
    public native wallet.core.jni.proto.Common.Result sign();

    public ZcashTransactionSigner(wallet.core.jni.proto.Bitcoin.SigningInput input) {
        nativeHandle = nativeCreate(input);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        ZcashTransactionSignerPhantomReference.register(this, nativeHandle);
    }

    public ZcashTransactionSigner(wallet.core.jni.proto.Bitcoin.SigningInput input, wallet.core.jni.proto.Bitcoin.TransactionPlan plan) {
        nativeHandle = nativeCreateWithPlan(input, plan);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        ZcashTransactionSignerPhantomReference.register(this, nativeHandle);
    }

}

class ZcashTransactionSignerPhantomReference extends java.lang.ref.PhantomReference<ZcashTransactionSigner> {
    private static java.util.Set<ZcashTransactionSignerPhantomReference> references = new HashSet<ZcashTransactionSignerPhantomReference>();
    private static java.lang.ref.ReferenceQueue<ZcashTransactionSigner> queue = new java.lang.ref.ReferenceQueue<ZcashTransactionSigner>();
    private long nativeHandle;

    private ZcashTransactionSignerPhantomReference(ZcashTransactionSigner referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(ZcashTransactionSigner referent, long nativeHandle) {
        references.add(new ZcashTransactionSignerPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        ZcashTransactionSignerPhantomReference ref = (ZcashTransactionSignerPhantomReference) queue.poll();
        for (; ref != null; ref = (ZcashTransactionSignerPhantomReference) queue.poll()) {
            ZcashTransactionSigner.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
