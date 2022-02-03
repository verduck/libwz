#include "stdafx.h"
#include "wz_uol.h"

WzUOL::WzUOL(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset)
  : WzProperty(stream, parent, name, root_property_offset, offset) {

  Load();
}

WzUOL::~WzUOL() {}

void WzUOL::Load() {
  stream()->seek(offset());
  uol_ = stream()->ReadPropertyString(root_property_offset());
  set_loaded(true);
}

std::shared_ptr<WzNamespace> WzUOL::Get() {
  std::shared_ptr<WzNamespace> cur = parent();
  cur = cur->GetChildFromPath(uol_);
  return cur;
}