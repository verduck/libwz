#pragma once

#include "wz_property.h"

class WzUOL : public WzProperty {
public:
  WzUOL(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset);
  ~WzUOL();

  std::shared_ptr<WzNamespace> Get();

  virtual void Load() override;
private:
  std::string uol_;
};