// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

namespace TW::NEO {

enum TransactionAttributeUsage {
    TAU_ContractHash = 0x00,

    TAU_ECDH02 = 0x02,
    TAU_ECDH03 = 0x03,

    TAU_Script = 0x20,

    TAU_Vote = 0x30,

    TAU_DescriptionUrl = 0x81,
    TAU_Description = 0x90,

    TAU_Hash1 = 0xa1,
    TAU_Hash2 = 0xa2,
    TAU_Hash3 = 0xa3,
    TAU_Hash4 = 0xa4,
    TAU_Hash5 = 0xa5,
    TAU_Hash6 = 0xa6,
    TAU_Hash7 = 0xa7,
    TAU_Hash8 = 0xa8,
    TAU_Hash9 = 0xa9,
    TAU_Hash10 = 0xaa,
    TAU_Hash11 = 0xab,
    TAU_Hash12 = 0xac,
    TAU_Hash13 = 0xad,
    TAU_Hash14 = 0xae,
    TAU_Hash15 = 0xaf,

    TAU_Remark = 0xf0,
    TAU_Remark1 = 0xf1,
    TAU_Remark2 = 0xf2,
    TAU_Remark3 = 0xf3,
    TAU_Remark4 = 0xf4,
    TAU_Remark5 = 0xf5,
    TAU_Remark6 = 0xf6,
    TAU_Remark7 = 0xf7,
    TAU_Remark8 = 0xf8,
    TAU_Remark9 = 0xf9,
    TAU_Remark10 = 0xfa,
    TAU_Remark11 = 0xfb,
    TAU_Remark12 = 0xfc,
    TAU_Remark13 = 0xfd,
    TAU_Remark14 = 0xfe,
    TAU_Remark15 = 0xff
};

}
