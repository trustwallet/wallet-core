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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.7.8/WalletCore.xcframework.zip",
            checksum: "6f37b01f92fe76b0c97e31652c1756678ff84eb42009fd39b19f534eb9dc562e"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.7.8/SwiftProtobuf.xcframework.zip",
            checksum: "a72a9e8191f1654114fbd2d9f6af3e777c5e3dffa06b2954f047bea1ee6cd529"
        )
    ]
)
