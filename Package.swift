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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.30/WalletCore.xcframework.zip",
            checksum: "55cd7f532faa22c54c9bfe689ac137ddd9ebef5229782bb7d8a7cba0c58efcf2"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.30/SwiftProtobuf.xcframework.zip",
            checksum: "12651521e4d9c2ee426d2d32f9079cd76901ad0ac3f66b223dea2f7a6b47d262"
        )
    ]
)
