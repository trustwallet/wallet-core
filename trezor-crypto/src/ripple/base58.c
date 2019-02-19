/**
 * Copyright (c) 2012-2014 Luke Dashjr
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

#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ripple/base58.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/memzero.h>

const char xrp_b58digits[] = "rpshnaf39wBUDNEGHJKLM4PQRST7VWXYZ2bcdeCg65jkm8oFqi1tuvAxyz";
const int8_t xrp_b58digits_map[] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,50,33,7,21,41,40,27,45,8,-1,-1,-1,-1,-1,-1,
    -1,54,10,38,12,14,47,15,16,-1,17,18,19,20,13,-1,
    22,23,24,25,26,11,28,29,30,31,32,-1,-1,-1,-1,-1,
    -1,5,34,35,36,37,6,39,3,49,42,43,-1,44,4,46,
    1,48,0,2,51,52,53,9,55,56,57,-1,-1,-1,-1,-1,
};

int xrp_base58_encode_check(const uint8_t *data, int datalen, HasherType hasher_type, char *str, int strsize)
{
	if (datalen > 128) {
		return 0;
	}
	uint8_t buf[datalen + 32];
	uint8_t *hash = buf + datalen;
	memcpy(buf, data, datalen);
	hasher_Raw(hasher_type, data, datalen, hash);
	size_t res = strsize;
	bool success = base58_encode(str, &res, buf, datalen + 4, xrp_b58digits);
	memzero(buf, sizeof(buf));
	return success ? res : 0;
}

int xrp_base58_decode_check(const char *str, HasherType hasher_type, uint8_t *data, int datalen)
{
    if (datalen > 128) {
		return 0;
	}
	uint8_t d[datalen + 4];
	size_t res = datalen + 4;
	if (base58_to_bin(d, &res, str, xrp_b58digits, xrp_b58digits_map) != true) {
		return 0;
	}
	uint8_t *nd = d + datalen + 4 - res;
	if (base58_check(nd, res, hasher_type, str, xrp_b58digits) < 0) {
		return 0;
	}
	memcpy(data, nd, res - 4);
	return res - 4;
}
