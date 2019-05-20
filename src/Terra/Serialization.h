
#pragma once

#include "../proto/Terra.pb.h"
#include <nlohmann/json.hpp>

namespace TW::TERRA{

    nlohmann::json signaturePreimage(const Terra::Proto::SigningInput &input);
    nlohmann::json transactionJson(const Terra::Proto::Transaction &transaction);

} // namespace

