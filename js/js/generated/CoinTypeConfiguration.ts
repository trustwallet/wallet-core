// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import { WalletCore } from '../WalletCore';
import { TW } from '../../proto/TrustWalletCore';
import * as Long from 'long';

import { CoinType } from './CoinType';

// Struct
export class CoinTypeConfiguration {

    public static getSymbol(type: CoinType): string {
        return WalletCore.Js_wallet_core_napi_CoinTypeConfiguration_getSymbol(type);
    }

    public static getDecimals(type: CoinType): number {
        return WalletCore.Js_wallet_core_napi_CoinTypeConfiguration_getDecimals(type);
    }

    public static getTransactionURL(type: CoinType, transactionID: string): string {
        return WalletCore.Js_wallet_core_napi_CoinTypeConfiguration_getTransactionURL(type, transactionID);
    }

    public static getID(type: CoinType): string {
        return WalletCore.Js_wallet_core_napi_CoinTypeConfiguration_getID(type);
    }

    public static getName(type: CoinType): string {
        return WalletCore.Js_wallet_core_napi_CoinTypeConfiguration_getName(type);
    }

}
