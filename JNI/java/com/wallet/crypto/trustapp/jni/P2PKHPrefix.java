// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package com.wallet.crypto.trustapp.jni;


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
}
