@import Foundation;
@import Security;

uint32_t random32(void) {
    uint32_t value;
    int status = SecRandomCopyBytes(kSecRandomDefault, sizeof(value), &value);
    NSCAssert(status == errSecSuccess, @"Failed to generate random number");
    return value;
}

void random_buffer(uint8_t *buf, size_t len) {
    int status = SecRandomCopyBytes(kSecRandomDefault, len, buf);
    NSCAssert(status == errSecSuccess, @"Failed to generate random number");
}
