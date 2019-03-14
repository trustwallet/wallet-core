// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum StellarMemoType {
    NONE (0),
    TEXT (1),
    ID (2),
    HASH (3),
    RETURN (4);

    private final int value;
    StellarMemoType(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static StellarMemoType createFromValue(int value) {
        switch (value) {
            case 0: return StellarMemoType.NONE;
            case 1: return StellarMemoType.TEXT;
            case 2: return StellarMemoType.ID;
            case 3: return StellarMemoType.HASH;
            case 4: return StellarMemoType.RETURN;
            default: return null;
        }
    }

}
