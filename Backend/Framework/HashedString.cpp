#include "Backend/pch.h"
#include "Backend/Framework/Debug.h"
#include "Backend/Framework/HashedString.h"
#include <string>
#include <map>

static HashedString<unsigned long long>::MapType s_globalStringMap;
template<>
HashedString<unsigned long long>::MapType& HashedString<unsigned long long>::s_stringMap = s_globalStringMap;
template<>
HashedString<unsigned long long>::MapType& HashedString<unsigned long>::s_stringMap      = s_globalStringMap;
template<>
HashedString<unsigned long long>::MapType& HashedString<unsigned short>::s_stringMap     = s_globalStringMap;
template<>
HashedString<unsigned long long>::MapType& HashedString<unsigned char>::s_stringMap      = s_globalStringMap;

template<>
HashedString<unsigned long long>::ID HashedString<unsigned long long>::HashFunction(const std::string& source)
{
  // "random" starting number
  ID r = 1435958502956573194;
  // sdbm hash
  const char* str = source.data();
  while (*str)
  {
    r = *str++ + (r << 6) + (r << 16) - r;
  }
  DEBUG_ONLY(ID start = r);
  while (s_stringMap.find(r) != s_stringMap.end())
  {
    if (s_stringMap[r] == source)
    {
      return r; // Occupied slot is for this string
    }
    ++r; // Bump hash until an empty spot is found
    DEBUG_REQUIRE_TRUE(start != r); // s_stringMap is full!
  }
  return r;
}

template<>
HashedString<unsigned long>::ID HashedString<unsigned long>::HashFunction(const std::string& source)
{
  // "random" starting number
  ID r = 2163057162;
  // sdbm hash
  const char* str = source.data();
  while (*str)
  {
    r = *str++ + (r << 3) + (r << 8) - r;
  }
  DEBUG_ONLY(ID start = r);
  while (s_stringMap.find(r) != s_stringMap.end())
  {
    if (s_stringMap[r] == source)
    {
      return r; // Occupied slot is for this string
    }
    ++r; // Bump hash until an empty spot is found
    DEBUG_REQUIRE_TRUE(start != r); // s_stringMap is full!
  }
  return r;
}

template<>
HashedString<unsigned short>::ID HashedString<unsigned short>::HashFunction(const std::string& source)
{
  bool odd = false;
  // "random" starting number
  ID r = 41482;
  // sdbm hash
  const char* str = source.data();
  while (*str)
  {
    r = *str++ + (r << (odd ? 2 : 3)) + (r << 4) - r;
    odd = !odd;
  }
  DEBUG_ONLY(ID start = r);
  while (s_stringMap.find(r) != s_stringMap.end())
  {
    if (s_stringMap[r] == source)
    {
      return r; // Occupied slot is for this string
    }
    ++r; // Bump hash until an empty spot is found
    DEBUG_REQUIRE_TRUE(start != r); // s_stringMap is full!
  }
  return r;
}

template<>
HashedString<unsigned char>::ID HashedString<unsigned char>::HashFunction(const std::string& source)
{
  unsigned char shift = 1;
  // "random" starting number
  ID r = 10;
  // sdbm hash
  const char* str = source.data();
  while (*str)
  {
    // the 'shift' part here looks complicated, it's not
    // it's just mapping:
    //   1->1
    //   2->1
    //   3->2
    // this is to properly split shifting by 2 and 3 bits from the `unsigned short` implementation
    r = *str++ + (r << ((shift & 1) + ((shift >> 1) & 1))) + (r << 2) - r;
    shift = shift * (shift < 2) + 1;
  }
  DEBUG_ONLY(ID start = r);
  while (s_stringMap.find(r) != s_stringMap.end())
  {
    if (s_stringMap[r] == source)
    {
      return r; // Occupied slot is for this string
    }
    ++r; // Bump hash until an empty spot is found
    DEBUG_REQUIRE_TRUE(start != r); // s_stringMap is full!
  }
  return r;
}

template<>
HashedString<unsigned long long>::HashedString(const std::string& source)
  : m_id(HashFunction(source))
{
  DEBUG_REQUIRE_TRUE(s_stringMap.find(m_id) == s_stringMap.end() || s_stringMap.at(m_id) == source); // Hash conflict!
  if (s_stringMap.find(m_id) == s_stringMap.end())
  {
    s_stringMap.emplace(m_id, source);
  }
}

template<>
HashedString<unsigned long>::HashedString(const std::string& source)
  : m_id(HashFunction(source))
{
  DEBUG_REQUIRE_TRUE(s_stringMap.find((unsigned long long)(m_id)) == s_stringMap.end() || s_stringMap.at((unsigned long long)(m_id)) == source); // Hash conflict!
  if (s_stringMap.find((unsigned long long)(m_id)) == s_stringMap.end())
  {
    s_stringMap.emplace((unsigned long long)(m_id), source);
  }
}

template<>
HashedString<unsigned short>::HashedString(const std::string& source)
  : m_id(HashFunction(source))
{
  DEBUG_REQUIRE_TRUE(s_stringMap.find((unsigned long long)(m_id)) == s_stringMap.end() || s_stringMap.at((unsigned long long)(m_id)) == source); // Hash conflict!
  if (s_stringMap.find((unsigned long long)(m_id)) == s_stringMap.end())
  {
    s_stringMap.emplace((unsigned long long)(m_id), source);
  }
}

template<>
HashedString<unsigned char>::HashedString(const std::string& source)
  : m_id(HashFunction(source))
{
  DEBUG_REQUIRE_TRUE(s_stringMap.find((unsigned long long)(m_id)) == s_stringMap.end() || s_stringMap.at((unsigned long long)(m_id)) == source); // Hash conflict!
  if (s_stringMap.find((unsigned long long)(m_id)) == s_stringMap.end())
  {
    s_stringMap.emplace((unsigned long long)(m_id), source);
  }
}

template<>
const std::string& HashedString<unsigned long long>::ToString() const
{
  DEBUG_REQUIRE_TRUE(IsValid()); // No string representation for invalid HashedStrings
  DEBUG_REQUIRE_TRUE(s_stringMap.find(m_id) != s_stringMap.end()); // Requested ID hasn't been hashed before
  return s_stringMap.at(m_id);
}

template<>
const std::string& HashedString<unsigned long>::ToString() const
{
  DEBUG_REQUIRE_TRUE(IsValid()); // No string representation for invalid HashedStrings
  DEBUG_REQUIRE_TRUE(s_stringMap.find((unsigned long long)(m_id)) != s_stringMap.end()); // Requested ID hasn't been hashed before
  return s_stringMap.at((unsigned long long)(m_id));
}

template<>
const std::string& HashedString<unsigned short>::ToString() const
{
  DEBUG_REQUIRE_TRUE(IsValid()); // No string representation for invalid HashedStrings
  DEBUG_REQUIRE_TRUE(s_stringMap.find((unsigned long long)(m_id)) != s_stringMap.end()); // Requested ID hasn't been hashed before
  return s_stringMap.at((unsigned long long)(m_id));
}

template<>
const std::string& HashedString<unsigned char>::ToString() const
{
  DEBUG_REQUIRE_TRUE(IsValid()); // No string representation for invalid HashedStrings
  DEBUG_REQUIRE_TRUE(s_stringMap.find((unsigned long long)(m_id)) != s_stringMap.end()); // Requested ID hasn't been hashed before
  return s_stringMap.at((unsigned long long)(m_id));
}
