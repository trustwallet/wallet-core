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

public class BinanceSigner {
    private long nativeHandle;

    private BinanceSigner() {
        nativeHandle = 0;
    }

    static BinanceSigner createFromNative(long nativeHandle) {
        BinanceSigner instance = new BinanceSigner();
        instance.nativeHandle = nativeHandle;
        BinanceSignerPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate(wallet.core.jni.proto.Binance.SigningInput input);
    static native void nativeDelete(long handle);

    public native byte[] build();

    public BinanceSigner(wallet.core.jni.proto.Binance.SigningInput input) {
        nativeHandle = nativeCreate(input);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        BinanceSignerPhantomReference.register(this, nativeHandle);
    }

}

class BinanceSignerPhantomReference extends java.lang.ref.PhantomReference<BinanceSigner> {
    private static java.util.Set<BinanceSignerPhantomReference> references = new HashSet<BinanceSignerPhantomReference>();
    private static java.lang.ref.ReferenceQueue<BinanceSigner> queue = new java.lang.ref.ReferenceQueue<BinanceSigner>();
    private long nativeHandle;

    private BinanceSignerPhantomReference(BinanceSigner referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(BinanceSigner referent, long nativeHandle) {
        references.add(new BinanceSignerPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        BinanceSignerPhantomReference ref = (BinanceSignerPhantomReference) queue.poll();
        for (; ref != null; ref = (BinanceSignerPhantomReference) queue.poll()) {
            BinanceSigner.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
