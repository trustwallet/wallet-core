// SPDX-License-Identifier: Apache-2.0
// Check chunking and fatal failures without replacing the production RNG.
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <bcrypt.h>

static int fail_random;
static unsigned calls;
static ULONG last_count;

static NTSTATUS test_random(BCRYPT_ALG_HANDLE algorithm, PUCHAR buffer, ULONG count, ULONG flags) {
    assert(algorithm == NULL && flags == BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    ++calls;
    last_count = count;
    if (fail_random)
        return (NTSTATUS)0xc0000001;
    memset(buffer, 0xa5, count);
    return 0;
}

// Simulate the ULONG boundary without allocating more than 4 GiB.
#undef ULONG_MAX
#define ULONG_MAX 7UL
#define BCryptGenRandom test_random
#include "../../trezor-crypto/crypto/rand.c"

int main(int argc, char** argv) {
    (void)argv;
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    if (argc > 1) {
        fail_random = 1;
        random32();
        return 0; // Must be unreachable: secure random failures abort.
    }
    random_buffer(NULL, 0);
    assert(calls == 0);
    uint8_t buffer[19] = {0};
    random_buffer(buffer + 1, 17);
    assert(calls == 3 && last_count == 3);
    assert(buffer[0] == 0 && buffer[18] == 0);
    for (unsigned i = 1; i < 18; ++i)
        assert(buffer[i] == 0xa5);
    assert(random32() == 0xa5a5a5a5 && calls == 4);
    return 0;
}
