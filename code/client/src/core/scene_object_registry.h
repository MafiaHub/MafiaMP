#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

namespace MafiaMP::Core {
    struct SceneObjectInfo {
        std::string debugPath;
        std::string category;
    };

    class SceneObjectRegistry {
      private:
        std::unordered_map<uint64_t, SceneObjectInfo> _registry;
        mutable std::mutex _mutex;

        static std::string ExtractCategory(const std::string &path);

      public:
        void Register(uint64_t hashName, const std::string &debugPath);
        void Unregister(uint64_t hashName);

        std::unordered_map<uint64_t, SceneObjectInfo> &GetRegistry() {
            return _registry;
        }

        std::mutex &GetMutex() const {
            return _mutex;
        }

        size_t GetCount() const {
            return _registry.size();
        }
    };

    extern SceneObjectRegistry gSceneObjectRegistry;
} // namespace MafiaMP::Core
