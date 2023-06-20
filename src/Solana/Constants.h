// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::Solana {
// https://docs.solana.com/developing/programming-model/transactions

static const std::string SYSTEM_PROGRAM_ID_ADDRESS = "11111111111111111111111111111111";
static const std::string STAKE_PROGRAM_ID_ADDRESS = "Stake11111111111111111111111111111111111111";
static const std::string TOKEN_PROGRAM_ID_ADDRESS = "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA";
static const std::string ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS = "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL";
static const std::string SYSVAR_RENT_ID_ADDRESS = "SysvarRent111111111111111111111111111111111";
static const std::string SYSVAR_CLOCK_ID_ADDRESS = "SysvarC1ock11111111111111111111111111111111";
static const std::string STAKE_CONFIG_ID_ADDRESS = "StakeConfig11111111111111111111111111111111";
static const std::string NULL_ID_ADDRESS = "11111111111111111111111111111111";
static const std::string SYSVAR_STAKE_HISTORY_ID_ADDRESS = "SysvarStakeHistory1111111111111111111111111";
static const std::string MEMO_PROGRAM_ID_ADDRESS = "MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr";
static const std::string SYSVAR_RECENT_BLOCKHASHS_ADDRESS = "SysvarRecentB1ockHashes11111111111111111111";
// https://github.com/solana-labs/solana/blob/master/sdk/program/src/message/versions/mod.rs#L24
static const std::uint8_t MESSAGE_VERSION_PREFIX{0x80};

}
