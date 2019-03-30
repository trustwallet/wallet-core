// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWData.h"
#include "TWOntologyProto.h"

typedef TWData *_Nonnull TW_Ontology_Proto_SigningInput;
typedef TWData *_Nonnull TW_Ontology_Proto_SigningOutput;

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWOntologySigner;

TW_EXPORT_STATIC_METHOD
TW_Ontology_Proto_SigningOutput TWOntologySignerSign(TW_Ontology_Proto_SigningInput input);

TW_EXTERN_C_END
