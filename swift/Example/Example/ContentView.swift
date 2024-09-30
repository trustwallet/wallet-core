// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import SwiftUI
import WalletCore

struct ContentView: View {

    let wallet = HDWallet(strength: 256, passphrase: "")!
    var body: some View {
        Text("Ethereum address: \(wallet.getAddressForCoin(coin: .ethereum))")
            .padding()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
