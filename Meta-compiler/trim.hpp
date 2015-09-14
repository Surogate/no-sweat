#ifndef TRIM_HPP
#define TRIM_HPP

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

// trim from start
template <typename T> static inline auto ltrim(T&& s) -> decltype(s)
{
   s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                          std::not1(std::ptr_fun<int, int>(std::isspace))));
   return std::move(s);
}

// trim from end
template <typename T> static inline auto rtrim(T&& s) -> decltype(s)
{
   s.erase(std::find_if(s.rbegin(), s.rend(),
               std::not1(std::ptr_fun<int, int>(std::isspace)))
               .base(),
       s.end());
   return std::move(s);
}

// trim from both ends
template <typename T> static inline auto trim(T&& s) -> decltype(s)
{
   return std::move(ltrim(rtrim(s)));
}

template <typename T> static inline auto remove_quote(T&& s) -> decltype(s)
{
   if(s.size())
   {
      if(s.back() == '"')
         s.pop_back();
      if(s.front() == '"' && s.size() == 1)
      {
         s.clear();
      }
      if(s.front() == '"')
      {
         for(std::size_t i = 1; i < s.size(); ++i)
         {
            s[i - 1] = s[i];
         }
         s.pop_back();
      }
   }
   return std::move(s);
}

template <typename T> static inline auto add_quote(T&& s) -> decltype(s)
{
   if(s.size())
   {
      if(s.back() != '"')
      {
         s.push_back('"');
      }
      if(s.front() != '"')
      {
         s = "\"" + s;
      }
   }
   else
   {
      s = "\"\"";
   }

   return std::move(s);
}

#endif //! TRIM_HPP
