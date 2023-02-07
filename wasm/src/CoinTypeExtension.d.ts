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
}
