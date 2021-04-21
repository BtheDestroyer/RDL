#pragma once
#include "Backend/pch.h"
#include "Backend/Framework/Debug.h"
#include <string>
#include <map>

// Template argument is only valid for:
//  - unsigned long long
//  - unsigned long
//  - unsigned short
//  - unsigned char
template <typename TIDType = unsigned long long>
class HashedString {
public:
  using ID = TIDType;
  using MapType = std::map<unsigned long long, const std::string>;

  HashedString()
    : m_id(0)
  {
  }

  HashedString(const HashedString& other)
    : m_id(other.m_id)
  {
  }

  DLLMODE explicit HashedString(const std::string& source);

  explicit HashedString(const char* source)
    : HashedString(std::string(source))
  {
  }

  HashedString& operator=(const HashedString& rhs)
  {
    m_id = rhs.m_id;
    return *this;
  }

  template <typename TIDType2>
  bool operator==(const HashedString<TIDType2>& rhs) const
  {
    return m_id == rhs.m_id;
  }

  template <typename TIDType2>
  bool operator!=(const HashedString<TIDType2>& rhs) const
  {
    return m_id != rhs.m_id;
  }

  template <typename TIDType2>
  bool operator>(const HashedString<TIDType2>& rhs) const
  {
    return m_id > rhs.m_id;
  }

  template <typename TIDType2>
  bool operator<(const HashedString<TIDType2>& rhs) const
  {
    return m_id < rhs.m_id;
  }

  operator ID() const
  {
    return GetID();
  }

  operator std::string() const
  {
    return ToString();
  }

  const ID& GetID() const
  {
    return m_id;
  }

  DLLMODE const std::string& ToString() const;

  bool IsValid() const
  {
    return m_id != ID();
  }

private:
  DLLMODE static  ID HashFunction(const std::string& source);

  ID m_id;
  static MapType& s_stringMap;
};

inline HashedString<unsigned long long> operator ""_hash(const char* str, std::size_t)
{
  return HashedString<unsigned long long>(str);
}

inline HashedString<unsigned long long> operator ""_hash_64(const char* str, std::size_t)
{
  return HashedString<unsigned long long>(str);
}

inline HashedString<unsigned long> operator ""_hash_32(const char* str, std::size_t)
{
  return HashedString<unsigned long>(str);
}

inline HashedString<unsigned short> operator ""_hash_16(const char* str, std::size_t)
{
  return HashedString<unsigned short>(str);
}

inline HashedString<unsigned char> operator ""_hash_8(const char* str, std::size_t)
{
  return HashedString<unsigned char>(str);
}
