// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
