// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#ifndef __USE_WASM
#define __USE_WASM
#endif // __USE_WASM

#ifndef __USE_MISC
#ifdef __USE_WASM

typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;

#endif // __USE_WASM
#endif // __USE_MISC
