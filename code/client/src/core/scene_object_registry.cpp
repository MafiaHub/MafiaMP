#include "scene_object_registry.h"

namespace MafiaMP::Core {
    SceneObjectRegistry gSceneObjectRegistry;

    std::string SceneObjectRegistry::ExtractCategory(const std::string &path) {
        // Path format: "sds/category/subcategory/name.sds" or similar
        // Extract the first meaningful directory as category

        size_t start = 0;

        // Skip common prefixes
        if (path.rfind("sds/", 0) == 0) {
            start = 4;
        }

        // Find the next slash to get the category
        size_t end = path.find('/', start);
        if (end == std::string::npos) {
            // No slash found, use the whole remaining string (minus extension)
            size_t dot = path.rfind('.');
            if (dot != std::string::npos && dot > start) {
                return path.substr(start, dot - start);
            }
            return path.substr(start);
        }

        return path.substr(start, end - start);
    }

    void SceneObjectRegistry::Register(uint64_t hashName, const std::string &debugPath) {
        std::lock_guard<std::mutex> lock(_mutex);
        _registry[hashName] = {debugPath, ExtractCategory(debugPath)};
    }

    void SceneObjectRegistry::Unregister(uint64_t hashName) {
        std::lock_guard<std::mutex> lock(_mutex);
        _registry.erase(hashName);
    }
} // namespace MafiaMP::Core
