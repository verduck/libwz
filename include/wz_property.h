#pragma once

#include "wz_namespace.h"

class WzProperty : public WzNamespace {
public:
    WzProperty(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, int32_t size, int32_t check_sum, std::streamoff root_property_offset, std::streamoff offset);
    WzProperty(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset);
    virtual ~WzProperty();

    virtual void Load() override;
protected:
    void LoadProperty();
    void LoadExtendedProperty(const std::string& name);

    std::streamoff root_property_offset() const;
private:
    int32_t size_;
    int32_t check_sum_;
    std::streamoff root_property_offset_;
};