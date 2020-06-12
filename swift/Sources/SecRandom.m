@import Foundation;
@import Security;

uint32_t random32(void) {
    uint32_t value;
    if (SecRandomCopyBytes(kSecRandomDefault, sizeof(value), &value) != errSecSuccess) {
        // failed to generate random number
        abort();
    }
    return value;
}

void random_buffer(uint8_t *buf, size_t len) {
    if (SecRandomCopyBytes(kSecRandomDefault, len, buf) != errSecSuccess) {
        // failed to generate random number
        abort();
    }
}
