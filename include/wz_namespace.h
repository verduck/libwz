#pragma once

#include <unordered_map>
#include "wz_stream.h"

class WzNamespace : public std::enable_shared_from_this<WzNamespace> {
public:
    WzNamespace(const std::unique_ptr<WzStream>& stream, const std::string& name, std::streamoff offset);
    WzNamespace(const std::unique_ptr<WzStream>& stream, const std::shared_ptr<WzNamespace>& parent, const std::string& name, std::streamoff offset);
    virtual ~WzNamespace();

    std::string name() const;
    const std::unordered_map<std::string, std::shared_ptr<WzNamespace>>& children() const; 
    std::shared_ptr<WzNamespace> GetChild(const std::string& name);
    std::shared_ptr<WzNamespace> GetChildFromPath(const std::string& path);
    std::string GetFullpath();

    virtual void Load() = 0;
protected:
    const std::unique_ptr<WzStream>& stream() const;
    std::streamoff offset() const;
    void set_offset(std::streamoff offset);
    const std::shared_ptr<WzNamespace>& parent() const;
    bool is_loaded() const;
    void set_loaded(bool loaded);
    void AddChild(const std::string& name, const std::shared_ptr<WzNamespace>& item);
private:
    const std::unique_ptr<WzStream>& stream_;
    std::shared_ptr<WzNamespace> parent_;
    std::string name_;
    std::streamoff offset_;
    bool is_loaded_;
    std::unordered_map<std::string, std::shared_ptr<WzNamespace>> children_;
};