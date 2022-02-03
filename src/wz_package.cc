#include "stdafx.h"
#include "wz_package.h"
#include "wz_type.h"
#include "wz_property.h"

WzPackage::WzPackage(const std::unique_ptr<WzStream>& stream, const std::string& name, std::streamoff offset) :
	WzPackage(stream, nullptr, name, 0, 0, offset) {
}

WzPackage::WzPackage(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, int32_t size, int32_t check_sum, std::streamoff offset) :
 WzNamespace(stream, parent, name, offset), 
 size_(size),
 check_sum_(check_sum) {}


WzPackage::~WzPackage() {}

void WzPackage::Load() {
	stream()->seek(offset());
	int32_t num = stream()->ReadCompressedInt();
	for (int i = 0; i < num; i++) {
		uint8_t type = stream()->read<uint8_t>();
		std::string name;
		int32_t size;
		int32_t check_sum;
		std::streamoff offset;
		std::streamoff remember_pos = 0;
		if (type == 1) {
			int32_t unknown = stream()->read<int32_t>();
			stream()->read<int16_t>();
			stream()->read<uint32_t>();
			continue;
		} else if (type == 2) {
			int32_t string_offset = stream()->read<int32_t>();
			remember_pos = stream()->tell();
			stream()->seek(static_cast<std::streampos>(stream()->begin_pos() + string_offset));
			type = stream()->read<uint8_t>();
			name = stream()->ReadString();
		} else if (type == 3 || type == 4) {
			name = stream()->ReadString();
			remember_pos = stream()->tell();
		}
		stream()->seek(remember_pos);
		size = stream()->ReadCompressedInt();
		check_sum = stream()->ReadCompressedInt();
		offset = stream()->ReadPos();
		if (type == static_cast<int>(WzType::kDirectory)) {
			AddChild(name, std::make_shared<WzPackage>(stream(), shared_from_this(), name, size, check_sum, offset));
		} else if (type == static_cast<int>(WzType::kProperty)) {
			AddProperty(name, size, check_sum, offset);
		}
	}
	set_loaded(true);
}

void WzPackage::AddProperty(const std::string& name, int size, int check_sum, std::streamoff offset) {
	std::streamoff remember_pos = stream()->tell();
	stream()->seek(offset);
	std::string property_type = stream()->ReadPropertyString(offset);
	if (property_type == "Property") {
		AddChild(name, std::make_shared<WzProperty>(stream(), shared_from_this(), name, size, check_sum, offset, stream()->tell()));
	}
	stream()->seek(remember_pos);
}