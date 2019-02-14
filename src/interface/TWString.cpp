#include <TrustWalletCore/TWString.h>
#include <string>

TWString *_Nonnull TWStringCreateWithUTF8Bytes(const char *_Nonnull bytes) {
    auto s = new std::string(bytes);
    return s;
}

size_t TWStringSize(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return s->size();
}

char TWStringGet(TWString *_Nonnull string, size_t index) {
    auto s = reinterpret_cast<const std::string*>(string);
    return (*s)[index];
}

const char *_Nonnull TWStringUTF8Bytes(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return s->data();
}

void TWStringDelete(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    delete s;
}
