// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWOntologyProto.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWOntologySigner;

TW_EXPORT_STATIC_METHOD
TW_Ontology_Proto_SigningOutput TWOntologySignerSign(TW_Ontology_Proto_SigningInput input);

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWOntologySignerMessage(TW_Ontology_Proto_TransactionInput input);

/// Builds a transaction to be broadcasted
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWOntologySignerTransaction(TW_Ontology_Proto_TransactionInput data, TWData *_Nonnull pubKey, TWData *_Nonnull signature);

TW_EXTERN_C_END
