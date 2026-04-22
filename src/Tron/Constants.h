// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

namespace TW::Tron {

// ---------------------------------------------------------------------------
// JSON field name constants
// ---------------------------------------------------------------------------

// Top-level transaction fields
static constexpr auto kFieldTypeUrlPrefix      = "type.googleapis.com";
static constexpr auto kFieldRawData            = "raw_data";
static constexpr auto kFieldSignature          = "signature";
static constexpr auto kFieldTxID               = "txID";
static constexpr auto kFieldRawDataHex         = "raw_data_hex";

// Contract wrapper fields
static constexpr auto kFieldType               = "type";
static constexpr auto kFieldParameter          = "parameter";
static constexpr auto kFieldProvider           = "provider";
static constexpr auto kFieldValue              = "value";
static constexpr auto kFieldTypeUrl            = "type_url";
static constexpr auto kFieldContract           = "contract";
static constexpr auto kFieldContractName       = "ContractName";
static constexpr auto kFieldPermId             = "Permission_id";

// raw_data fields
static constexpr auto kFieldRefBlockBytes      = "ref_block_bytes";
static constexpr auto kFieldRefBlockHash       = "ref_block_hash";
static constexpr auto kFieldRefBlockNum        = "ref_block_num";
static constexpr auto kFieldTimestamp          = "timestamp";
static constexpr auto kFieldExpiration         = "expiration";
static constexpr auto kFieldFeeLimit           = "fee_limit";
static constexpr auto kFieldData               = "data";

// Contract value fields — common
static constexpr auto kFieldOwnerAddress       = "owner_address";
static constexpr auto kFieldToAddress          = "to_address";
static constexpr auto kFieldAmount             = "amount";
static constexpr auto kFieldResource           = "resource";
static constexpr auto kFieldReceiverAddress    = "receiver_address";
static constexpr auto kFieldBalance            = "balance";

// TransferAssetContract
static constexpr auto kFieldAssetName          = "asset_name";

// VoteAssetContract
static constexpr auto kFieldVoteAddress        = "vote_address";
static constexpr auto kFieldSupport            = "support";
static constexpr auto kFieldCount              = "count";

// VoteWitnessContract
static constexpr auto kFieldVotes              = "votes";
static constexpr auto kFieldVoteCount          = "vote_count";

// FreezeBalanceContract
static constexpr auto kFieldFrozenBalance      = "frozen_balance";
static constexpr auto kFieldFrozenDuration     = "frozen_duration";

// UnfreezeBalanceV2Contract
static constexpr auto kFieldUnfreezeBalance    = "unfreeze_balance";

// DelegateResourceContract
static constexpr auto kFieldLock               = "lock";
static constexpr auto kFieldLockPeriod         = "lock_period";

// TriggerSmartContract
static constexpr auto kFieldContractAddress    = "contract_address";
static constexpr auto kFieldCallValue          = "call_value";
static constexpr auto kFieldCallTokenValue     = "call_token_value";
static constexpr auto kFieldTokenId            = "token_id";

} // namespace TW::Tron

