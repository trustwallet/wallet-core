# Devconsole.ts

`Devconsole.ts` is a command-line utility to quickly work with `wallet-core` library.
It is based on `node` `typescript` console, and uses the WASM version of `wallet-core`.

Type `help()` after starting it.

Some wallet-core namespaces are exposed, and methods can be called directly.

Note that auto-completion works in the console.

## Build:

```
cd samples/typescript/devconsole.ts
npm install
npm run build
npm run start
```

## Sample usage:

```
> help()
This is an interactive typescript shell, to work with wallet-core (wasm)
You can use:
...
> w1 = HDWallet.create(256, '')
W {}
> w1.mnemonic()
'round profit immense ... sniff'
> w1.getAddressForCoin(CoinType.cosmos)
'cosmos1kgd25tvkz3jtuee4cl796trc2s0hjsfgqmhmku'
```
