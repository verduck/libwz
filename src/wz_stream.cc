#include "stdafx.h"
#include "wz_stream.h"

WzStream::WzStream(const std::string& filename) :
    file_stream_(filename, std::ios::in | std::ios::binary) {}

WzStream::~WzStream() {}

std::streampos WzStream::tell() {
    return file_stream_.tellg();
}

void WzStream::seek(std::streamoff off) {
    file_stream_.seekg(off);
}

void WzStream::seek(std::streamoff off, std::ios_base::seekdir way) {
    file_stream_.seekg(off, way);
}

int32_t WzStream::begin_pos() {
    return begin_pos_;
}

void WzStream::set_begin_pos(int32_t begin_pos) {
    begin_pos_ = begin_pos;
}

void WzStream::GenerateKey(int32_t version) {
    int32_t encrypted_version_num = static_cast<int32_t>(read<int16_t>());
	int32_t version_hash = 0;
	int32_t decrypted_version_num = 0;
	std::string version_num_str = std::to_string(version);

	int32_t a = 0, b = 0, c = 0, d = 0, l = 0;
	l = static_cast<int32_t>(version_num_str.size());
	for (int i = 0; i < l; i++) {
		version_hash = (32 * version_hash) + static_cast<int32_t>(version_num_str[i] + 1);
	}
	a = (version_hash >> 24) & 0xFF;
	b = (version_hash >> 16) & 0xFF;
	c = (version_hash >> 8) & 0xFF;
	d = version_hash & 0xFF;

	decrypted_version_num = (0xff ^ a ^ b ^ c ^ d);

	if (encrypted_version_num == decrypted_version_num) {
		version_hash_ = static_cast<uint32_t>(version_hash);
	} else {
		version_hash_ = 0;
	}
}

std::streampos WzStream::ReadPos() {
    uint32_t pos = static_cast<uint32_t>(tell());
	pos = (pos - begin_pos_) ^ 0xFFFFFFFF;
	pos *= version_hash_;
	pos -= 0x581C3F6D;
	pos = (pos << (pos & 0x1F)) | (pos >> (32 - pos & 0x1F));
    uint32_t encrypted_pos = read<uint32_t>();
    pos ^= encrypted_pos;
    pos += begin_pos_ * 2;

	return static_cast<std::streamoff>(pos);
}

std::string WzStream::ReadString() {
    std::string result;
    int32_t length = read<int8_t>();
    if (length == 0) {
        return result;
    } else if (length > 0) {
        std::wstring wresult;
        using convert_type = std::codecvt_utf8<wchar_t>;
        uint16_t mask = 0xAAAA;
        if (length == 127)
            length = read<int32_t>();
        if (length <= 0)
            return result;
        for (int i = 0; i < length; i++) {
            uint16_t encrypted_char = read<uint16_t>();
            encrypted_char ^= mask;
            encrypted_char ^= 0;
            wresult += static_cast<wchar_t>(encrypted_char);
            mask++;
        }
        std::wstring_convert<convert_type, wchar_t> converter;
		result = converter.to_bytes(wresult);
    } else {
        uint8_t mask = 0xAA;
        if (length == -128)
            length = read<int32_t>();
        else
            length = -length;
        if (length <= 0)
            return result;
        for (int i = 0; i < length; i++) {
            uint8_t encrypted_char = read<uint8_t>();
            encrypted_char ^= mask;
            encrypted_char ^= 0;
            result += static_cast<char>(encrypted_char);
            mask++;
        }
    }

    return result;
}

std::string WzStream::ReadStringByLength(size_t length) {
    std::string result;
    uint8_t ch = 0;
    for (size_t i = 0; i < length; i++) {
        ch = read<uint8_t>();
        result += ch;
    }
    return result;
}

std::string WzStream::ReadNullTerminatedString() {
    std::string result;
    uint8_t ch = read<uint8_t>();
    while (ch != 0) {
        result += ch;
        ch = read<uint8_t>();
    }

    return result;
}

std::string WzStream::ReadPropertyString(std::streamoff offset) {
    std::string result;

    uint8_t key = read<uint8_t>();
    switch (key) {
    case 0:
    case 0x73:
        result = ReadString();
        break;
    case 1:
    case 0x1B:
    {
        int32_t increment = read<int32_t>();
        std::streamoff off = offset + static_cast<std::streamoff>(increment);
        std::streamoff temp = tell();
        seek(off);
        result = ReadString();
        seek(temp);
        break;
    }
    default:
        result = "";
        break;
    }

    return result;
}

int32_t WzStream::ReadCompressedInt() {
    int8_t num = read<int8_t>();
    if (num == -128)
        return read<int32_t>();
    else
        return static_cast<int32_t>(num);
}

void WzStream::read(char* str, std::streamsize count) {
    file_stream_.read(str, count);
}