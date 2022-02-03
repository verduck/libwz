#include "stdafx.h"
#include "wz_convex2d.h"

WzConvex2D::WzConvex2D(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset) :
  WzProperty(stream, parent, name, root_property_offset, offset) {
}

WzConvex2D::~WzConvex2D() {}

void WzConvex2D::Load() {
  stream()->seek(offset());
  int32_t count = stream()->ReadCompressedInt();
  for (int i = 0; i < count; i++) {
    LoadExtendedProperty(name());
  }
}