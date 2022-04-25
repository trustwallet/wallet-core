# Sample Wasm

## DISCLAIMER

> This is a sample application with demonstration purpose only,
> do not use it with real addresses, real transactions, or real funds.
> Use it at your own risk.

## Prerequisites

1. Everything you need to build the wallet core: https://developer.trustwallet.com/wallet-core/developing-the-library/building
2. Install [emsdk](https://emscripten.org/docs/getting_started/downloads.html)
- - python3
- - cmake
- - run `tools/install-wasm-dependencies`

## Building and Running

- run `tools/generate-files`
- run `source emsdk/emsdk_env.sh`
- run  `tools/wasm-build`
- run `python3 -m http.server 8000`
- open web browser and navigate to `http://127.0.0.1:8000/samples/wasm/wallet-core.html`
