#include "Action.h"
#include "../HexCoding.h"

using namespace TW::EOS;
using json = nlohmann::json;

void PermissionLevel::serialize(Data& o) const {
    actor.serialize(o);
    permission.serialize(o);
}

json PermissionLevel::serialize() const noexcept {
    json obj;
    obj["actor"] = actor.string();
    obj["permission"] = permission.string();
    return obj;
}

void Action::serialize(Data& o) const {
    account.serialize(o);
    name.serialize(o);
    encodeCollection(authorization, o);
    Bravo::encodeVarInt32(data.size(), o);
    append(o, data);
}

json Action::serialize() const noexcept {
    json obj;
    obj["account"] = account.string();
    obj["name"] = name.string();
    obj["authorizations"] = encodeCollection(authorization);
    obj["data"] = hex(data);
    return obj;
}

TransferAction::TransferAction( const std::string& currency,
                                const std::string& from, 
                                const std::string& to, 
                                const std::string& assetString, 
                                const std::string& memo) {
    account = Name(currency);
    name = Name("transfer");
    authorization.push_back(PermissionLevel(Name(from), Name("active")));
    
    setData(from, to, assetString, memo);
}

void TransferAction::setData(const std::string& from, const std::string& to, const std::string& assetString, const std::string& memo) {
    Name(from).serialize(data);
    Name(to).serialize(data);
    Bravo::Asset::fromString(assetString).serialize(data);
    Bravo::encodeString(memo, data);
}