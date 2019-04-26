#pragma once

#include "../Bravo/Asset.h"
#include "../BinaryCoding.h"
#include "Name.h"

#include <nlohmann/json.hpp>
#include <string>

using Data = TW::Data;

namespace TW::EOS {
class PermissionLevel {
public:
    Name actor, permission;

    PermissionLevel(const Name& actor, const Name& permission) : actor(actor), permission(permission) { }
    virtual ~PermissionLevel() { }

    void serialize(Data& o) const;
    nlohmann::json serialize() const noexcept;
};

class Action {
public:
    Name account, name;
    std::vector<PermissionLevel> authorization;
    Data data;

    virtual ~Action() { }

    virtual void serialize(Data& o) const;
    virtual nlohmann::json serialize() const noexcept;
};

class TransferAction: public Action {
public:
    TransferAction(const std::string& currency, const std::string& from, const std::string& to, const Bravo::Asset& asset, const std::string& memo);
private:
    void setData(const std::string& from, const std::string& to, const Bravo::Asset& asset, const std::string& memo);
};
} // namespace TW::EOS