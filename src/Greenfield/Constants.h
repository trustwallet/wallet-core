// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>

#pragma once

namespace TW::Greenfield {

static constexpr uint64_t TIMEOUT_HEIGHT = 0;
static constexpr auto* TIMEOUT_HEIGHT_STR = "0";
static constexpr auto* FEE_GRANTER = "";
static constexpr auto* MSG_SEND_TYPE = "/cosmos.bank.v1beta1.MsgSend";
static constexpr auto* MSG_TRANSFER_OUT_TYPE = "/greenfield.bridge.MsgTransferOut";

} // namespace TW::Greenfield
