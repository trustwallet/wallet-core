// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum P2PKHPrefix {
    BITCOIN ((byte) 0x00),
    LITECOIN ((byte) 0x30),
    DASH ((byte) 0x4C),
    ZCOIN ((byte) 0x52),
    ZCASHT ((byte) 0xB8);

    private final byte value;
    P2PKHPrefix(byte value) {
        this.value = value;
    }
    public byte value() { return value; }

    public static P2PKHPrefix createFromValue(byte value) {
        switch (value) {
            case (byte) 0x00: return P2PKHPrefix.BITCOIN;
            case (byte) 0x30: return P2PKHPrefix.LITECOIN;
            case (byte) 0x4C: return P2PKHPrefix.DASH;
            case (byte) 0x52: return P2PKHPrefix.ZCOIN;
            case (byte) 0xB8: return P2PKHPrefix.ZCASHT;
            default: return null;
        }
    }

}
