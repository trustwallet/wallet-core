export namespace HexCoding {
    export function decode(hex: string): Uint8Array;
    export function encode(buffer: Uint8Array | Buffer): string;
}
