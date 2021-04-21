#include "Backend/pch.h"
#include "Backend/RDL/RDL.h"

namespace RDL
{
  void Manager::LoadPlugins()
  {
    WIN32_FIND_DATA FileFindData;
    HANDLE hFind = FindFirstFile(TEXT("./Plugins/*.dll"), &FileFindData);
    HINSTANCE hinstDLL;
    if (hFind != INVALID_HANDLE_VALUE)
    {
      do {
        std::wstring filenameW(FileFindData.cFileName);
        filenameW = L"./Plugins/" + filenameW;
#pragma warning(disable:4244)
        std::string filename(filenameW.begin(), filenameW.end());
#pragma warning(default:4244)
        hinstDLL = LoadLibrary(filenameW.c_str());
        if (hinstDLL != nullptr)
        {
          m_loadedDLLs.push_back(hinstDLL);
        }
        else
        {
          std::cerr << "Failed to load plugin: " << filename << "\n";
        }
      } while (FindNextFile(hFind, &FileFindData));
    }
    std::cout << "Loaded " << m_loadedDLLs.size() << " plugins!\n";
    FindClose(hFind);
  }

  Manager& Manager::Get()
  {
    static Manager singleton;
    return singleton;
  }

  Manager::Manager()
    : m_registrationTable()
  {
  }

  Manager::~Manager()
  {
    for (auto& dll : m_loadedDLLs)
    {
      FreeLibrary(dll);
    }
  }

  const std::type_info& Manager::GetObjectType(const ObjectName& name) const
  {
    const auto it = m_registrationTable.find(name);
    if (it != m_registrationTable.end())
    {
      return *it->second.second;
    }
    return typeid(nullptr);
  }

  bool Manager::IsObjectRegistered(const ObjectName& name)
  {
    return m_registrationTable.find(name) != m_registrationTable.end();
  }

  bool Manager::Unregister(const ObjectName& name)
  {
    const auto it = m_registrationTable.find(name);
    if (it != m_registrationTable.end())
    {
      m_registrationTable.erase(it);
      return true;
    }
    return false;
  }

  std::vector<ObjectName> Manager::GetObjectNames() const
  {
    std::vector<ObjectName> r;
    r.reserve(m_registrationTable.size());
    for (const auto& it : m_registrationTable)
    {
      r.push_back(it.first);
    }
    return r;
  }

  std::size_t Manager::GetLoadedPluginCount() const
  {
    return m_loadedDLLs.size();
  }
}
