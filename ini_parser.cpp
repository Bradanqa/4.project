#include "ini_parser.h"


ini_parser::ini_parser(std::string filename)
{
   load_data_from_file(filename);

   if (!validate_syntax())
   {
      throw std::runtime_error("Ошибочный синтаксис");
   }
}


void ini_parser::load_data_from_file(const std::string& filename)
{
   std::ifstream file(filename);
   
   if (!file.is_open())
   {
      throw std::runtime_error("Не удалось открыть файл: " + filename);
   }

   std::string line;
   std::string current_section;

   while (std::getline(file, line))
   {
      line = crop(line);

      if (line.empty() || line[0] == ';')
      {
         continue;
      }

      if (line.front() == '[' && line.back() == ']')
      {
         current_section = crop(line.substr(1, line.size() - 2));
      }
      else
      {
         auto equal_sign = line.find('=');

         if (equal_sign == std::string::npos)
         {
            throw std::runtime_error("Неверная переменная: " + line);
         }

         std::string key = crop(line.substr(0, equal_sign));
         std::string value = crop(line.substr(equal_sign + 1));

         if (!current_section.empty())
         {
            data_buffer[current_section][key] = value;
         }
         else
         {
            throw std::runtime_error("Неверное : " + line);
         }
      }
   }

   file.close();
}

bool ini_parser::validate_syntax()
{

   // TODO: проверка на корректность синтаксиса

   return !data_buffer.empty();
}

std::string ini_parser::parse(const std::string& section_var)
{
   auto delimiter_pos = section_var.find('.');

   if (delimiter_pos == std::string::npos)
   {
      throw std::runtime_error("Неверный формат запроса: " + section_var);
   }

   std::string section = section_var.substr(0, delimiter_pos);
   std::string var = section_var.substr(delimiter_pos + 1);

   auto section_iter = data_buffer.find(section);
   if (section_iter == data_buffer.end())
   {
      throw std::runtime_error("Section не найдена: " + section);
   }

   auto var_iter = section_iter->second.find(var);
   if (var_iter == section_iter->second.end())
   {
      throw std::runtime_error("Переменная не найдена в секции " + section + ": " + var);
   }

   return var_iter->second;
}

std::string ini_parser::crop(const std::string& str)
{
   auto start = std::find_if(str.begin(), str.end(), [](unsigned char temp)
      { return !std::isspace(temp); });

   auto end = std::find_if(str.rbegin(), str.rend(), [](unsigned char temp)
      { return !std::isspace(temp); }).base();
   
   if (start >= end)
   {
      return "";
   }
   
   return std::string(start, end);
}


template<>
int ini_parser::convert<int>(const std::string& value)
{
   return std::stoi(value);
}

template<>
double ini_parser::convert<double>(const std::string& value)
{
   return std::stod(value);
}

template<>
std::string ini_parser::convert<std::string>(const std::string& value)
{
   return value;
}
