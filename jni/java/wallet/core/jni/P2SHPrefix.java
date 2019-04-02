// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum P2SHPrefix {
    BITCOIN ((byte) 0x05),
    LITECOIN ((byte) 0x32),
    DASH ((byte) 0x10),
    DECRED ((byte) 0x1a),
    ZCOIN ((byte) 0x07),
    ZCASHT ((byte) 0xBD);

    private final byte value;
    P2SHPrefix(byte value) {
        this.value = value;
    }
    public byte value() { return value; }

    public static P2SHPrefix createFromValue(byte value) {
        switch (value) {
            case (byte) 0x05: return P2SHPrefix.BITCOIN;
            case (byte) 0x32: return P2SHPrefix.LITECOIN;
            case (byte) 0x10: return P2SHPrefix.DASH;
            case (byte) 0x1a: return P2SHPrefix.DECRED;
            case (byte) 0x07: return P2SHPrefix.ZCOIN;
            case (byte) 0xBD: return P2SHPrefix.ZCASHT;
            default: return null;
        }
    }

}
