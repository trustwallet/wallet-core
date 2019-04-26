# Trust Wallet Core

[![npm](https://img.shields.io/badge/npm-v0.1.0-blue.svg?style=flat-square&logo=npm)](https://www.npmjs.com/package/trust-wallet-core)

`trust-wallet-core` is a JavaScript wrapper around the [wallet-core](https://github.com/TrustWallet/wallet-core) library that is a standalone `npm` package.

Type definitions for TypeScript are included.

## Install

```
$ npm install trust-wallet-core
```

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