#pragma once

#include "wz_namespace.h"

class WzSound : public WzNamespace {
public:
  WzSound(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset);
  ~WzSound();

  uint8_t *bytes() const;

  virtual void Load() override;
private:
  int32_t play_time_;
  std::unique_ptr<uint8_t[]> header_;
  std::unique_ptr<uint8_t[]> bytes_;
};