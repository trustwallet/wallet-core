// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

/// A key file that was present in the key directory but could not be loaded.
public struct InvalidKey {
    /// The URL of the file on disk.
    public let fileURL: URL

    /// Human-readable reason why the file failed to load.
    public let loadError: String

    /// True when the JSON contains a top-level `address` field, which is the
    /// convention used by third-party keystores (e.g. MyEtherWallet / Geth).
    public let isThirdPartyKeystore: Bool

    /// Address from the first entry in `activeAccounts`, if present in the JSON.
    public let firstAccountAddress: String?
}
