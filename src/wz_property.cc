#include "stdafx.h"
#include "wz_property.h"
#include "wz_variant.h"
#include "wz_canvas.h"
#include "wz_convex2d.h"
#include "wz_vector2d.h"
#include "wz_uol.h"
#include "wz_sound.h"

WzProperty::WzProperty(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, int32_t size, int32_t check_sum, std::streamoff root_property_offset, std::streamoff offset) :
	WzNamespace(stream, parent, name, offset),
	size_(size),
	check_sum_(check_sum),
	root_property_offset_(root_property_offset) {
}

WzProperty::WzProperty(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset) :
	WzProperty(stream, parent, name, 0, 0, root_property_offset, offset) {
}

WzProperty::~WzProperty() {}

void WzProperty::Load() {
	stream()->seek(offset());
	LoadProperty();
	set_loaded(true);
}

void WzProperty::LoadProperty() {
	uint16_t b = stream()->read<uint16_t>();
	if (b != 0) {
		return;
	}
	int32_t num = stream()->ReadCompressedInt();
	for (int i = 0; i < num; i++) {
		std::string name = stream()->ReadPropertyString(root_property_offset_);
		uint8_t value_type = stream()->read<uint8_t>();
		switch (value_type) {
		case 0: {
			AddChild(name, std::make_shared<WzVariant<uint8_t>>(shared_from_this(), name, stream()->tell(), 0));
			break;
		}
		case 2:
		case 11: {
			uint16_t data = stream()->read<uint16_t>();
			AddChild(name, std::make_shared<WzVariant<uint16_t>>(shared_from_this(), name, stream()->tell(), data));
			break;
		}
		case 3:
		case 19: {
			int32_t data = stream()->ReadCompressedInt();
			AddChild(name, std::make_shared<WzVariant<int32_t>>(shared_from_this(), name, stream()->tell(), data));
			break;
		}
		case 20: {
			int64_t data = stream()->read<int64_t>();
			AddChild(name, std::make_shared<WzVariant<int64_t>>(shared_from_this(), name, stream()->tell(), data));
			break;
		}
		case 4: {
			float_t data;
			if (stream()->read<char>() == 0x80) {
				data = stream()->read<float_t>();
			} else {
				data = 0.0f;
			}
			AddChild(name, std::make_shared<WzVariant<float_t>>(shared_from_this(), name, stream()->tell(), data));
			break;
		}
		case 5: {
			double_t data = stream()->read<double_t>();
			AddChild(name, std::make_shared<WzVariant<double_t>>(shared_from_this(), name, stream()->tell(), data));
			break;
		}
		case 8: {
			std::string data = stream()->ReadPropertyString(root_property_offset_);
			AddChild(name,  std::make_shared<WzVariant<std::string>>(shared_from_this(), name, stream()->tell(), data));
			break;
		}
		case 9: {
			uint32_t size = stream()->read<uint32_t>();
			std::streamoff pos = stream()->tell() + static_cast<std::streamoff>(size);
			LoadExtendedProperty(name);
			stream()->seek(pos);
			break;
		}
		default:
			break;
		}
	}
}

void WzProperty::LoadExtendedProperty(const std::string& name) {
	std::string property_type = stream()->ReadPropertyString(root_property_offset_);
	if (property_type == "Property") {
		AddChild(name, std::make_shared<WzProperty>(stream(), shared_from_this(), name, root_property_offset_, stream()->tell()));
	} else if (property_type == "Canvas") {
		AddChild(name, std::make_shared<WzCanvas>(stream(), shared_from_this(), name, root_property_offset_, stream()->tell()));
	} else if (property_type == "Shape2D#Convex2D") {
		AddChild(name, std::make_shared<WzConvex2D>(stream(), shared_from_this(), name, root_property_offset_, stream()->tell()));
	} else if (property_type == "Shape2D#Vector2D") {
		AddChild(name, std::make_shared<WzVector2D>(stream(), shared_from_this(), name, stream()->tell()));
	} else if (property_type == "UOL") {
		AddChild(name, std::make_shared<WzUOL>(stream(), shared_from_this(), name, root_property_offset_, stream()->tell()));
	} else if (property_type == "Sound_DX8") {
		AddChild(name, std::make_shared<WzSound>(stream(), shared_from_this(), name, stream()->tell()));
	}
}

std::streamoff WzProperty::root_property_offset() const {
	return root_property_offset_;
}