export class CoinTypeExtension {
    constructor(CoinType)
    value(): CoinType
    blockchain(): Blockchain
    purpose(): Purpose
    curve(): Curve
    xpubVersion(): HDVersion
    xprvVersion(): HDVersion
    validate(address: string): boolean
    derivationPath(): string
    derivationPathWithDerivation(derivation: Derivation): string
    deriveAddress(privateKey: PrivateKey): string
    deriveAddressFromPublicKey(publicKey: PublicKey): string
    hrp(): HRP
    P2pkhPrefix(): number
    P2shPrefix(): number
}
