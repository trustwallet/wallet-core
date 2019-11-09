/**
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

// implement BIP32 caching
#ifndef USE_BIP32_CACHE
#define USE_BIP32_CACHE 0
#define BIP32_CACHE_SIZE 10
#define BIP32_CACHE_MAXDEPTH 8
#endif

// support constructing BIP32 nodes from ed25519 and curve25519 curves.
#ifndef USE_BIP32_25519_CURVES
#define USE_BIP32_25519_CURVES    1
#endif

// add way how to mark confidential data
#ifndef CONFIDENTIAL
#define CONFIDENTIAL
#endif

#endif
