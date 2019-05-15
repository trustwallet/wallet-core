export const bufToHex = (buffer: ArrayBuffer, prefix: boolean = true) => {
    if (prefix) {
        return `0x${Array.prototype.map.call(new Uint8Array(buffer), (x: any) => (`00${x.toString(16)}`).slice(-2)).join('')}`;
    }
    return `${Array.prototype.map.call(new Uint8Array(buffer), (x: any) => (`00${x.toString(16)}`).slice(-2)).join('')}`;
};

export const fromHexString = (hexString: string) => {
    // Clean hex
    const cleanHexString = hexString.substr(0, 2) === '0x' ? hexString.substr(2) : hexString;
    // Ensure even padding
    const paddedHexString = cleanHexString.length % 2 === 0 ? cleanHexString : `0${cleanHexString}`;

    const match = paddedHexString.match(/.{1,2}/g);
    if (match !== null) {
        return new Uint8Array(match.map(byte => parseInt(byte, 16)));
    }
    return new Uint8Array(0);
};
