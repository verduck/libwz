#pragma once

#include "wz_property.h"

class WzCanvas : public WzProperty {
public:
    WzCanvas(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff root_property_offset, std::streamoff offset);
    ~WzCanvas();

    void generate_texture(int32_t format, int32_t size, uint8_t* data);

    virtual void Load() override;

    int32_t width() const;
    int32_t height() const;
private:
    int32_t width_;
    int32_t height_;
    int32_t format_;
};