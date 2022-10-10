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
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.0.6/WalletCore.xcframework.zip",
            checksum: "a3df0c2b30fc59ede0a2600266fc19b8c0cf655dbef3fb832488c8ddedcb6b93"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.0.6/SwiftProtobuf.xcframework.zip",
            checksum: "61fa8483d4bd43f1898db6997eff0279426f15f9e518e12db0d762ec5f927a9b"
        )
    ]
)
