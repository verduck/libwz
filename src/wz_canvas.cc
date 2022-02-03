#include "stdafx.h"
#include "wz_canvas.h"
#include "wz_property.h"
#include "wz_variant.h"

WzCanvas::WzCanvas(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset) :
	WzProperty(stream, parent, name, root_property_offset, offset),
	width_(0),
	height_(0) {
}

WzCanvas::~WzCanvas() {
	//glDeleteTextures(1, &texture_id_);
}

void WzCanvas::Load() {
	stream()->seek(offset());
	std::streamoff data_begin_offset;
	int32_t format, data_len, dec_data_len;
	uint16_t header;
	std::unique_ptr<uint8_t[]> data;
	stream()->seek(1, std::ios::cur);
	if (stream()->read<uint8_t>() == 1) {
		LoadProperty();
	}
	width_ = stream()->ReadCompressedInt();
	height_ = stream()->ReadCompressedInt();
	format_ = stream()->ReadCompressedInt() + stream()->read<uint8_t>();

	if (stream()->read<int32_t>() != 0) {
		return;
	}
	data_len = stream()->read<int32_t>() - 1;
	switch (format_) {
	case 1:
		dec_data_len = width_ * height_ * 2;
		break;
	case 2:
		dec_data_len = width_ * height_ * 4;
		break;
	case 513:
		dec_data_len = width_ * height_ * 2;
		break;
	case 517:
		dec_data_len = width_ * height_ / 128;
		break;
	
	default:
		break;
	}
	stream()->seek(1, std::ios::cur);

	data_begin_offset = stream()->tell();
	header = stream()->read<uint16_t>();
	stream()->seek(data_begin_offset);
	if (header != 0x9c78 && header != 0xda78) {
		uint64_t end_of_data = static_cast<uint64_t>(data_begin_offset) + data_len;
		int blocks_size = 0;
		std::vector<uint8_t> data_stream;
		while (data_begin_offset < end_of_data) {
			blocks_size = stream()->read<uint32_t>();
			for (int i = 0; i < blocks_size; i++)
				data_stream.push_back(stream()->read<uint8_t>() ^ 0);
		}
		size_t dst_len = dec_data_len;
		data = std::make_unique<uint8_t[]>(dst_len);
		uncompress(static_cast<Bytef*>(data.get()), (uLongf*)&dst_len, static_cast<Bytef*>(&data_stream[0]), static_cast<uLong>(data_stream.size()));
	} else {
		std::unique_ptr<uint8_t[]> buf_src = std::make_unique<uint8_t[]>(data_len);
		stream()->read((char*) buf_src.get(), data_len);
		if (buf_src) {
			size_t dst_len = dec_data_len;
			data = std::make_unique<uint8_t[]>(dst_len);
			uncompress(static_cast<Bytef*>(data.get()), (uLongf*)&dst_len, static_cast<Bytef*>(buf_src.get()), data_len);
		}
	}
	set_loaded(true);
}

void WzCanvas::generate_texture(int32_t format, int32_t size, uint8_t* data) {
	std::unique_ptr<uint8_t[]> pixels;
	size_t bpp = 0, szBuf;
	//GLenum type = GL_UNSIGNED_BYTE;
	switch (format) {
	case 1: {
		szBuf = size * 2;
		pixels = std::make_unique<uint8_t[]>(szBuf);
		int b, g;
		for (int i = 0; i < size; i++) {
			b = data[i] & 0x0F;
			b |= (b << 4);
			pixels[i * 2] = (uint8_t)b;

			g = data[i] & 0xF0;
			g |= (g >> 4);
			pixels[i * 2 + 1] = (uint8_t)g;
		}
		/*for (uint32_t i = 0; i < size; i++) {
			pixels[i * 2] = (data[i] & 0x0F) * 0x11;
			pixels[i * 2 + 1] = ((data[i] & 0xF0) >> 4) * 0x11;
		}*/

		//type = GL_UNSIGNED_BYTE;
	}
	break;
	case 2: {
		bpp = 32;
		szBuf = size;

		pixels = std::make_unique<uint8_t[]>(szBuf);
		memmove(pixels.get(), data, szBuf);
		//type = GL_UNSIGNED_BYTE;
	}
	break;
	case 513: {
		bpp = 16;
		szBuf = size;

		pixels = std::make_unique<uint8_t[]>(szBuf);
		memmove(pixels.get(), data, szBuf);
		//type = GL_UNSIGNED_SHORT_5_6_5_REV;
	}
	break;
	case 517: {
		bpp = 8; // or 16
		szBuf = width_ * height_;
		pixels = std::make_unique<uint8_t[]>(szBuf);
		int x = 0, y = 0;
		uint8_t iB = 0;
		for (int i = 0; i < size; i++) {
			for (uint8_t j = 0; j < 8; j++) {
				iB = ((data[i] & (0x01 << (7 - j))) >> (7 - j)) * 0xFF;
				for (int k = 0; k < 16; k++) {
					if (x == width_) {
						x = 0;
						y++;
					}
					pixels[y * height_ + x] = iB;
					x++;
				}
			}
		}
		//type = GL_UNSIGNED_SHORT_5_6_5_REV;
	}
	break;
	default:
		break;
	}
}

int32_t WzCanvas::width() const {
	return width_;
}

int32_t WzCanvas::height() const {
	return height_;
}