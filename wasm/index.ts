// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import * as Loader from "./lib/wallet-core";
import { TW } from "./generated/core_proto";
import { WalletCore } from "./src/wallet-core";
import * as KeyStore from "./src/keystore";

declare function load(): Promise<WalletCore>;

export const initWasm: typeof load = Loader;
export { TW, WalletCore, KeyStore };
