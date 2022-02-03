#pragma once

#include <fstream>

class WzStream {
public:
  WzStream(const std::string& filename);
	~WzStream();

	std::streampos tell();
	void seek(std::streamoff off);
	void seek(std::streamoff off, std::ios_base::seekdir way);

	int32_t begin_pos();
	void set_begin_pos(int32_t begin_pos);

	void GenerateKey(int32_t version);

	std::streampos ReadPos();
	std::string ReadString();
	std::string ReadStringByLength(size_t length);
	std::string ReadNullTerminatedString();
	std::string ReadPropertyString(std::streamoff offset);

	int32_t ReadCompressedInt();

	void read(char* str, std::streamsize count);

  template<typename T>
	T read();
private:
	std::ifstream file_stream_;

	uint32_t version_hash_;
	int32_t begin_pos_;
};

template<class T>
inline T WzStream::read() {
	T v;
	file_stream_.read((char*)& v, sizeof(v));
	return v;
}