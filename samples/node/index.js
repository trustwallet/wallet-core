#!/usr/bin/env node

const { TW, WalletCore } = require('@trustwallet/wallet-core');

const sleep = async (milliseconds) => {
    await new Promise(resolve => setTimeout(resolve, milliseconds));
}

(async function() {
    await sleep(1000);
    const { CoinType, HexCoding } = WalletCore;

    console.log(HexCoding.decode("0xce2fd7544e0b2cc94692d4a704debef7bcb61328"));
    console.log(CoinType.ethereum.value);
    console.log(TW);
})();

