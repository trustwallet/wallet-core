cc_library(
    name = "trustwallet_includes",
    hdrs = glob([
        "include/**/*.h",
    ]),
    strip_include_prefix = "/3rdparty/trustwallet/include",
    visibility = ["//visibility:public"],
    deps = [
    ],
    copts = [
            "-D_Nonnull=",
            ],
)

cc_library(
    name = "trustwallet",
    srcs = glob([
        "src/**/*.cpp",
        "src/**/*.cc",
    ]),
    hdrs = glob([
        "src/**/*.h",
        "src/**/*.hpp",
    ]),
    strip_include_prefix = "/3rdparty/trustwallet/src",
    visibility = ["//visibility:public"],
    deps = [
        ":trustwallet_includes",
        "//3rdparty:nlohmann",
        "//3rdparty/generated/cpp",
        "//3rdparty/trustwallet/rust:wallet_core_rs",
        "//3rdparty/trustwallet/trezor-crypto",
        "//3rdparty/trustwallet/trezor-crypto:ed25519",
        "@boost//:algorithm",
        "@boost//:archive",
        "@boost//:lexical_cast",
        "@boost//:multiprecision",
        "@boost//:serialization",
        "@boost//:uuid",
        "@org_libsodium//:libsodium",
        "@protobuf//:protobuf",
    ],
    copts = [
        "-D_Nonnull=",
        ],
)
