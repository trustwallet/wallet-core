#include "Data.h"
#include "ReadData.h"

TW::Data TW::readBytes(const TW::Data &from, int max, int initial_pos) {
    if (from.size() - initial_pos < max) {
        throw std::invalid_argument("Data::Cannot read enough bytes!");
    }
    return TW::Data(from.begin() + initial_pos, from.begin() + initial_pos + max);
}

TW::Data TW::readVarBytes(const Data &from, int initial_pos) {
    int32_t size = readInt32(from, initial_pos);
    return readBytes(from, size, initial_pos + 4);
}

uint16_t TW::readUInt16(const TW::Data &from, int initial_pos) {
    TW::Data bytes = readBytes(from, 2, initial_pos);
    uint16_t val = bytes[0] | (bytes[1] << 8);
    return val;
}

uint32_t TW::readUInt32(const Data &from, int initial_pos)  {
    TW::Data bytes = readBytes(from, 4, initial_pos);
    uint32_t val = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    return val;
}

uint64_t TW::readUInt64(const TW::Data &from, int initial_pos)  {
    TW::Data bytes = readBytes(from, 8, initial_pos);
    uint32_t val1 = bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24);
    uint64_t val2 = bytes[4] | (bytes[5]<<8) | (bytes[6]<<16) | (bytes[7]<<24);
    uint64_t val = val1 | val2 << 32;
    return val;
}

int16_t TW::readInt16(const TW::Data &from, int initial_pos) {
    return (int16_t) readUInt16(from, initial_pos);
}

int32_t TW::readInt32(const Data &from, int initial_pos) {
    return (int32_t) readUInt32(from, initial_pos);
}

int64_t TW::readInt64(const TW::Data &from, int initial_pos) {
    return (int64_t) readUInt64(from, initial_pos);
}

uint64_t TW::readVarUInt(const TW::Data &from, uint64_t max, int initial_pos) {
    byte fb = from[initial_pos];
    uint64_t value;
    if (fb == 0xFD) {
        value = readUInt16(from, initial_pos + 1);
    } else if (fb == 0xFE) {
        value = readUInt32(from, initial_pos + 1);
    } else if (fb == 0xFF) {
        value = readUInt64(from, initial_pos + 1);
    } else {
        value = fb;
    }
    if (value > max) {
        // std::cout << "TOO HUGE VALUE: " << value << " max=" << max << std::endl;
        throw std::invalid_argument("IBinaryReader::ReadVarInt error: Too huge value! FormatException");
        return -1;
    }
    return value;
}

int64_t TW::readVarInt(const TW::Data &from, uint64_t max, int initial_pos) {
    return (int64_t) readVarUInt(from, max, initial_pos);
}



TW::Data TW::writeVarBytes(const Data &from, int initial_pos) {
    return concat(writeInt(from.size() - initial_pos), TW::Data(from.begin() + initial_pos, from.end()));
}

TW::Data TW::writeUshort(ushort v) {
    return TW::Data({(byte) (((v >> 0) << 8) >> 8), (byte) (((v >> 8) << 8) >> 8)});
}

TW::Data TW::writeUint(uint v) {
    return Data({
        (byte) (((v >> 0) << 24) >> 24),
        (byte) (((v >> 8) << 24) >> 24),
        (byte) (((v >> 16) << 24) >> 24),
        (byte) (((v >> 24) << 24) >> 24),
    });
}

TW::Data TW::writeUlong(ulong v) {
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

TW::Data TW::writeShort(short v) {
    return writeUshort(ushort(v));
}

TW::Data TW::writeInt(int v) {
    return writeUint(uint(v));
}

TW::Data TW::writeLong(long v) {
    return writeUlong(ulong(v));
}

TW::Data TW::writeVarUInt(ulong value) {
    if (value < 0) {
        throw std::invalid_argument("IBinaryWriter::WriteVarInt ArgumentOutOfRangeException");
    }
    if (value < 0xFD) {
        return Data({(byte) value});
    } else if (value <= 0xFFFF) {
        return concat(Data({(byte) 0xFD}), writeUshort((ushort) value));
    } else if (value <= 0xFFFFFFFF) {
        return concat(Data({(byte) 0xFE}), writeUint((uint) value));
    } else {
        return concat(Data({(byte) 0xFF}), writeUlong(value));
    }
}

TW::Data TW::writeVarInt(long value) {
    return writeVarUInt((ulong) value);
}
