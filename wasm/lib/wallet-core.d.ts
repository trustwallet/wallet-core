// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

export class CoinType {
    value: number;

    static ethereum: CoinType;
    static solana: CoinType;
    static bitcoin: CoinType;
    static litecoin: CoinType;
    static dogecoin: CoinType;
    static cosmos: CoinType;
    static binance: CoinType;
    static smartChain: CoinType;
}

export class Curve {
    value: number;
    static secp256k1: Curve;
    static ed25519: Curve;
}

export class HDWallet {
    static create(strength: number, passphrase: Uint8Array | Buffer): HDWallet;
    static createWithMnemonic(mnemonic: string, passphrase: string): HDWallet;
    getAddressForCoin(coin: CoinType): string;
    mnemonic(): string;
    delete(): void;
}

export class Mnemonic {
    static isValid(mnemonic: string): boolean;
    static isValidWord(word: string): boolean;
    static suggest(hint: string): string;
}

export enum AESPaddingMode {
    zero = 0,
    pkcs7 = 1,
}

export class AES {
    static decryptCBC(ciphertext: Uint8Array | Buffer , key: Uint8Array | Buffer, iv: Uint8Array | Buffer, paddingMode: AESPaddingMode): Uint8Array;
    static encryptCBC(key: Uint8Array | Buffer, plaintext: Uint8Array | Buffer, iv: Uint8Array | Buffer, paddingMode: AESPaddingMode): Uint8Array;
}

export namespace HexCoding {
    export function decode(hex: string): Uint8Array;
    export function encode(buffer: Uint8Array | Buffer): string;
}

export namespace Hash {
    export function keccak256(data: Uint8Array | Buffer): Uint8Array;
    export function sha256(data: Uint8Array | Buffer): Uint8Array;
    export function sha512_256(data: Uint8Array | Buffer): Uint8Array;
}

export class PBKDF2 {
    static hmacSha256(password: Uint8Array | Buffer, salt: Uint8Array | Buffer, iterations: number, outputLength: number): Uint8Array;
}

export class AnyAddress {
    static createWithString(address: string, coin: CoinType): AnyAddress;
    static createWithPublicKey(pubkey: PublicKey, coin: CoinType): AnyAddress;
    coin(): CoinType;
    data(): Uint8Array;
    description(): string;
    delete(): void;
}

export class StoredKey {
    static importHDWallet(mnemonic: string, name: string, password: Uint8Array | Buffer, coin: CoinType): StoredKey;
    isMnemonic(): boolean;
    exportJSON(): string;
    delete(): void;
}

export class PrivateKey {
    static isValid(data: Uint8Array | Buffer, curve: Curve): boolean;
    static createWithData(data: Uint8Array | Buffer): PrivateKey;
    getPublicKeySecp256k1(compressed: boolean): PublicKey;
    getPublicKeyEd25519(): PublicKey;
    getPublicKey(curve: Curve): PublicKey;
    sign(data: Uint8Array | Buffer, curve: Curve): Uint8Array;
    delete(): void;
}

export class PublicKey {    
    data(): Uint8Array;
    delete(): void;
}

export class AnySigner {
    static sign(data: Uint8Array | Buffer, coin: CoinType): Uint8Array;
}

export class EthereumAbi {
    static encodeTyped(message: string): Uint8Array;
}
