cc_library(
    name = "trezor_headers",
    hdrs = glob([
        "include/**/*.h",
    ]),
    strip_include_prefix = "/3rdparty/trustwallet/trezor-crypto/include",
    deps = [
    ],
)

cc_library(
    name = "ed25519",
    srcs = glob([
        "crypto/ed25519-donna/*.c",
    ]),
    hdrs = glob([
        "crypto/ed25519-donna/*.c",
    ]),
    strip_include_prefix = "/3rdparty/trustwallet/trezor-crypto/crypto/ed25519-donna",
    visibility = ["//visibility:public"],
    deps = [
        ":trezor_headers",
    ],
)

cc_library(
    name = "monero_headers",
    hdrs = [
        "crypto/monero/base58.h",
        "crypto/monero/int-util.h",
        "crypto/monero/monero.h",
        "crypto/monero/range_proof.h",
        "crypto/monero/serialize.h",
        "crypto/monero/xmr.h",
    ],
    strip_include_prefix = "/3rdparty/trustwallet/trezor-crypto/crypto/monero",
    deps = [
    ],
)

cc_library(
    name = "table_headers",
    hdrs = [
        "crypto/nist256p1.table",
        "crypto/secp256k1.table",
    ],
    strip_include_prefix = "/3rdparty/trustwallet/trezor-crypto/crypto",
    deps = [
    ],
)

cc_library(
    name = "trezor-crypto",
    srcs = glob(
        [
            "**/*.c",
        ],
        exclude = [
            "crypto/ed25519-donna/*.c",
            "crypto/tests/**/*",
            "crypto/tools/**/*",
        ],
    ),
    visibility = ["//visibility:public"],
    deps = [
        ":ed25519",
        ":monero_headers",
        ":table_headers",
        ":trezor_headers",
        "@org_libsodium//:libsodium",
    ],
)
