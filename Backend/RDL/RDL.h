#pragma once
#include "Backend/pch.h"
#include "Backend/Framework/HashedString.h"

namespace RDL
{
  using ObjectName         = HashedString<unsigned long long>;
  using ObjectPtr          = void*;
  using ObjectRegistration = std::pair<ObjectPtr, const std::type_info*>;

  class DLLMODE Manager
  {
  public:

    void LoadPlugins();

    template <typename TObjectType>
    bool Register(TObjectType* object, ObjectName name)
    {
      const auto it = m_registrationTable.find(name);
      if (it == m_registrationTable.end())
      {
        ObjectRegistration registration = std::make_pair(static_cast<ObjectPtr>(object), &typeid(TObjectType));
        m_registrationTable[name] = registration;
        return m_registrationTable.at(name) == registration;
      }
      return false;
    }

    template <typename TObjectType>
    bool Register(TObjectType& object, ObjectName name)
    {
      return Register(&object, name);
    }

    template <typename TObjectType>
    const TObjectType* TryGetPtr(ObjectName name) const
    {
      const auto it = m_registrationTable.find(name);
      if (it != m_registrationTable.end())
      {
        if (*it->second.second != typeid(TObjectType))
        {
          // Type mismatch between registration and retrieval
          return nullptr;
        }
        return static_cast<TObjectType*>(it->second.first);
      }
      return nullptr;
    }

    template <typename TObjectType>
    TObjectType* TryGetMutablePtr(ObjectName name) const
    {
      const auto it = m_registrationTable.find(name);
      if (it != m_registrationTable.end())
      {
        if (*it->second.second != typeid(TObjectType))
        {
          // Type mismatch between registration and retrieval
          return nullptr;
        }
        return static_cast<TObjectType*>(it->second.first);
      }
      return nullptr;
    }

    template <typename TObjectType>
    const TObjectType* ForceGetPtr(ObjectName name) const
    {
      const auto it = m_registrationTable.find(name);
      if (it != m_registrationTable.end())
      {
        return static_cast<TObjectType*>(it->second.first);
      }
      return nullptr;
    }

    template <typename TObjectType>
    TObjectType* ForceGetMutablePtr(ObjectName name) const
    {
      const auto it = m_registrationTable.find(name);
      if (it != m_registrationTable.end())
      {
        return static_cast<TObjectType*>(it->second.first);
      }
      return nullptr;
    }

    template <typename TObjectType>
    const TObjectType& GetRef(ObjectName name) const
    {
      return *ForceGetPtr<const TObjectType>(name);
    }

    template <typename TObjectType>
    TObjectType& GetMutableRef(ObjectName name) const
    {
      return *ForceGetMutablePtr<TObjectType>(name);
    }

    const std::type_info& GetObjectType(const ObjectName& name) const;

    bool IsObjectRegistered(const ObjectName& name);
    bool Unregister(const ObjectName& name);

    std::vector<ObjectName> GetObjectNames() const;
    std::size_t GetLoadedPluginCount() const;

    static Manager& Get();
    ~Manager();

  protected:
    Manager();

  private:
    std::map<ObjectName, ObjectRegistration> m_registrationTable;
    std::vector<HINSTANCE> m_loadedDLLs;
  };

  template <typename T>
  class Object
  {
  public:
    Object(const ObjectName& name)
      : m_name(name)
    {
    }

    operator T&()
    {
      return Manager::Get().GetMutableRef<T>(m_name);
    }

    operator const T&() const
    {
      return Manager::Get().GetRef<T>(m_name);
    }

    T* operator&()
    {
      return Manager::Get().TryGetMutablePtr<T>(m_name);
    }

    const T* operator&() const
    {
      return Manager::Get().TryGetPtr<T>(m_name);
    }

    T operator*()
    {
      return operator T&();
    }

    const T operator*() const
    {
      return operator const T&();
    }

    bool IsValid() const
    {
      return Manager::Get().IsObjectRegistered(m_name);
    }

  private:
    ObjectName m_name;
  };
}
