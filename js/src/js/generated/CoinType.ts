// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


import { Purpose } from './Purpose';
import { Curve } from './Curve';
import { HDVersion } from './HDVersion';
import { PrivateKey } from './PrivateKey';
import { PublicKey } from './PublicKey';


export enum CoinType {
    AION = 425,
    BINANCE = 714,
    BITCOIN = 0,
    BITCOINCASH = 145,
    CALLISTO = 820,
    COSMOS = 118,
    DASH = 5,
    DECRED = 42,
    ETHEREUM = 60,
    ETHEREUMCLASSIC = 61,
    GO = 6060,
    GROESTLCOIN = 17,
    ICON = 74,
    KIN = 2017,
    LITECOIN = 2,
    NEO = 888,
    NIMIQ = 242,
    ONTOLOGY = 1024,
    POA = 178,
    RIPPLE = 144,
    STELLAR = 148,
    TEZOS = 1729,
    THUNDERTOKEN = 1001,
    TOMOCHAIN = 889,
    TRON = 195,
    VECHAIN = 818,
    WANCHAIN = 5718350,
    XDAI = 700,
    ZCASH = 133,
    ZCOIN = 136,
    THETA = 500
}

export class CoinTypeUtil {

    public static purpose(coin: CoinType): Purpose { }
    public static curve(coin: CoinType): Curve { }
    public static xpubVersion(coin: CoinType): HDVersion { }
    public static xprvVersion(coin: CoinType): HDVersion { }
    public static validate(coin: CoinType, address: string): boolean { }
    public static derivationPath(coin: CoinType): string { }
    public static deriveAddress(coin: CoinType, privateKey: PrivateKey): string { }
    public static deriveAddressFromPublicKey(coin: CoinType, publicKey: PublicKey): string { }

}
