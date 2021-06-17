// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWEthereumAbiStruct;


/// Compute the hash of a struct, used for signing, according to EIP712.
/// Struct is described by a json string (with values), and its type info (may contain type info of sub-types also).
/// Throws on error.
/// Example input:
/// - "Person"
/// - R"({"name": "Cow", "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"})"
/// - R"([{"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]}])"
/// Returned data must be deleted (hint: use WRAPD() macro).
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiStructHashStruct(TWString* _Nonnull structType, TWString* _Nonnull valueJson, TWString* _Nonnull typesJson);

TW_EXTERN_C_END
