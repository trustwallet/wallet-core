// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


import { WalletCore } from '../WalletCore';

export enum HDVersion {
    NONE = 0,
    XPUB = 0x0488b21e,
    XPRV = 0x0488ade4,
    YPUB = 0x049d7cb2,
    YPRV = 0x049d7878,
    ZPUB = 0x04b24746,
    ZPRV = 0x04b2430c,
    LTUB = 0x019da462,
    LTPV = 0x019d9cfe,
    MTUB = 0x01b26ef6,
    MTPV = 0x01b26792,
    DPUB = 0x2fda926,
    DPRV = 0x2fda4e8,
    DGUB = 0x02facafd,
    DGPV = 0x02fac398
}

export class HDVersionUtil {

    public static isPublic(version: HDVersion): boolean {
        return WalletCore.Js_wallet_core_napi_HDVersion_isPublic(version);
    }

    public static isPrivate(version: HDVersion): boolean {
        return WalletCore.Js_wallet_core_napi_HDVersion_isPrivate(version);
    }

}
