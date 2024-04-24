// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "WalletConsole.h"
#include <iostream>

int main() {
    TW::WalletConsole::WalletConsole console(std::cin, std::cout);
    console.loop();
}
