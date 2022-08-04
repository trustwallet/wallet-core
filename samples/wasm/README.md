# Wasm Sample

## DISCLAIMER

> This is a sample html for demonstration purpose only,
> do not use it with real addresses, real transactions, or real funds.
> Use it at your own risk.

## Prerequisites

1. Everything you need to build the wallet core: https://developer.trustwallet.com/wallet-core/developing-the-library/building
2. Install [emsdk](https://emscripten.org/docs/getting_started/downloads.html)
- - python3
- - cmake
- - run `tools/install-dependencies` if you just cloned this repo
- - run `tools/install-wasm-dependencies`
3. node.js

## Building and Running

- run `tools/generate-files`
- run  `tools/wasm-build`
- cd `wasm`
- - run `npm install`
- - run `npm run codegen:js-browser`
- - run `npm run copy:wasm-sample`
- cd `samples/wasm`
- run `python3 -m http.server 8000`
- open web browser and navigate to `http://127.0.0.1:8000`

## Notes

This sample also demonstrates how to use protobuf.js models in html directly, `core_proto.js` is built by command defined in `wasm/package.json`

```shell
npm run codegen:js-browser
```

For modern javaScript sample, please check out:

-  React: https://github.com/hewigovens/wallet-core-react
-  Flutter: https://github.com/iampawan/Wallet-Core-Web/tree/dev
