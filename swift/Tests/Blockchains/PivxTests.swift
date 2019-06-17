import TrustWalletCore
import XCTest

class PivxAddressTests: XCTestCase {
    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "e2f7e8a1b0b0fa6934f0171a632ac3fc2c96901255ad27c2e97281e1359c76ef")!)!
        let address = CoinType.pivx.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address, "DNzepvRm2MLG5GKdXaJDfX2T4BtWigMdAJ")
    }

    func testExtendedKeys() {
        let wallet = HDWallet.test

        let pprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .pivx, version: .pprv)
        let ppub = wallet.getExtendedPubKey(purpose: .bip44, coin: .pivx, version: .ppub)

        XCTAssertEqual(pprv, "TDt9EdpfhPDELAeGTSrid5xySLbyZsYNTpBhR2cxwsjPRcF8rx9SK93a2ib2Q45LPS5Jm1XMqJwDgitj3ak6rKiJhSpaRPYM4uVJDTZMMeCTudX")
        XCTAssertEqual(ppub, "ToEA6mjPA2rG9FoAHvd3rF1XHVq9QnpwKNbvLMhMyaVF5Sy6yHA622sAxuGeTCtJnMFLY3Ax9hufQ8KNZpqxxu69usfML9A5or6AjgNQMP3HyKh")
    }

    func testDeriveFromPPub() {
        let ppub = "ToEA6mjPA2rG9FoAHvd3rF1XHVq9QnpwKNbvLMhMyaVF5Sy6yHA622sAxuGeTCtJnMFLY3Ax9hufQ8KNZpqxxu69usfML9A5or6AjgNQMP3HyKh"

        let coin = CoinType.pivx
        let ppubAddr2 = HDWallet.derive(from: ppub, at: DerivationPath(purpose: coin.purpose, coinType: coin, account: 0, change: 1, address: 2))!
        let ppubAddr9 = HDWallet.derive(from: ppub, at: DerivationPath(purpose: coin.purpose, coinType: coin, account: 0, change: 1, address: 9))!

        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: ppubAddr2), "DE3kuemmX8HRFG4ExHxBGeH9JLbXh8ykEm")
        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: ppubAddr9), "DQX87A26bCE5SsNJjA11D3GJ6BPpqxz4rK")
    }
}
