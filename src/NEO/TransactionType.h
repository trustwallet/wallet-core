// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

namespace TW::NEO {

enum TransactionType // int, not byte
{
    TT_MinerTransaction = 0x00,
    TT_IssueTransaction = 0x01,
    TT_ClaimTransaction = 0x02,
    TT_EnrollmentTransaction = 0x20,
    TT_RegisterTransaction = 0x40,
    TT_ContractTransaction = 0x80,
    TT_StateTransaction = 0x90,
    TT_PublishTransaction = 0xd0,
    TT_InvocationTransaction = 0xd1
};

}
