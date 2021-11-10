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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.32/WalletCore.xcframework.zip",
            checksum: "eaa96d79dbe156f5a0fa2ead3c832096e3ada5d00e4ddd8da8ed45ed06c077c7"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.32/SwiftProtobuf.xcframework.zip",
            checksum: "31c47b25f0accc669299b6fb60b63b87a146fdb15fe3149e996c1e28407d844e"
        )
    ]
)
