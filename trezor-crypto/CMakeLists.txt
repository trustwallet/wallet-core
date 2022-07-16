# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

set(TW_WARNING_FLAGS 
    -W
    -Wall
    -Wextra
    -Wimplicit-function-declaration
    -Wredundant-decls
    -Wstrict-prototypes
    -Wundef
    -Wshadow
    -Wpointer-arith
    -Wformat
    -Wreturn-type
    -Wsign-compare
    -Wmultichar
    -Wformat-nonliteral
    -Winit-self
    -Wuninitialized
    -Wformat-security
    -Wno-missing-braces
)

add_library(TrezorCrypto
    crypto/bignum.c crypto/ecdsa.c crypto/curves.c crypto/secp256k1.c crypto/rand.c crypto/hmac.c crypto/bip32.c crypto/bip39.c crypto/slip39.c crypto/pbkdf2.c crypto/base58.c crypto/base32.c
    crypto/address.c
    crypto/script.c
    crypto/ripemd160.c
    crypto/sha2.c
    crypto/sha3.c
    crypto/hasher.c
    crypto/aes/aescrypt.c crypto/aes/aeskey.c crypto/aes/aestab.c crypto/aes/aes_modes.c
    crypto/ed25519-donna/curve25519-donna-32bit.c crypto/ed25519-donna/curve25519-donna-helpers.c crypto/ed25519-donna/modm-donna-32bit.c
    crypto/ed25519-donna/ed25519-donna-basepoint-table.c crypto/ed25519-donna/ed25519-donna-32bit-tables.c crypto/ed25519-donna/ed25519-donna-impl-base.c
    crypto/ed25519-donna/ed25519.c crypto/ed25519-donna/curve25519-donna-scalarmult-base.c crypto/ed25519-donna/ed25519-sha3.c crypto/ed25519-donna/ed25519-keccak.c crypto/ed25519-donna/ed25519-blake2b.c
    crypto/sodium/private/fe_25_5/fe.c crypto/sodium/private/ed25519_ref10.c crypto/sodium/private/ed25519_ref10_fe_25_5.c crypto/sodium/keypair.c
    crypto/monero/base58.c
    crypto/monero/serialize.c
    #crypto/monero/xmr.c
    crypto/monero/range_proof.c
    crypto/blake256.c
    crypto/blake2b.c crypto/blake2s.c
    crypto/chacha_drbg.c
    crypto/chacha20poly1305/chacha20poly1305.c crypto/chacha20poly1305/chacha_merged.c crypto/chacha20poly1305/poly1305-donna.c crypto/chacha20poly1305/rfc7539.c
    crypto/rc4.c
    crypto/nano.c
    crypto/nem.c
    crypto/cash_addr.c
    crypto/memzero.c
    crypto/scrypt.c
    crypto/nist256p1.c
    crypto/groestl.c
    crypto/hmac_drbg.c
    crypto/rfc6979.c
    crypto/shamir.c
    crypto/zilliqa.c
    crypto/cardano.c
)

if (EMSCRIPTEN)
    message(STATUS "Skip building trezor-crypto/tests")
    set(TW_WARNING_FLAGS ${TW_WARNING_FLAGS} -Wno-bitwise-instead-of-logical)
else ()
    if(NOT ANDROID AND NOT IOS_PLATFORM)
        add_subdirectory(crypto/tests)
    endif()
endif()

target_compile_options(TrezorCrypto PRIVATE ${TW_WARNING_FLAGS} -Werror PUBLIC -Wno-deprecated-volatile)

target_include_directories(TrezorCrypto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)

install(
    TARGETS TrezorCrypto
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
