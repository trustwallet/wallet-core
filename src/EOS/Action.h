#pragma once

#include "../Bravo/Serialization.h"
#include "../Bravo/Asset.h"
#include "../BinaryCoding.h"
#include "Name.h"

#include <nlohmann/json.hpp>
#include <string>

namespace TW {
namespace EOS {
    class PermissionLevel: Bravo::Serializable {
    public:
        Name actor, permission;

        PermissionLevel(const Name& actor, const Name& permission) : actor(actor), permission(permission) { }
        virtual ~PermissionLevel() { }

        void serialize(Data& o) const;
        nlohmann::json serialize() const noexcept;
    };

    class Action: Bravo::Serializable {
    public:
        Name account, name;
        std::vector<PermissionLevel> authorization;
        Data data;

        virtual ~Action() { }

        void serialize(Data& o) const;
        nlohmann::json serialize() const noexcept;
    };

    class TransferAction: public Action {
    public:
        TransferAction(const std::string& currency, const std::string& actor, const std::string& from, const std::string& to, const std::string& assetString, const std::string& memo);
    private:
        void setData(const std::string& from, const std::string& to, const std::string& assetString, const std::string& memo);
    };
}} // namespace

struct TWEOSAction {
    TW::EOS::Action impl;
};