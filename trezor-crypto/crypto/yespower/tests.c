/*-
 * Copyright 2013-2018 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>

#include "yespower.h"

#undef TEST_PBKDF2_SHA256

#ifdef TEST_PBKDF2_SHA256
#include <assert.h>

#include "sha256.h"

static void print_PBKDF2_SHA256_raw(const char *passwd, size_t passwdlen,
    const char *salt, size_t saltlen, uint64_t c, size_t dkLen)
{
	uint8_t dk[64];
	int i;

	assert(dkLen <= sizeof(dk));

	/* XXX This prints the strings truncated at first NUL */
	printf("PBKDF2_SHA256(\"%s\", \"%s\", %llu, %llu) = ",
	    passwd, salt, (unsigned long long)c, (unsigned long long)dkLen);

	PBKDF2_SHA256((const uint8_t *) passwd, passwdlen,
	    (const uint8_t *) salt, saltlen, c, dk, dkLen);

	for (i = 0; i < dkLen; i++)
		printf("%02x%c", dk[i], i < dkLen - 1 ? ' ' : '\n');
}

static void print_PBKDF2_SHA256(const char *passwd,
    const char *salt, uint64_t c, size_t dkLen)
{
	print_PBKDF2_SHA256_raw(passwd, strlen(passwd), salt, strlen(salt), c,
	    dkLen);
}
#endif

static const char *pers_bsty_magic = "BSTY";

static void print_yespower(yespower_version_t version, uint32_t N, uint32_t r,
    const char *pers)
{
	yespower_params_t params = {
		.version = version,
		.N = N,
		.r = r,
		.pers = (const uint8_t *)pers,
		.perslen = pers ? strlen(pers) : 0
	};
	uint8_t src[80];
	yespower_binary_t dst;
	int i;

	const char *q = (pers && pers != pers_bsty_magic) ? "\"": "";
	printf("yespower(%u, %u, %u, %s%s%s) = ", version, N, r,
	    q, pers ? pers : "NULL", q);

	for (i = 0; i < sizeof(src); i++)
		src[i] = i * 3;

	if (pers == pers_bsty_magic) {
		params.pers = src;
		params.perslen = sizeof(src);
	}

	if (yespower_tls(src, sizeof(src), &params, &dst)) {
		puts("FAILED");
		return;
	}

	for (i = 0; i < sizeof(dst); i++)
		printf("%02x%c", dst.uc[i], i < sizeof(dst) - 1 ? ' ' : '\n');
}

int main(int argc, const char * const *argv)
{
	setvbuf(stdout, NULL, _IOLBF, 0);

#ifdef TEST_PBKDF2_SHA256
	print_PBKDF2_SHA256("password", "salt", 1, 20);
	print_PBKDF2_SHA256("password", "salt", 2, 20);
	print_PBKDF2_SHA256("password", "salt", 4096, 20);
	print_PBKDF2_SHA256("password", "salt", 16777216, 20);
	print_PBKDF2_SHA256("passwordPASSWORDpassword",
	    "saltSALTsaltSALTsaltSALTsaltSALTsalt", 4096, 25);
	print_PBKDF2_SHA256_raw("pass\0word", 9, "sa\0lt", 5, 4096, 16);
#if 0
	print_PBKDF2_SHA256("password", "salt", 1, 32);
	print_PBKDF2_SHA256("password", "salt", 2, 32);
	print_PBKDF2_SHA256("password", "salt", 4096, 32);
	print_PBKDF2_SHA256("password", "salt", 16777216, 32);
	print_PBKDF2_SHA256("passwordPASSWORDpassword",
	    "saltSALTsaltSALTsaltSALTsaltSALTsalt", 4096, 40);
	print_PBKDF2_SHA256("password", "salt", 4096, 16);
	print_PBKDF2_SHA256("password", "salt", 1, 20);
	print_PBKDF2_SHA256("password", "salt", 2, 20);
	print_PBKDF2_SHA256("password", "salt", 4096, 20);
	print_PBKDF2_SHA256("password", "salt", 16777216, 20);
	print_PBKDF2_SHA256("password", "salt", 4096, 25);
	print_PBKDF2_SHA256("password", "salt", 4096, 16);
#endif
#endif

	print_yespower(YESPOWER_0_5, 2048, 8, "Client Key"); /* yescrypt 0.5 */
	print_yespower(YESPOWER_0_5, 2048, 8, pers_bsty_magic); /* BSTY */
	print_yespower(YESPOWER_0_5, 4096, 16, "Client Key"); /* Cryply */
	print_yespower(YESPOWER_0_5, 4096, 24, "Jagaricoin");
	print_yespower(YESPOWER_0_5, 4096, 32, "WaviBanana");
	print_yespower(YESPOWER_0_5, 2048, 32, "Client Key");
	print_yespower(YESPOWER_0_5, 1024, 32, "Client Key");

	print_yespower(YESPOWER_0_5, 2048, 8, NULL); /* no personality */

	print_yespower(YESPOWER_0_9, 2048, 8, NULL);
	print_yespower(YESPOWER_0_9, 4096, 16, NULL);
	print_yespower(YESPOWER_0_9, 4096, 32, NULL);
	print_yespower(YESPOWER_0_9, 2048, 32, NULL);
	print_yespower(YESPOWER_0_9, 1024, 32, NULL);

	print_yespower(YESPOWER_0_9, 1024, 32, "personality test");

	return 0;
}
