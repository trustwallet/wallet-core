#include "Data.h"

#include <string>

inline TW::Data TW::readBytes(const TW::Data &from, int max, int initial_pos) {
    if (from.size() - initial_pos < max) {
        throw std::invalid_argument("Data::Cannot read enough bytes!");
    }
    return TW::Data(from.begin() + initial_pos, from.begin() + initial_pos + max);
}

inline uint16_t TW::readUInt16(const TW::Data &from, int initial_pos) {
    TW::Data bytes = readBytes(from, 2, initial_pos);
    uint16_t val = bytes[0] | (bytes[1] << 8);
    return val;
}

// read int16 in little-endian format
inline int16_t TW::readInt16(const TW::Data &from, int initial_pos) {
    return (int16_t) readUInt16(from, initial_pos);
}

inline uint64_t TW::readUInt64(const TW::Data &from, int initial_pos)
{
    TW::Data bytes = readBytes(from, 8, initial_pos);
    uint32_t val1 = bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24);
    ulong val2 = bytes[4] | (bytes[5]<<8) | (bytes[6]<<16) | (bytes[7]<<24);
    ulong val = val1 | val2 << 32;
    return val;
}

inline int64_t TW::readInt64(const TW::Data &from, int initial_pos)
{
    return (int64_t) readUInt64(from, initial_pos);
}

inline TW::Data TW::writeUshort(ushort v) {
    return TW::Data({(byte) (((v >> 0) << 8) >> 8), (byte) (((v >> 8) << 8) >> 8)});
}

inline TW::Data TW::writeShort(short v) {
    return writeUshort(ushort(v));
}

inline TW::Data TW::writeUint(uint v) {
    return Data({
        (byte) (((v >> 0) << 24) >> 24),
        (byte) (((v >> 8) << 24) >> 24),
        (byte) (((v >> 16) << 24) >> 24),
        (byte) (((v >> 24) << 24) >> 24),
    });
}

inline TW::Data TW::writeInt(int v) {
    return writeUint(uint(v));
}

inline TW::Data TW::writeUlong(ulong v) {
    return Data({
        (byte) (((v >> 0) << 56) >> 56),
        (byte) (((v >> 8) << 56) >> 56),
        (byte) (((v >> 16) << 56) >> 56),
        (byte) (((v >> 24) << 56) >> 56),
        (byte) (((v >> 32) << 56) >> 56),
        (byte) (((v >> 40) << 56) >> 56),
        (byte) (((v >> 48) << 56) >> 56),
        (byte) (((v >> 56) << 56) >> 56)
    });
}

inline TW::Data TW::writeLong(long v) {
    return writeUlong(ulong(v));
}
