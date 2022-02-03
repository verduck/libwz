#include "stdafx.h"
#include "wz_sound.h"

WzSound::WzSound(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset) :
  WzNamespace(stream, parent, name, offset),
  play_time_(0) {

  Load();
}

WzSound::~WzSound() {}

void WzSound::Load() {
  stream()->seek(offset());
  stream()->seek(1, std::ios::cur);
  int32_t len = stream()->ReadCompressedInt();
  play_time_ = stream()->ReadCompressedInt();
  header_ = std::make_unique<uint8_t[]>(82);
  stream()->read((char*) header_.get(), 82);
  bytes_ = std::make_unique<uint8_t[]>(len);
  stream()->read((char*) bytes_.get(), len);
  set_loaded(true);
}

uint8_t *WzSound::bytes() const {
  return bytes_.get();
}