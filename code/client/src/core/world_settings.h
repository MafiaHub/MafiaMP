#pragma once

#include <map>
#include <string>

// World settings the server hands every client as it joins, before it streams the city in.
namespace MafiaMP::Core::Locations {
    // Location (e.g. "hotel") -> state layer to stream (e.g. "during"); replaces the previous set.
    void SetVariants(const std::map<std::string, std::string> &variants);

    // The package the city stream loads in place of `sdsName`, or an empty string to keep it.
    std::string Redirect(const char *sdsName);
} // namespace MafiaMP::Core::Locations
