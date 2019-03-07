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

public class HDWallet {
    private long nativeHandle;

    private HDWallet() {
        nativeHandle = 0;
    }

    static HDWallet createFromNative(long nativeHandle) {
        HDWallet instance = new HDWallet();
        instance.nativeHandle = nativeHandle;
        HDWalletPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native long nativeCreate(int strength, String passphrase);
    static native long nativeCreateWithMnemonic(String mnemonic, String passphrase);
    static native long nativeCreateWithData(byte[] data, String passphrase);
    static native void nativeDelete(long handle);

    public static native boolean isValid(String mnemonic);
    public static native PublicKey getPublicKeyFromExtended(String extended, Curve curve, HDVersion versionPublic, HDVersion versionPrivate, int change, int address);
    public static native String getAddressFromExtended(String extended, Curve curve, CoinType coinType, int change, int address);
    public native byte[] seed();
    public native String mnemonic();
    public native PrivateKey getKey(CoinType coin, int account, int change, int address);
    public native String getExtendedPrivateKey(Purpose purpose, CoinType coin, HDVersion version);
    public native String getExtendedPublicKey(Purpose purpose, CoinType coin, HDVersion version);

    public HDWallet(int strength, String passphrase) {
        nativeHandle = nativeCreate(strength, passphrase);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        HDWalletPhantomReference.register(this, nativeHandle);
    }

    public HDWallet(String mnemonic, String passphrase) {
        nativeHandle = nativeCreateWithMnemonic(mnemonic, passphrase);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        HDWalletPhantomReference.register(this, nativeHandle);
    }

    public HDWallet(byte[] data, String passphrase) {
        nativeHandle = nativeCreateWithData(data, passphrase);
        if (nativeHandle == 0) {
            throw new InvalidParameterException();
        }

        HDWalletPhantomReference.register(this, nativeHandle);
    }

}

class HDWalletPhantomReference extends java.lang.ref.PhantomReference<HDWallet> {
    private static java.util.Set<HDWalletPhantomReference> references = new HashSet<HDWalletPhantomReference>();
    private static java.lang.ref.ReferenceQueue<HDWallet> queue = new java.lang.ref.ReferenceQueue<HDWallet>();
    private long nativeHandle;

    private HDWalletPhantomReference(HDWallet referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(HDWallet referent, long nativeHandle) {
        references.add(new HDWalletPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        HDWalletPhantomReference ref = (HDWalletPhantomReference) queue.poll();
        for (; ref != null; ref = (HDWalletPhantomReference) queue.poll()) {
            HDWallet.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
