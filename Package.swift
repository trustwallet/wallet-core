// swift-tools-version:5.3

import PackageDescription

let package = Package(
    name: "WalletCore",
    platforms: [.iOS(.v13)],
    products: [
        .library(
            name: "WalletCore", targets: ["WalletCore", "SwiftProtobuf"]
        )
    ],
    dependencies: [
    ],
    targets: [
        .binaryTarget(
            name: "WalletCore",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.31/WalletCore.xcframework.zip",
            checksum: "29d88807485f88992e00a5b7ed1ddf53ca57dacb89ba5e5368dc931102600879"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.31/SwiftProtobuf.xcframework.zip",
            checksum: "d1035aa8a32f2483305bbe9cd3d5774bf6c1a65ce5a37213c9ee651c78873a55"
        )
    ]
)
