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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.9.5/WalletCore.xcframework.zip",
            checksum: "bb3bccd29e4f03c58bfa1fd746a0f8d6fc4ffc9ec2183beb4facd7b01974a9a2"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.9.5/SwiftProtobuf.xcframework.zip",
            checksum: "7903f5e9487db4764dc57be000c384a0619a96c275993711f3a5a858d3c865bd"
        )
    ]
)
