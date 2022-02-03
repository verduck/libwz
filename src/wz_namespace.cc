#include "stdafx.h"
#include "wz_namespace.h"
#include "wz_property.h"
#include "string_utils.h"

WzNamespace::WzNamespace(const std::unique_ptr<WzStream>& stream, const std::string& name, std::streamoff offset) :
    WzNamespace(stream, nullptr, name, offset) {
}

WzNamespace::WzNamespace(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset) :
    stream_(stream),
    parent_(parent),
    name_(name),
    offset_(offset),
    is_loaded_(false) {
}

WzNamespace::~WzNamespace() {}

std::string WzNamespace::name() const {
    return name_;
}

const std::unique_ptr<WzStream>& WzNamespace::stream() const {
    return stream_;
}

std::streamoff WzNamespace::offset() const {
    return offset_;
}

void WzNamespace::set_offset(std::streamoff offset) {
    offset_ = offset;
}

const std::shared_ptr<WzNamespace>& WzNamespace::parent() const {
    return parent_;
}

bool WzNamespace::is_loaded() const {
    return is_loaded_;
}

void WzNamespace::set_loaded(bool loaded) {
    is_loaded_ = loaded;
}

void WzNamespace::AddChild(const std::string& name, const std::shared_ptr<WzNamespace>& item) {
    children_.emplace(name, item);
}

const std::unordered_map<std::string, std::shared_ptr<WzNamespace>>& WzNamespace::children() const {
    return children_;
}

std::shared_ptr<WzNamespace> WzNamespace::GetChild(const std::string& name) {
    if (!is_loaded_) {
        Load();
    }
    if (name == "..") {
        return parent();
    }
    auto iter = children_.find(name);
    if (iter == children_.end()) {
        return nullptr;
    }
    
    return iter->second;
}

std::shared_ptr<WzNamespace> WzNamespace::GetChildFromPath(const std::string& path) {
    std::shared_ptr<WzNamespace> child = shared_from_this();
    std::vector<std::string> splits = StringUtils::split(path, '/');
    for (const std::string& str : splits) {
        child = child->GetChild(str);
    }

    return child;
}

std::string WzNamespace::GetFullpath() {
    std::string path;
    WzNamespace *cur = this;
    while (cur != nullptr) {
        path.insert(0, "/");
        path.insert(0, cur->name());
        cur = cur->parent().get();
    }

    path.pop_back();

    return path;
}