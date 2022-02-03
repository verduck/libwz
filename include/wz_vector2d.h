#pragma once

#include "wz_namespace.h"

class WzVector2D : public WzNamespace {
public:
    WzVector2D(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset);
    ~WzVector2D();

    int32_t x() const;
    int32_t y() const;

    virtual void Load() override;
private:
    int32_t x_;
    int32_t y_;
};