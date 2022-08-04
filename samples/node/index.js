#!/usr/bin/env node

const { initWasm, TW } = require('@trustwallet/wallet-core');

(async function() {
    const start = new Date().getTime();
    console.log(`Initializing...`);
    const { CoinType, HexCoding } = await initWasm();
    console.log(`Done in ${new Date().getTime() - start} ms`);
    console.log(HexCoding.decode("0xce2fd7544e0b2cc94692d4a704debef7bcb61328"));
    console.log(CoinType.ethereum.value);
    console.log(TW.Ethereum);
})();
