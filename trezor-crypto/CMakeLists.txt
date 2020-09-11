add_library(TrezorCrypto
    src/bignum.c src/ecdsa.c src/curves.c src/secp256k1.c src/rand.c src/hmac.c src/bip32.c src/bip39.c src/pbkdf2.c src/base58.c src/base32.c
    src/address.c
    src/script.c
    src/ripemd160.c
    src/sha2.c
    src/sha3.c
    src/hasher.c
    src/aes/aescrypt.c src/aes/aeskey.c src/aes/aestab.c src/aes/aes_modes.c
    src/ed25519-donna/curve25519-donna-32bit.c src/ed25519-donna/curve25519-donna-helpers.c src/ed25519-donna/modm-donna-32bit.c
    src/ed25519-donna/ed25519-donna-basepoint-table.c src/ed25519-donna/ed25519-donna-32bit-tables.c src/ed25519-donna/ed25519-donna-impl-base.c
    src/ed25519-donna/ed25519.c src/ed25519-donna/curve25519-donna-scalarmult-base.c src/ed25519-donna/ed25519-sha3.c src/ed25519-donna/ed25519-keccak.c src/ed25519-donna/ed25519-blake2b.c
    src/sodium/private/fe_25_5/fe.c src/sodium/private/ed25519_ref10.c src/sodium/private/ed25519_ref10_fe_25_5.c src/sodium/keypair.c
    src/monero/base58.c
    src/monero/serialize.c
    src/monero/xmr.c
    src/monero/range_proof.c
    src/blake256.c
    src/blake2b.c src/blake2s.c
    src/chacha20poly1305/chacha20poly1305.c src/chacha20poly1305/chacha_merged.c src/chacha20poly1305/poly1305-donna.c src/chacha20poly1305/rfc7539.c
    src/rc4.c
    src/nano.c
    src/nem.c
    src/cash_addr.c
    src/memzero.c
    src/scrypt.c
    src/ripple/base58.c
    src/nist256p1.c
    src/groestl.c
    src/schnorr.c
)

target_compile_options(TrezorCrypto
    PRIVATE
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
        -Werror
)

target_include_directories(TrezorCrypto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)

if(NOT ANDROID AND NOT IOS_PLATFORM)
    add_subdirectory(tests)
endif()

install(TARGETS TrezorCrypto
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
