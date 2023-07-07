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
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.2.1/WalletCore.xcframework.zip",
            checksum: "b7f9fe6b5aa1fd216d43f676400c9db6999c6494e0120db73b2afb49dcc1f013"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.2.1/SwiftProtobuf.xcframework.zip",
            checksum: "12bbc92dc2225c661e301cc2826e034fcb69b2a144f8c4ff2e51ce6ccf122124"
        )
    ]
)
