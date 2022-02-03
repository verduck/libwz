#pragma once

#include "wz_namespace.h"

template<typename T>
class WzVariant : public WzNamespace {
public:
    WzVariant(const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset, T value);

    T value() const;

    virtual void Load() override;
private:
    T value_;
};

template<typename T>
WzVariant<T>::WzVariant(const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset, T value) :
    WzNamespace(nullptr, parent, name, offset),
    value_(value) {

}

template<typename T>
T WzVariant<T>::value() const {
    return value_;
}

template<typename T>
void WzVariant<T>::Load() {
    set_loaded(true);
}