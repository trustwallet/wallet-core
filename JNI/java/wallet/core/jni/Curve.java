// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum Curve {
    SECP256K1 (0),
    ED25519 (1);

    private final int value;
    Curve(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static Curve createFromValue(int value) {
        switch (value) {
            case 0: return Curve.SECP256K1;
            case 1: return Curve.ED25519;
            default: return null;
        }
    }


    public String toString() {
        switch (this) {
        case SECP256K1: return "secp256k1";
        case ED25519: return "ed25519";
        default: return "";
        }
    }
}
