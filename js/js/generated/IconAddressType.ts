// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


import { WalletCore } from '../WalletCore';

export enum IconAddressType {
    ADDRESS = 0,
    CONTRACT = 1
}

export class IconAddressTypeUtil {

    public static toString(iconaddresstype: IconAddressType): string {
        switch (iconaddresstype) {
            case IconAddressType.ADDRESS: return 'hx';
            case IconAddressType.CONTRACT: return 'cx';
            default: return '';
        }
    }

}
