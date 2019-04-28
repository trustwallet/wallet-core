#include "Data.h"

#include <string>

using namespace TW;

inline Data readBytes(const Data &from, int max, int initial_pos = 0) {
    if (from.size() - initial_pos < max) {
        throw std::invalid_argument("Data::Cannot read enough bytes!");
    }
    return Data(from.begin() + initial_pos, from.begin() + initial_pos + max);
}

inline uint16_t readUInt16(const Data &from, int initial_pos = 0) {
    Data bytes = readBytes(from, 2, initial_pos);
    uint16_t val = bytes[0] | (bytes[1] << 8);
    return val;
}

// read int16 in little-endian format
inline int16_t readInt16(const Data &from, int initial_pos = 0) {
    return (int16_t) readUInt16(from, initial_pos);
}

inline uint64_t readUInt64(const Data &from, int initial_pos = 0)
{
    Data bytes = readBytes(from, 8, initial_pos);
    uint32 val1 = bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24);
    ulong val2 = bytes[4] | (bytes[5]<<8) | (bytes[6]<<16) | (bytes[7]<<24);
    ulong val = val1 | val2 << 32;
    return val;
}

inline int64_t readInt64(const Data &from, int initial_pos = 0)
{
    return (int64_t) readUInt64(from, initial_pos);
}

inline Data write(ushort v) {
    return Data({(byte) (((v >> 0) << 8) >> 8), (byte) (((v >> 8) << 8) >> 8)});
}

inline Data write(short v) {
    return write(ushort(v));
}

inline Data write(uint v) {
    return Data({
        (byte) (((v >> 0) << 24) >> 24),
        (byte) (((v >> 8) << 24) >> 24),
        (byte) (((v >> 16) << 24) >> 24),
        (byte) (((v >> 24) << 24) >> 24),
    });
}

inline Data write(int v) {
    return write(uint(v));
}

inline Data write(ulong v) {
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

inline Data write(long v) {
    return write(ulong(v));
}
