#include "stdafx.h"
#include "wz_vector2d.h"

WzVector2D::WzVector2D(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset) :
    WzNamespace(stream, parent, name, offset) {
    
    Load();
}

WzVector2D::~WzVector2D() {}

void WzVector2D::Load() {
    stream()->seek(offset());
    x_ = stream()->ReadCompressedInt();
    y_ = stream()->ReadCompressedInt();
    set_loaded(true);
}

int32_t WzVector2D::x() const {
    return x_;
}

int32_t WzVector2D::y() const {
    return y_;
}