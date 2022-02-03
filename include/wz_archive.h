#pragma once

#include "wz_stream.h"
#include "wz_namespace.h"
#include "wz_package.h"

class WzArchive {
public:
    WzArchive(const std::string& path, int32_t version);

    void Load();
    std::shared_ptr<WzNamespace> GetChild(const std::string& name) const;
    std::shared_ptr<WzNamespace> GetChildFromPath(const std::string& path) const;
private:
    std::string name_;
    int32_t version_;
    std::unique_ptr<WzStream> stream_;
    std::shared_ptr<WzPackage> root_;
};