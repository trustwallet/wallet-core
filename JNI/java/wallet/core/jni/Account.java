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

public class Account {
    private long nativeHandle;

    private Account() {
        nativeHandle = 0;
    }

    static Account createFromNative(long nativeHandle) {
        Account instance = new Account();
        instance.nativeHandle = nativeHandle;
        AccountPhantomReference.register(instance, nativeHandle);
        return instance;
    }

    static native void nativeDelete(long handle);

    public native String address();
    public native String derivationPath();
    public native String extendedPublicKey();
    public native CoinType coin();

}

class AccountPhantomReference extends java.lang.ref.PhantomReference<Account> {
    private static java.util.Set<AccountPhantomReference> references = new HashSet<AccountPhantomReference>();
    private static java.lang.ref.ReferenceQueue<Account> queue = new java.lang.ref.ReferenceQueue<Account>();
    private long nativeHandle;

    private AccountPhantomReference(Account referent, long nativeHandle) {
        super(referent, queue);
        this.nativeHandle = nativeHandle;
    }

    static void register(Account referent, long nativeHandle) {
        references.add(new AccountPhantomReference(referent, nativeHandle));
    }

    public static void doDeletes() {
        AccountPhantomReference ref = (AccountPhantomReference) queue.poll();
        for (; ref != null; ref = (AccountPhantomReference) queue.poll()) {
            Account.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }
}
