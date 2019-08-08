# Trust Wallet Core

[![npm](https://img.shields.io/badge/npm-v0.4.0-blue.svg?style=flat-square&logo=npm)](https://www.npmjs.com/package/trust-wallet-core)

`trust-wallet-core` is a JavaScript wrapper around the [wallet-core](https://github.com/TrustWallet/wallet-core) library that is a standalone `npm` package.

Type definitions for TypeScript are included.

## Install

    $ npm install trust-wallet-core

## Usage

```js
const WalletCore = require("trust-wallet-core");

WalletCore.HDWallet.create(128);
//=> "army cook device assault vendor case coast rose dismiss lady dust point!"
```

```js
from { HDWallet } from 'trust-wallet-core';

HDWallet.create(128);
//=> "army cook device assault vendor case coast rose dismiss lady dust point!"
```

## Documentation

### Platforms

`trust-wallet-core` only supports `macOS/OSX` at the moment. `Linux` and `Windows` support is forthcoming.

### Bindings

-   This project uses [`node-addon-api`](https://github.com/nodejs/node-addon-api) to interface with `wallet-core` [C headers]((https://github.com/TrustWallet/wallet-core/tree/master/include/TrustWalletCore)). 
-   The `node-addon-api` functions are wrapped in TypeScript that is then used to generate a JavaScript module with type definitions.

### Memory management

-   Please note that memory allocated with C/C++ is released once there are no references to the JavaScript object, then Node.js garbage collector will try to release it from memory.

## Other Resources

-   For general [`TrustWallet/wallet-core`](https://github.com/TrustWallet/wallet-core) documentation, please refer to [GitBooks](https://developer.trustwallet.com/wallet-core).

-   For documentation on specific modules that are available, please refer to [`TrustWallet/wallet-core`](https://github.com/TrustWallet/wallet-core) [exposed C headers](https://github.com/TrustWallet/wallet-core/tree/master/include/TrustWalletCore).
