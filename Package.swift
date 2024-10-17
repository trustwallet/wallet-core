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
            url: "https://github.com/trustwallet/wallet-core/releases/download/4.1.12/WalletCore.xcframework.zip",
            checksum: "1632bbbab1c6a588689eec77a24e1468d9a6746968652cf0a7e669e996c3d24d"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/4.1.12/SwiftProtobuf.xcframework.zip",
            checksum: "33d80c20428c9db4fcf99d1272ba19655f7c6ee7e5b1809fa8a7e7d4aa1b222b"
        )
    ]
)
