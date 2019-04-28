#include "Account.h"
#include "../Base58.h"

using namespace TW;
using namespace TW::IOST;

bool Account::isValid(const std::string& s) {
    if (s.size() < 5 || s.size() > 11) { 
        return false; 
    }
    for (char ch : s) {
        if ((ch < 'a' || ch > 'z') && (ch < '0' || ch > '9') && ch != '_') {
            return false;
        }
    }
    return true;
}

std::string Account::encodePubKey(const PublicKey& publicKey) {
    return Base58::bitcoin.encode(publicKey.bytes);
} 

Account::Account(const Proto::AccountInfo& account) {
    name = account.name();
    if (account.active_key() != "") {
        auto activeKeyBytesPtr = account.active_key().begin();
        activeKey = Data(activeKeyBytesPtr, activeKeyBytesPtr + PrivateKey::size);
    }
    if (account.owner_key() != "") {
        auto ownerKeyBytesPtr = account.owner_key().begin();
        ownerKey = Data(ownerKeyBytesPtr, ownerKeyBytesPtr + PrivateKey::size);
    }
}

Data Account::sign(const Data& digest, TWCurve curve) const {
    return PrivateKey(activeKey).sign(digest, curve);
}

Data Account::publicActiveKey() const {
    return PrivateKey(activeKey).getPublicKey(TWPublicKeyTypeED25519).bytes;
}

Data Account::publicOwnerKey() const {
    return PrivateKey(ownerKey).getPublicKey(TWPublicKeyTypeED25519).bytes;
}





