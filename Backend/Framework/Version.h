#pragma once
#include <json.hpp>

struct Version {
  std::string version;
  std::vector<std::string> changelog;
  std::string downloadPathRoot;
  std::vector<std::string> downloadFiles;
  std::string executablePath;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Version, version, changelog, downloadPathRoot, downloadFiles, executablePath);
