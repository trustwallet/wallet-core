// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//
export class AnySigner {
    static sign(data: Uint8Array | Buffer, coin: CoinType): Uint8Array;
    static plan(data: Uint8Array | Buffer, coin: CoinType): Uint8Array;
    static supportsJSON(coin: CoinType): boolean;
}
export namespace HexCoding {
    export function decode(hex: string): Uint8Array;
    export function encode(buffer: Uint8Array | Buffer): string;
}
export class NEARAccount {
    static createWithString(string: string): NEARAccount;
    description(): string;
    delete(): void;
}
export class TransactionCompiler {
    static buildInput(coinType: CoinType, from: string, to: string, amount: string, asset: string, memo: string, chainId: string): Uint8Array;
    static preImageHashes(coinType: CoinType, txInputData: Uint8Array | Buffer): Uint8Array;
    static compileWithSignatures(coinType: CoinType, txInputData: Uint8Array | Buffer, signatures: DataVector, publicKeys: DataVector): Uint8Array;
}
export class PublicKeyType {
    value: number;
    static secp256k1: PublicKeyType;
    static secp256k1Extended: PublicKeyType;
    static nist256p1: PublicKeyType;
    static nist256p1Extended: PublicKeyType;
    static ed25519: PublicKeyType;
    static ed25519Blake2b: PublicKeyType;
    static curve25519: PublicKeyType;
    static ed25519Cardano: PublicKeyType;
}
export class Cardano {
    static minAdaAmount(tokenBundle: Uint8Array | Buffer): number;
}
export class EthereumAbiFunction {
    static createWithString(name: string): EthereumAbiFunction;
    getType(): string;
    addParamUInt8(val: number, isOutput: boolean): number;
    addParamUInt16(val: number, isOutput: boolean): number;
    addParamUInt32(val: number, isOutput: boolean): number;
    addParamUInt64(val: number, isOutput: boolean): number;
    addParamUInt256(val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamUIntN(bits: number, val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamInt8(val: number, isOutput: boolean): number;
    addParamInt16(val: number, isOutput: boolean): number;
    addParamInt32(val: number, isOutput: boolean): number;
    addParamInt64(val: number, isOutput: boolean): number;
    addParamInt256(val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamIntN(bits: number, val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamBool(val: boolean, isOutput: boolean): number;
    addParamString(val: string, isOutput: boolean): number;
    addParamAddress(val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamBytes(val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamBytesFix(size: number, val: Uint8Array | Buffer, isOutput: boolean): number;
    addParamArray(isOutput: boolean): number;
    getParamUInt8(idx: number, isOutput: boolean): number;
    getParamUInt64(idx: number, isOutput: boolean): number;
    getParamUInt256(idx: number, isOutput: boolean): Uint8Array;
    getParamBool(idx: number, isOutput: boolean): boolean;
    getParamString(idx: number, isOutput: boolean): string;
    getParamAddress(idx: number, isOutput: boolean): Uint8Array;
    addInArrayParamUInt8(arrayIdx: number, val: number): number;
    addInArrayParamUInt16(arrayIdx: number, val: number): number;
    addInArrayParamUInt32(arrayIdx: number, val: number): number;
    addInArrayParamUInt64(arrayIdx: number, val: number): number;
    addInArrayParamUInt256(arrayIdx: number, val: Uint8Array | Buffer): number;
    addInArrayParamUIntN(arrayIdx: number, bits: number, val: Uint8Array | Buffer): number;
    addInArrayParamInt8(arrayIdx: number, val: number): number;
    addInArrayParamInt16(arrayIdx: number, val: number): number;
    addInArrayParamInt32(arrayIdx: number, val: number): number;
    addInArrayParamInt64(arrayIdx: number, val: number): number;
    addInArrayParamInt256(arrayIdx: number, val: Uint8Array | Buffer): number;
    addInArrayParamIntN(arrayIdx: number, bits: number, val: Uint8Array | Buffer): number;
    addInArrayParamBool(arrayIdx: number, val: boolean): number;
    addInArrayParamString(arrayIdx: number, val: string): number;
    addInArrayParamAddress(arrayIdx: number, val: Uint8Array | Buffer): number;
    addInArrayParamBytes(arrayIdx: number, val: Uint8Array | Buffer): number;
    addInArrayParamBytesFix(arrayIdx: number, size: number, val: Uint8Array | Buffer): number;
    delete(): void;
}
export class StellarVersionByte {
    value: number;
    static accountID: StellarVersionByte;
    static seed: StellarVersionByte;
    static preAuthTX: StellarVersionByte;
    static sha256Hash: StellarVersionByte;
}
export class FIOAccount {
    static createWithString(string: string): FIOAccount;
    description(): string;
    delete(): void;
}
export class BitcoinAddress {
    static equal(lhs: BitcoinAddress, rhs: BitcoinAddress): boolean;
    static isValid(data: Uint8Array | Buffer): boolean;
    static isValidString(string: string): boolean;
    static createWithString(string: string): BitcoinAddress;
    static createWithData(data: Uint8Array | Buffer): BitcoinAddress;
    static createWithPublicKey(publicKey: PublicKey, prefix: number): BitcoinAddress;
    description(): string;
    prefix(): number;
    keyhash(): Uint8Array;
    delete(): void;
}
export class Purpose {
    value: number;
    static bip44: Purpose;
    static bip49: Purpose;
    static bip84: Purpose;
    static bip1852: Purpose;
}
export class AES {
    static encryptCBC(key: Uint8Array | Buffer, data: Uint8Array | Buffer, iv: Uint8Array | Buffer, mode: AESPaddingMode): Uint8Array;
    static decryptCBC(key: Uint8Array | Buffer, data: Uint8Array | Buffer, iv: Uint8Array | Buffer, mode: AESPaddingMode): Uint8Array;
    static encryptCTR(key: Uint8Array | Buffer, data: Uint8Array | Buffer, iv: Uint8Array | Buffer): Uint8Array;
    static decryptCTR(key: Uint8Array | Buffer, data: Uint8Array | Buffer, iv: Uint8Array | Buffer): Uint8Array;
}
export class HDVersion {
    value: number;
    static none: HDVersion;
    static xpub: HDVersion;
    static xprv: HDVersion;
    static ypub: HDVersion;
    static yprv: HDVersion;
    static zpub: HDVersion;
    static zprv: HDVersion;
    static ltub: HDVersion;
    static ltpv: HDVersion;
    static mtub: HDVersion;
    static mtpv: HDVersion;
    static dpub: HDVersion;
    static dprv: HDVersion;
    static dgub: HDVersion;
    static dgpv: HDVersion;
}
export class Curve {
    value: number;
    static secp256k1: Curve;
    static ed25519: Curve;
    static ed25519Blake2bNano: Curve;
    static curve25519: Curve;
    static nist256p1: Curve;
    static ed25519ExtendedCardano: Curve;
}

declare function describeCurve(value: Curve): string;

export class PublicKey {
    static isValid(data: Uint8Array | Buffer, type: PublicKeyType): boolean;
    static recover(signature: Uint8Array | Buffer, message: Uint8Array | Buffer): PublicKey;
    static createWithData(data: Uint8Array | Buffer, type: PublicKeyType): PublicKey;
    isCompressed(): boolean;
    compressed(): PublicKey;
    uncompressed(): PublicKey;
    data(): Uint8Array;
    keyType(): PublicKeyType;
    description(): string;
    verify(signature: Uint8Array | Buffer, message: Uint8Array | Buffer): boolean;
    verifyAsDER(signature: Uint8Array | Buffer, message: Uint8Array | Buffer): boolean;
    verifyZilliqaSchnorr(signature: Uint8Array | Buffer, message: Uint8Array | Buffer): boolean;
    delete(): void;
}
export class AnyAddress {
    static equal(lhs: AnyAddress, rhs: AnyAddress): boolean;
    static isValid(string: string, coin: CoinType): boolean;
    static createWithString(string: string, coin: CoinType): AnyAddress;
    static createWithPublicKey(publicKey: PublicKey, coin: CoinType): AnyAddress;
    description(): string;
    coin(): CoinType;
    data(): Uint8Array;
    delete(): void;
}
export class EthereumAbiValue {
    static encodeBool(value: boolean): Uint8Array;
    static encodeInt32(value: number): Uint8Array;
    static encodeUInt32(value: number): Uint8Array;
    static encodeInt256(value: Uint8Array | Buffer): Uint8Array;
    static encodeUInt256(value: Uint8Array | Buffer): Uint8Array;
    static encodeAddress(value: Uint8Array | Buffer): Uint8Array;
    static encodeString(value: string): Uint8Array;
    static encodeBytes(value: Uint8Array | Buffer): Uint8Array;
    static encodeBytesDyn(value: Uint8Array | Buffer): Uint8Array;
    static decodeUInt256(input: Uint8Array | Buffer): string;
    static decodeValue(input: Uint8Array | Buffer, type: string): string;
    static decodeArray(input: Uint8Array | Buffer, type: string): string;
}
export class Mnemonic {
    static isValid(mnemonic: string): boolean;
    static isValidWord(word: string): boolean;
    static suggest(prefix: string): string;
}
export class StellarMemoType {
    value: number;
    static none: StellarMemoType;
    static text: StellarMemoType;
    static id: StellarMemoType;
    static hash: StellarMemoType;
    static return: StellarMemoType;
}
export class Blockchain {
    value: number;
    static bitcoin: Blockchain;
    static ethereum: Blockchain;
    static vechain: Blockchain;
    static tron: Blockchain;
    static icon: Blockchain;
    static binance: Blockchain;
    static ripple: Blockchain;
    static tezos: Blockchain;
    static nimiq: Blockchain;
    static stellar: Blockchain;
    static aion: Blockchain;
    static cosmos: Blockchain;
    static theta: Blockchain;
    static ontology: Blockchain;
    static zilliqa: Blockchain;
    static ioTeX: Blockchain;
    static eos: Blockchain;
    static nano: Blockchain;
    static nuls: Blockchain;
    static waves: Blockchain;
    static aeternity: Blockchain;
    static nebulas: Blockchain;
    static fio: Blockchain;
    static solana: Blockchain;
    static harmony: Blockchain;
    static near: Blockchain;
    static algorand: Blockchain;
    static polkadot: Blockchain;
    static cardano: Blockchain;
    static neo: Blockchain;
    static filecoin: Blockchain;
    static elrondNetwork: Blockchain;
    static oasisNetwork: Blockchain;
    static decred: Blockchain;
    static zcash: Blockchain;
    static groestlcoin: Blockchain;
    static thorchain: Blockchain;
    static ronin: Blockchain;
    static kusama: Blockchain;
}
export class BitcoinSigHashType {
    value: number;
    static all: BitcoinSigHashType;
    static none: BitcoinSigHashType;
    static single: BitcoinSigHashType;
    static fork: BitcoinSigHashType;
    static forkBTG: BitcoinSigHashType;
}
export class SegwitAddress {
    static equal(lhs: SegwitAddress, rhs: SegwitAddress): boolean;
    static isValidString(string: string): boolean;
    static createWithString(string: string): SegwitAddress;
    static createWithPublicKey(hrp: HRP, publicKey: PublicKey): SegwitAddress;
    description(): string;
    hrp(): HRP;
    witnessVersion(): number;
    witnessProgram(): Uint8Array;
    delete(): void;
}
export class HDWallet {
    static getPublicKeyFromExtended(extended: string, coin: CoinType, derivationPath: string): PublicKey;
    static create(strength: number, passphrase: string): HDWallet;
    static createWithMnemonic(mnemonic: string, passphrase: string): HDWallet;
    static createWithMnemonicCheck(mnemonic: string, passphrase: string, check: boolean): HDWallet;
    static createWithEntropy(entropy: Uint8Array | Buffer, passphrase: string): HDWallet;
    seed(): Uint8Array;
    mnemonic(): string;
    entropy(): Uint8Array;
    getMasterKey(curve: Curve): PrivateKey;
    getKeyForCoin(coin: CoinType): PrivateKey;
    getAddressForCoin(coin: CoinType): string;
    getKey(coin: CoinType, derivationPath: string): PrivateKey;
    getDerivedKey(coin: CoinType, account: number, change: number, address: number): PrivateKey;
    getExtendedPrivateKey(purpose: Purpose, coin: CoinType, version: HDVersion): string;
    getExtendedPublicKey(purpose: Purpose, coin: CoinType, version: HDVersion): string;
    getExtendedPrivateKeyAccount(purpose: Purpose, coin: CoinType, derivation: Derivation, version: HDVersion, account: number): string;
    getExtendedPublicKeyAccount(purpose: Purpose, coin: CoinType, derivation: Derivation, version: HDVersion, account: number): string;
    getExtendedPrivateKeyDerivation(purpose: Purpose, coin: CoinType, derivation: Derivation, version: HDVersion): string;
    getExtendedPublicKeyDerivation(purpose: Purpose, coin: CoinType, derivation: Derivation, version: HDVersion): string;
    delete(): void;
}
export class StoredKeyEncryptionLevel {
    value: number;
    static default: StoredKeyEncryptionLevel;
    static minimal: StoredKeyEncryptionLevel;
    static weak: StoredKeyEncryptionLevel;
    static standard: StoredKeyEncryptionLevel;
}
export class SS58AddressType {
    value: number;
    static polkadot: SS58AddressType;
    static kusama: SS58AddressType;
}
export class StellarPassphrase {
    value: number;
    static stellar: StellarPassphrase;
    static kin: StellarPassphrase;
}

declare function describeStellarPassphrase(value: StellarPassphrase): string;

export class Account {
    static create(address: string, coin: CoinType, derivation: Derivation, derivationPath: string, publicKey: string, extendedPublicKey: string): Account;
    address(): string;
    derivation(): Derivation;
    derivationPath(): string;
    publicKey(): string;
    extendedPublicKey(): string;
    coin(): CoinType;
    delete(): void;
}
export class CoinType {
    value: number;
    static aeternity: CoinType;
    static aion: CoinType;
    static binance: CoinType;
    static bitcoin: CoinType;
    static bitcoinCash: CoinType;
    static bitcoinGold: CoinType;
    static callisto: CoinType;
    static cardano: CoinType;
    static cosmos: CoinType;
    static dash: CoinType;
    static decred: CoinType;
    static digiByte: CoinType;
    static dogecoin: CoinType;
    static eos: CoinType;
    static ethereum: CoinType;
    static ethereumClassic: CoinType;
    static fio: CoinType;
    static goChain: CoinType;
    static groestlcoin: CoinType;
    static icon: CoinType;
    static ioTeX: CoinType;
    static kava: CoinType;
    static kin: CoinType;
    static litecoin: CoinType;
    static monacoin: CoinType;
    static nebulas: CoinType;
    static nuls: CoinType;
    static nano: CoinType;
    static near: CoinType;
    static nimiq: CoinType;
    static ontology: CoinType;
    static poanetwork: CoinType;
    static qtum: CoinType;
    static xrp: CoinType;
    static solana: CoinType;
    static stellar: CoinType;
    static tezos: CoinType;
    static theta: CoinType;
    static thunderToken: CoinType;
    static neo: CoinType;
    static tomoChain: CoinType;
    static tron: CoinType;
    static veChain: CoinType;
    static viacoin: CoinType;
    static wanchain: CoinType;
    static zcash: CoinType;
    static firo: CoinType;
    static zilliqa: CoinType;
    static zelcash: CoinType;
    static ravencoin: CoinType;
    static waves: CoinType;
    static terra: CoinType;
    static terraV2: CoinType;
    static harmony: CoinType;
    static algorand: CoinType;
    static kusama: CoinType;
    static polkadot: CoinType;
    static filecoin: CoinType;
    static elrond: CoinType;
    static bandChain: CoinType;
    static smartChainLegacy: CoinType;
    static smartChain: CoinType;
    static oasis: CoinType;
    static polygon: CoinType;
    static thorchain: CoinType;
    static bluzelle: CoinType;
    static optimism: CoinType;
    static arbitrum: CoinType;
    static ecochain: CoinType;
    static avalancheCChain: CoinType;
    static xdai: CoinType;
    static fantom: CoinType;
    static cryptoOrg: CoinType;
    static celo: CoinType;
    static ronin: CoinType;
    static osmosis: CoinType;
    static ecash: CoinType;
    static cronosChain: CoinType;
    static smartBitcoinCash: CoinType;
    static kuCoinCommunityChain: CoinType;
    static boba: CoinType;
    static metis: CoinType;
    static aurora: CoinType;
    static evmos: CoinType;
    static nativeEvmos: CoinType;
    static moonriver: CoinType;
    static moonbeam: CoinType;
    static kavaEvm: CoinType;
    static klaytn: CoinType;
    static meter: CoinType;
    static okxchain: CoinType;
}
export class StoredKey {
    static load(path: string): StoredKey;
    static importPrivateKey(privateKey: Uint8Array | Buffer, name: string, password: Uint8Array | Buffer, coin: CoinType): StoredKey;
    static importHDWallet(mnemonic: string, name: string, password: Uint8Array | Buffer, coin: CoinType): StoredKey;
    static importJSON(json: Uint8Array | Buffer): StoredKey;
    static createLevel(name: string, password: Uint8Array | Buffer, encryptionLevel: StoredKeyEncryptionLevel): StoredKey;
    static create(name: string, password: Uint8Array | Buffer): StoredKey;
    identifier(): string;
    name(): string;
    isMnemonic(): boolean;
    accountCount(): number;
    encryptionParameters(): string;
    account(index: number): Account;
    accountForCoin(coin: CoinType, wallet: HDWallet): Account;
    accountForCoinDerivation(coin: CoinType, derivation: Derivation, wallet: HDWallet): Account;
    addAccountDerivation(address: string, coin: CoinType, derivation: Derivation, derivationPath: string, publicKey: string, extendedPublicKey: string): void;
    addAccount(address: string, coin: CoinType, derivationPath: string, publicKey: string, extendedPublicKey: string): void;
    removeAccountForCoin(coin: CoinType): void;
    removeAccountForCoinDerivation(coin: CoinType, derivation: Derivation): void;
    removeAccountForCoinDerivationPath(coin: CoinType, derivationPath: string): void;
    store(path: string): boolean;
    decryptPrivateKey(password: Uint8Array | Buffer): Uint8Array;
    decryptMnemonic(password: Uint8Array | Buffer): string;
    privateKey(coin: CoinType, password: Uint8Array | Buffer): PrivateKey;
    wallet(password: Uint8Array | Buffer): HDWallet;
    exportJSON(): Uint8Array;
    fixAddresses(password: Uint8Array | Buffer): boolean;
    delete(): void;
}
export class EthereumChainID {
    value: number;
    static ethereum: EthereumChainID;
    static classic: EthereumChainID;
    static poa: EthereumChainID;
    static vechain: EthereumChainID;
    static callisto: EthereumChainID;
    static tomochain: EthereumChainID;
    static polygon: EthereumChainID;
    static okc: EthereumChainID;
    static thundertoken: EthereumChainID;
    static gochain: EthereumChainID;
    static meter: EthereumChainID;
    static celo: EthereumChainID;
    static wanchain: EthereumChainID;
    static cronos: EthereumChainID;
    static optimism: EthereumChainID;
    static xdai: EthereumChainID;
    static smartbch: EthereumChainID;
    static fantom: EthereumChainID;
    static boba: EthereumChainID;
    static kcc: EthereumChainID;
    static heco: EthereumChainID;
    static metis: EthereumChainID;
    static moonbeam: EthereumChainID;
    static moonriver: EthereumChainID;
    static ronin: EthereumChainID;
    static kavaevm: EthereumChainID;
    static klaytn: EthereumChainID;
    static avalanchec: EthereumChainID;
    static evmos: EthereumChainID;
    static arbitrum: EthereumChainID;
    static smartchain: EthereumChainID;
    static aurora: EthereumChainID;
}
export class CoinTypeConfiguration {
    static getSymbol(type: CoinType): string;
    static getDecimals(type: CoinType): number;
    static getTransactionURL(type: CoinType, transactionID: string): string;
    static getAccountURL(type: CoinType, accountID: string): string;
    static getID(type: CoinType): string;
    static getName(type: CoinType): string;
}
export class THORChainSwap {
    static buildSwap(input: Uint8Array | Buffer): Uint8Array;
}
export class PrivateKey {
    static isValid(data: Uint8Array | Buffer, curve: Curve): boolean;
    static create(): PrivateKey;
    static createWithData(data: Uint8Array | Buffer): PrivateKey;
    static createCopy(key: PrivateKey): PrivateKey;
    data(): Uint8Array;
    getPublicKeySecp256k1(compressed: boolean): PublicKey;
    getPublicKeyNist256p1(): PublicKey;
    getPublicKeyEd25519(): PublicKey;
    getPublicKeyEd25519Blake2b(): PublicKey;
    getPublicKeyEd25519Cardano(): PublicKey;
    getPublicKeyCurve25519(): PublicKey;
    getSharedKey(publicKey: PublicKey, curve: Curve): Uint8Array;
    sign(digest: Uint8Array | Buffer, curve: Curve): Uint8Array;
    signAsDER(digest: Uint8Array | Buffer): Uint8Array;
    signZilliqaSchnorr(message: Uint8Array | Buffer): Uint8Array;
    delete(): void;
}
export class Base58 {
    static encode(data: Uint8Array | Buffer): string;
    static encodeNoCheck(data: Uint8Array | Buffer): string;
    static decode(string: string): Uint8Array;
    static decodeNoCheck(string: string): Uint8Array;
}
export class Derivation {
    value: number;
    static default: Derivation;
    static custom: Derivation;
    static bitcoinSegwit: Derivation;
    static bitcoinLegacy: Derivation;
    static litecoinLegacy: Derivation;
    static solanaSolana: Derivation;
}
export class GroestlcoinAddress {
    static equal(lhs: GroestlcoinAddress, rhs: GroestlcoinAddress): boolean;
    static isValidString(string: string): boolean;
    static createWithString(string: string): GroestlcoinAddress;
    static createWithPublicKey(publicKey: PublicKey, prefix: number): GroestlcoinAddress;
    description(): string;
    delete(): void;
}
export class Hash {
    static sha1(data: Uint8Array | Buffer): Uint8Array;
    static sha256(data: Uint8Array | Buffer): Uint8Array;
    static sha512(data: Uint8Array | Buffer): Uint8Array;
    static sha512_256(data: Uint8Array | Buffer): Uint8Array;
    static keccak256(data: Uint8Array | Buffer): Uint8Array;
    static keccak512(data: Uint8Array | Buffer): Uint8Array;
    static sha3_256(data: Uint8Array | Buffer): Uint8Array;
    static sha3_512(data: Uint8Array | Buffer): Uint8Array;
    static ripemd(data: Uint8Array | Buffer): Uint8Array;
    static blake256(data: Uint8Array | Buffer): Uint8Array;
    static blake2b(data: Uint8Array | Buffer, size: number): Uint8Array;
    static groestl512(data: Uint8Array | Buffer): Uint8Array;
    static sha256SHA256(data: Uint8Array | Buffer): Uint8Array;
    static sha256RIPEMD(data: Uint8Array | Buffer): Uint8Array;
    static sha3_256RIPEMD(data: Uint8Array | Buffer): Uint8Array;
    static blake256Blake256(data: Uint8Array | Buffer): Uint8Array;
    static blake256RIPEMD(data: Uint8Array | Buffer): Uint8Array;
    static groestl512Groestl512(data: Uint8Array | Buffer): Uint8Array;
}
export class PrivateKeyType {
    value: number;
    static default: PrivateKeyType;
    static cardano: PrivateKeyType;
}
export class SolanaAddress {
    static createWithString(string: string): SolanaAddress;
    description(): string;
    defaultTokenAddress(tokenMintAddress: string): string;
    delete(): void;
}
export class PBKDF2 {
    static hmacSha256(password: Uint8Array | Buffer, salt: Uint8Array | Buffer, iterations: number, dkLen: number): Uint8Array;
    static hmacSha512(password: Uint8Array | Buffer, salt: Uint8Array | Buffer, iterations: number, dkLen: number): Uint8Array;
}
export class RippleXAddress {
    static equal(lhs: RippleXAddress, rhs: RippleXAddress): boolean;
    static isValidString(string: string): boolean;
    static createWithString(string: string): RippleXAddress;
    static createWithPublicKey(publicKey: PublicKey, tag: number): RippleXAddress;
    description(): string;
    tag(): number;
    delete(): void;
}
export class DataVector {
    static create(): DataVector;
    static createWithData(data: Uint8Array | Buffer): DataVector;
    size(): number;
    add(data: Uint8Array | Buffer): void;
    get(index: number): Uint8Array;
    delete(): void;
}
export class Base64 {
    static decode(string: string): Uint8Array;
    static decodeUrl(string: string): Uint8Array;
    static encode(data: Uint8Array | Buffer): string;
    static encodeUrl(data: Uint8Array | Buffer): string;
}
export class AESPaddingMode {
    value: number;
    static zero: AESPaddingMode;
    static pkcs7: AESPaddingMode;
}
export class HRP {
    value: number;
    static unknown: HRP;
    static bitcoin: HRP;
    static litecoin: HRP;
    static viacoin: HRP;
    static groestlcoin: HRP;
    static digiByte: HRP;
    static monacoin: HRP;
    static cosmos: HRP;
    static bitcoinCash: HRP;
    static bitcoinGold: HRP;
    static ioTeX: HRP;
    static zilliqa: HRP;
    static terra: HRP;
    static cryptoOrg: HRP;
    static kava: HRP;
    static oasis: HRP;
    static bluzelle: HRP;
    static bandChain: HRP;
    static elrond: HRP;
    static binance: HRP;
    static ecash: HRP;
    static thorchain: HRP;
    static harmony: HRP;
    static cardano: HRP;
    static qtum: HRP;
    static osmosis: HRP;
    static terraV2: HRP;
    static nativeEvmos: HRP;
}

declare function describeHRP(value: HRP): string;

export class Base32 {
    static decodeWithAlphabet(string: string, alphabet: string): Uint8Array;
    static decode(string: string): Uint8Array;
    static encodeWithAlphabet(data: Uint8Array | Buffer, alphabet: string): string;
    static encode(data: Uint8Array | Buffer): string;
}
export class EthereumAbi {
    static encode(fn: EthereumAbiFunction): Uint8Array;
    static decodeOutput(fn: EthereumAbiFunction, encoded: Uint8Array | Buffer): boolean;
    static decodeCall(data: Uint8Array | Buffer, abi: string): string;
    static encodeTyped(messageJson: string): Uint8Array;
}
export class BitcoinScript {
    static equal(lhs: BitcoinScript, rhs: BitcoinScript): boolean;
    static buildPayToPublicKey(pubkey: Uint8Array | Buffer): BitcoinScript;
    static buildPayToPublicKeyHash(hash: Uint8Array | Buffer): BitcoinScript;
    static buildPayToScriptHash(scriptHash: Uint8Array | Buffer): BitcoinScript;
    static buildPayToWitnessPubkeyHash(hash: Uint8Array | Buffer): BitcoinScript;
    static buildPayToWitnessScriptHash(scriptHash: Uint8Array | Buffer): BitcoinScript;
    static lockScriptForAddress(address: string, coin: CoinType): BitcoinScript;
    static hashTypeForCoin(coinType: CoinType): number;
    static create(): BitcoinScript;
    static createWithData(data: Uint8Array | Buffer): BitcoinScript;
    static createCopy(script: BitcoinScript): BitcoinScript;
    size(): number;
    data(): Uint8Array;
    scriptHash(): Uint8Array;
    isPayToScriptHash(): boolean;
    isPayToWitnessScriptHash(): boolean;
    isPayToWitnessPublicKeyHash(): boolean;
    isWitnessProgram(): boolean;
    matchPayToPubkey(): Uint8Array;
    matchPayToPubkeyHash(): Uint8Array;
    matchPayToScriptHash(): Uint8Array;
    matchPayToWitnessPublicKeyHash(): Uint8Array;
    matchPayToWitnessScriptHash(): Uint8Array;
    encode(): Uint8Array;
    delete(): void;
}

export interface WalletCore {
    AES: typeof AES;
    AESPaddingMode: typeof AESPaddingMode;
    Base32: typeof Base32;
    Base64: typeof Base64;
    Curve: typeof Curve;
    CoinType: typeof CoinType;
    Mnemonic: typeof Mnemonic;
    HexCoding: typeof HexCoding;
    AnyAddress: typeof AnyAddress;
    AnySigner: typeof AnySigner;
    PublicKey: typeof PublicKey;
    PrivateKey: typeof PrivateKey;
    HDWallet: typeof HDWallet;
    Hash: typeof Hash;
    HRP: typeof HRP;
    describeHRP: typeof describeHRP;
    PBKDF2: typeof PBKDF2;
    StoredKey: typeof StoredKey;
    EthereumAbi: typeof EthereumAbi;
}
