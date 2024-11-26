#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <stdexcept>

class ini_parser
{
public:
   ini_parser() = delete;
   ini_parser(const ini_parser&) = delete;
   ini_parser(std::string filename);

   template<typename Type>
   Type get_value(const std::string& section_and_value);

private:
   void load_data_from_file(const std::string& filename);
   std::string parse(const std::string& section_var);
   std::string crop(const std::string& str);

   template<typename Type>
   inline Type convert(const std::string& value);


   std::map<std::string, std::map<std::string, std::string>> data_buffer;
};

template<typename Type>
Type ini_parser::get_value(const std::string& section_and_value)
{
   std::string result = parse(section_and_value);

   return convert<Type>(result);
}

template<>
inline int ini_parser::convert<int>(const std::string& value)
{
   return std::stoi(value);
}

template<>
inline double ini_parser::convert<double>(const std::string& value)
{
   return std::stod(value);
}

template<>
inline std::string ini_parser::convert<std::string>(const std::string& value)
{
   return value;
}
