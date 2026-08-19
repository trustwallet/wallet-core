// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "WalletCore",
    platforms: [.iOS(.v13)],
    products: [
        .library(name: "WalletCore", targets: ["WalletCore"]),
        .library(name: "WalletCoreSwiftProtobuf", targets: ["WalletCoreSwiftProtobuf"])
    ],
    dependencies: [],
    targets: [
        .binaryTarget(
            name: "WalletCore",
            url: "https://github.com/trustwallet/wallet-core/releases/download/4.7.3/WalletCore.xcframework.zip",
            checksum: "179b12764383479fe8ab02689872113ebdfcf923ca96896c133a31e516b038dc"
        ),
        .binaryTarget(
            name: "WalletCoreSwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/4.7.3/WalletCoreSwiftProtobuf.xcframework.zip",
            checksum: "875e4d9fa1279dda74a2b040eb75896496860352d56778d3cdd0959d20ee93a8"
        )
    ]
)
