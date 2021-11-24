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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.34/WalletCore.xcframework.zip",
            checksum: "1595ed7e1efb85e939dbbc8e7b763a8d4a4285d209f622aa960701b0cc054f0d"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.34/SwiftProtobuf.xcframework.zip",
            checksum: "c7839954f10427df3f47629880f1ed76c39201500c8e5efcba808e37893995b9"
        )
    ]
)
