// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


import { WalletCore } from '../WalletCore';

import { Blockchain } from './Blockchain';
import { Purpose } from './Purpose';
import { Curve } from './Curve';
import { HDVersion } from './HDVersion';
import { HRP } from './HRP';
import { PrivateKey } from './PrivateKey';
import { PublicKey } from './PublicKey';

export enum CoinType {
    AETERNITY = 457,
    AION = 425,
    BINANCE = 714,
    COINEX = 688,
    BITCOIN = 0,
    BITCOINCASH = 145,
    BITCOINSV = 236,
    SIMPLELEDGER = 245,
    BRAVOCOIN = 282,
    CALLISTO = 820,
    COSMOS = 118,
    DASH = 5,
    DECRED = 42,
    DIGIBYTE = 20,
    DOGECOIN = 3,
    ELLAISM = 163,
    EOS = 194,
    ETHEREUM = 60,
    ETHEREUMCLASSIC = 61,
    ETHERSOCIAL = 31102,
    FIO = 235,
    GOCHAIN = 6060,
    GROESTLCOIN = 17,
    ICON = 74,
    IOST = 291,
    IOTEX = 304,
    KIN = 2017,
    LITECOIN = 2,
    MONACOIN = 22,
    NEBULAS = 2718,
    NULS = 8964,
    LUX = 3003,
    NANO = 165,
    NEO = 888,
    NIMIQ = 242,
    ONTOLOGY = 1024,
    POANETWORK = 178,
    QTUM = 2301,
    XRP = 144,
    STEEM = 135,
    STELLAR = 148,
    TEZOS = 1729,
    THETA = 500,
    THUNDERTOKEN = 1001,
    TOMOCHAIN = 889,
    TRON = 195,
    VECHAIN = 818,
    VIACOIN = 14,
    WANCHAIN = 5718350,
    XDAI = 700,
    ZCASH = 133,
    ZCOIN = 136,
    ZILLIQA = 313,
    SEMUX = 7562605,
    DEXON = 237,
    ZELCASH = 19167,
    ARK = 111,
    RAVENCOIN = 175,
    WAVES = 5741564,
    TERRA = 330
}

export class CoinTypeUtil {

    public static blockchain(coin: CoinType): Blockchain {
        return WalletCore.Js_wallet_core_napi_CoinType_blockchain(coin);
    }

    public static purpose(coin: CoinType): Purpose {
        return WalletCore.Js_wallet_core_napi_CoinType_purpose(coin);
    }

    public static curve(coin: CoinType): Curve {
        return WalletCore.Js_wallet_core_napi_CoinType_curve(coin);
    }

    public static xpubVersion(coin: CoinType): HDVersion {
        return WalletCore.Js_wallet_core_napi_CoinType_xpubVersion(coin);
    }

    public static xprvVersion(coin: CoinType): HDVersion {
        return WalletCore.Js_wallet_core_napi_CoinType_xprvVersion(coin);
    }

    public static hrp(coin: CoinType): HRP {
        return WalletCore.Js_wallet_core_napi_CoinType_hrp(coin);
    }

    public static p2pkhPrefix(coin: CoinType): number {
        return WalletCore.Js_wallet_core_napi_CoinType_p2pkhPrefix(coin);
    }

    public static p2shPrefix(coin: CoinType): number {
        return WalletCore.Js_wallet_core_napi_CoinType_p2shPrefix(coin);
    }

    public static staticPrefix(coin: CoinType): number {
        return WalletCore.Js_wallet_core_napi_CoinType_staticPrefix(coin);
    }

    public static validate(coin: CoinType, address: string): boolean {
        return WalletCore.Js_wallet_core_napi_CoinType_validate(coin, address);
    }

    public static derivationPath(coin: CoinType): string {
        return WalletCore.Js_wallet_core_napi_CoinType_derivationPath(coin);
    }

    public static deriveAddress(coin: CoinType, privateKey: PrivateKey): string {
        return WalletCore.Js_wallet_core_napi_CoinType_deriveAddress(coin, privateKey.getNativeHandle());
    }

    public static deriveAddressFromPublicKey(coin: CoinType, publicKey: PublicKey): string {
        return WalletCore.Js_wallet_core_napi_CoinType_deriveAddressFromPublicKey(coin, publicKey.getNativeHandle());
    }

}
