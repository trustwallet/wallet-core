// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import * as Loader from "./lib/wallet-core";
import { TW } from "./generated/core_proto";
import { WalletCore } from "./src/wallet-core";
import * as KeyStore from "./src/keystore";

declare function load(): Promise<WalletCore>;

export const initWasm: typeof load = Loader;
export { TW, WalletCore, KeyStore };
