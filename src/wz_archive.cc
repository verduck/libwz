#include "stdafx.h"
#include "wz_archive.h"
#include "wz_package.h"

WzArchive::WzArchive(const std::string& path, int32_t version) : 
    name_(path),
    version_(version) {

    stream_ = std::make_unique<WzStream>(path);
}

void WzArchive::Load() {
    stream_->ReadStringByLength(4); // PKG1
    stream_->read<uint64_t>(); // size
    stream_->set_begin_pos(stream_->read<int32_t>()); // begin poss
    stream_->ReadNullTerminatedString();
    stream_->GenerateKey(version_);
    root_ = std::make_shared<WzPackage>(stream_, name_, stream_->tell());
    root_->Load();
}

std::shared_ptr<WzNamespace> WzArchive::GetChild(const std::string& name) const {
    return root_->GetChild(name);
}

std::shared_ptr<WzNamespace> WzArchive::GetChildFromPath(const std::string& path) const {
    return root_->GetChildFromPath(path);
}