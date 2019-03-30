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

public class DecredSigner {
    private long nativeHandle;

    private DecredSigner() {
        nativeHandle = 0;
    }

    static DecredSigner createFromNative(long nativeHandle) {
        DecredSigner instance = new DecredSigner();
        instance.nativeHandle = nativeHandle;
        DecredSignerPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate(wallet.core.jni.proto.Bitcoin.SigningInput input);
    static native long nativeCreateWithPlan(wallet.core.jni.proto.Bitcoin.SigningInput input, wallet.core.jni.proto.Bitcoin.TransactionPlan plan);
    static native void nativeDelete(long handle);

    public native wallet.core.jni.proto.Bitcoin.TransactionPlan plan();
    public native wallet.core.jni.proto.Common.Result sign();

    public DecredSigner(wallet.core.jni.proto.Bitcoin.SigningInput input) {
        nativeHandle = nativeCreate(input);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        DecredSignerPhantomReference.register(this, nativeHandle);
    }

    public DecredSigner(wallet.core.jni.proto.Bitcoin.SigningInput input, wallet.core.jni.proto.Bitcoin.TransactionPlan plan) {
        nativeHandle = nativeCreateWithPlan(input, plan);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        DecredSignerPhantomReference.register(this, nativeHandle);
    }

}

class DecredSignerPhantomReference extends java.lang.ref.PhantomReference<DecredSigner> {
    private static java.util.Set<DecredSignerPhantomReference> references = new HashSet<DecredSignerPhantomReference>();
    private static java.lang.ref.ReferenceQueue<DecredSigner> queue = new java.lang.ref.ReferenceQueue<DecredSigner>();
    private long nativeHandle;

    private DecredSignerPhantomReference(DecredSigner referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(DecredSigner referent, long nativeHandle) {
        references.add(new DecredSignerPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        DecredSignerPhantomReference ref = (DecredSignerPhantomReference) queue.poll();
        for (; ref != null; ref = (DecredSignerPhantomReference) queue.poll()) {
            DecredSigner.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
