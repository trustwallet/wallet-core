// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "WalletCore",
    platforms: [.iOS(.v13)],
    products: [
        .library(name: "WalletCore", targets: ["WalletCore"]),
        .library(name: "SwiftProtobuf", targets: ["SwiftProtobuf"])
    ],
    dependencies: [],
    targets: [
        .binaryTarget(
            name: "WalletCore",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.8/WalletCore.xcframework.zip",
            checksum: "ce7e45a8eea666dacc5cd9076f597e4f824c030299414c7fdd91e71b29aa09cc"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.8/SwiftProtobuf.xcframework.zip",
            checksum: "222a306b49d7733b9c5d790b07c0d8240f68f27315f1d127aa4ed743b1b11920"
        )
    ]
)
