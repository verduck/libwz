#pragma once

#include "wz_property.h"

class WzConvex2D : public WzProperty {
public:
  WzConvex2D(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset);
  ~WzConvex2D();

  virtual void Load() override;
};