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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.33/WalletCore.xcframework.zip",
            checksum: "ac5f5a407292b05cff42fe06d5d73fb2a6c85793c27f75cf973bfa25dd395e2a"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.33/SwiftProtobuf.xcframework.zip",
            checksum: "e3d1ad6189dd788e436044f8a94ee5d0a7fd2e9769e1029ef51b1c612d2fd818"
        )
    ]
)
