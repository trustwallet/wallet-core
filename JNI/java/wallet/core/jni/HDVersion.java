// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum HDVersion {
    NONE (0),
    XPUB (0x0488b21e),
    XPRV (0x0488ade4),
    YPUB (0x049d7cb2),
    YPRV (0x049d7878),
    ZPUB (0x04b24746),
    ZPRV (0x04b2430c),
    LTUB (0x019da462),
    LTPV (0x019d9cfe),
    MTUB (0x01b26ef6),
    MTPV (0x01b26792);

    private final int value;
    HDVersion(int value) {
        this.value = value;
    }
    public int value() { return value; }

    public static HDVersion createFromValue(int value) {
        switch (value) {
            case 0: return HDVersion.NONE;
            case 0x0488b21e: return HDVersion.XPUB;
            case 0x0488ade4: return HDVersion.XPRV;
            case 0x049d7cb2: return HDVersion.YPUB;
            case 0x049d7878: return HDVersion.YPRV;
            case 0x04b24746: return HDVersion.ZPUB;
            case 0x04b2430c: return HDVersion.ZPRV;
            case 0x019da462: return HDVersion.LTUB;
            case 0x019d9cfe: return HDVersion.LTPV;
            case 0x01b26ef6: return HDVersion.MTUB;
            case 0x01b26792: return HDVersion.MTPV;
            default: return null;
        }
    }

}
