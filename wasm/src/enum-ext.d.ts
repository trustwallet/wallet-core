// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

export class BitcoinSigHashTypeExt {
    static isSingle(type: BitcoinSigHashType): boolean;
    static isNone(type: BitcoinSigHashType): boolean;
}

export class CoinTypeExt {
    static blockchain(coin: CoinType): Blockchain;
    static purpose(coin: CoinType): Purpose;
    static curve(coin: CoinType): Curve;
    static xpubVersion(coin: CoinType): HDVersion;
    static xprvVersion(coin: CoinType): HDVersion;
    static hrp(coin: CoinType): HRP;
    static p2pkhPrefix(coin: CoinType): number;
    static p2shPrefix(coin: CoinType): number;
    static staticPrefix(coin: CoinType): number;
    static chainId(coin: CoinType): string;
    static slip44Id(coin: CoinType): number;
    static ss58Prefix(coin: CoinType): number;
    static publicKeyType(coin: CoinType): PublicKeyType;
    static validate(coin: CoinType, address: string): boolean;
    static derivationPath(coin: CoinType): string;
    static derivationPathWithDerivation(coin: CoinType, derivation: Derivation): string;
    static deriveAddress(coin: CoinType, privateKey: PrivateKey): string;
    static deriveAddressFromPublicKey(coin: CoinType, publicKey: PublicKey): string;
}

export class HDVersionExt {
    static isPublic(version: HDVersion): boolean;
    static isPrivate(version: HDVersion): boolean;
}
