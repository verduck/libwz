#pragma once

#include "wz_namespace.h"

class WzPackage : public WzNamespace {
public:
    WzPackage(const std::unique_ptr<WzStream>& stream, const std::string& name, std::streamoff offset);
    WzPackage(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, int32_t size, int32_t check_sum, std::streamoff offset);
    ~WzPackage();

    virtual void Load() override;
private:
    int32_t size_;
    int32_t check_sum_;

    void AddProperty(const std::string& name, int size, int check_sum, std::streamoff offset);
};