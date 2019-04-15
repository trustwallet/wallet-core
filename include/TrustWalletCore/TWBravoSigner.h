#pragma once

#include "TWBase.h"

#include "TWBravoProto.h"
#include "TWCommonProto.h"

TW_EXTERN_C_BEGIN

/// Represents a Bravo Signer.
TW_EXPORT_CLASS
struct TWBravoSigner;

TW_EXPORT_STATIC_METHOD
TW_Proto_Result TWBravoSignerSign(TW_Bravo_Proto_SigningInput input);

TW_EXTERN_C_END