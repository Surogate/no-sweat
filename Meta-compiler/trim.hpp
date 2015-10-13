#ifndef TRIM_HPP
#define TRIM_HPP

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <type_traits>

// trim from start
template <typename T> static inline T ltrim(T&& s)
{
   s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                          std::not1(std::ptr_fun<int, int>(std::isspace))));
   return std::move(s);
}

// trim from end
template <typename T> static inline T rtrim(T&& s)
{
   s.erase(std::find_if(s.rbegin(), s.rend(),
               std::not1(std::ptr_fun<int, int>(std::isspace)))
               .base(),
       s.end());
   return std::move(s);
}

// trim from both ends
template <typename T> static inline T trim(T&& s)
{
   return std::move(ltrim(rtrim(s)));
}

template <typename T> static inline T remove_quote(T&& s)
{
   s = trim(s);
   typedef typename std::remove_reference<T>::type::value_type value_type;
   auto quote_char = value_type('"');
   if(s.size())
   {
      if(s.back() == quote_char)
         s.pop_back();
      if(s.front() == quote_char && s.size() == 1)
      {
         s.clear();
      }
      if(s.front() == quote_char)
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

template <typename T> static inline T add_quote(T&& s)
{
   s = trim(s);
   typedef typename std::remove_reference<T>::type::value_type value_type;
   auto quote_char = value_type('"');
   if(s.size())
   {
      if(s.back() != quote_char)
      {
         s.push_back(quote_char);
      }
      if(s.front() != quote_char)
      {
         s = quote_char + s;
      }
   }
   else
   {
      s.resize(2);
      s[0] = quote_char;
      s[1] = quote_char;
   }

   return std::move(s);
}

#endif //! TRIM_HPP
