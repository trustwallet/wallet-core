// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum IconAddressType {
    ADDRESS (0),
    CONTRACT (1);

    private final int value;
    IconAddressType(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public String toString() {
        switch (this) {
        case ADDRESS: return "hx";
        case CONTRACT: return "cx";
        default: return "";
        }
    }
}
