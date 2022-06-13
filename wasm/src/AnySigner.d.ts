export class AnySigner {
    static sign(data: Uint8Array | Buffer, coin: CoinType): Uint8Array;
    static plan(data: Uint8Array | Buffer, coin: CoinType): Uint8Array;
    static supportsJSON(coin: CoinType): boolean;
}
