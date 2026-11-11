#include <utils/safe_win32.h>

#include "world_settings.h"

#include <cstring>
#include <filesystem>
#include <mutex>

// A story location streams as a base layer, lh_loc_<name>_nostate_* (its outside), and a state
// layer on top of it: _before_ in free roam, _during_ in its mission (its interior), _after_. Its
// variant swaps the state layer the city stream loads.
namespace MafiaMP::Core::Locations {
    namespace {
        std::mutex gMutex;
        std::map<std::string, std::string> gVariants; // "lh_loc_<name>" -> state layer

        // "/sds/city/x.sds" exists as "<game>/sds_retail/city/x.sds".
        bool PackageExists(const std::string &sdsPath) {
            if (sdsPath.rfind("/sds/", 0) != 0) {
                return false;
            }
            static const auto root = [] {
                wchar_t exe[MAX_PATH] = {};
                GetModuleFileNameW(nullptr, exe, MAX_PATH);
                return std::filesystem::path(exe).parent_path() / "sds_retail";
            }();
            return std::filesystem::exists(root / sdsPath.substr(5));
        }
    } // namespace

    void SetVariants(const std::map<std::string, std::string> &variants) {
        std::lock_guard lock(gMutex);
        gVariants.clear();
        for (const auto &[location, variant] : variants) {
            gVariants["lh_loc_" + location] = variant;
        }
    }

    std::string Redirect(const char *sdsName) {
        if (!sdsName || !std::strstr(sdsName, "lh_loc_")) {
            return {};
        }
        const std::string path(sdsName);
        std::lock_guard lock(gMutex);
        for (const auto &[location, variant] : gVariants) {
            for (const char *layer : {"_before_", "_during_", "_after_"}) {
                const auto at = path.find(location + layer);
                if (at == std::string::npos) {
                    continue;
                }
                std::string target = path;
                target.replace(at + location.size(), std::strlen(layer), "_" + variant + "_");
                if (target != path && PackageExists(target)) {
                    return target;
                }
            }
        }
        return {};
    }
} // namespace MafiaMP::Core::Locations
