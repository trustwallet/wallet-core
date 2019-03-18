// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum StellarPassphrase {
    STELLAR (0),
    KIN (1);

    private final int value;
    StellarPassphrase(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static StellarPassphrase createFromValue(int value) {
        switch (value) {
            case 0: return StellarPassphrase.STELLAR;
            case 1: return StellarPassphrase.KIN;
            default: return null;
        }
    }


    public String toString() {
        switch (this) {
        case STELLAR: return "Public Global Stellar Network ; September 2015";
        case KIN: return "Kin Mainnet ; December 2018";
        default: return "";
        }
    }
}
