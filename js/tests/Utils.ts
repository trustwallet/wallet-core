export const bufToHex = (buffer: ArrayBuffer) => {
    return `0x${Array.prototype.map.call(new Uint8Array(buffer), (x: any) => (`00${x.toString(16)}`).slice(-2)).join('')}`;
};

export const fromHexString = (hexString: string) => {
    const newHexString = hexString.length % 2 === 0 ? hexString : `0${hexString}`;

    const match = newHexString.match(/.{1,2}/g);
    if (match !== null) {
        return new Uint8Array(match.map(byte => parseInt(byte, 16)));
    }
    return new Uint8Array();
};
