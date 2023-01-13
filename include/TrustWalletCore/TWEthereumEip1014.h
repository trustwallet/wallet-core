// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWEthereumEip1014;

/// EIP-1014: Skinny CREATE2 (guess smart contract create2 address)
///
/// \param fromEthAddress valid eth address
/// \param salt always 32 bytes stack item
/// \param initCodeHash The init_code is the code that, when executed, produces the runtime bytecode that will be placed into the state, and which typically is used by high level languages to implement a ‘constructor’. Need to be provided hashed with keccak256
/// \return Ethereum resulting address
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumEip1014AddressCreate2(TWString* _Nonnull fromEthAddress, TWData* _Nonnull salt, TWData* _Nonnull initCodeHash);

TW_EXTERN_C_END
