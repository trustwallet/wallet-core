#pragma once

#include "TWBase.h"

#include "TWEOSProto.h"
#include "TWCommonProto.h"

TW_EXTERN_C_BEGIN

/// Represents a EOS Signer.
TW_EXPORT_CLASS
struct TWEOSSigner;

TW_EXPORT_STATIC_METHOD
TW_Proto_Result TWEOSSignerSign(TW_EOS_Proto_SigningInput input);

TW_EXTERN_C_END