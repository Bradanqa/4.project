#include "ini_parser.h"


ini_parser::ini_parser(std::string filename)
{
   load_data_from_file(filename);
}


void ini_parser::load_data_from_file(const std::string& filename)
{
   std::ifstream file(filename);
   
   if (!file.is_open())
   {
      throw std::runtime_error("Не удалось открыть файл: " + filename);
   }

   int line_number = 0;
   std::string line;
   std::string current_section;

   while (std::getline(file, line))
   {
      line_number++;

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

         if (equal_sign == std::string::npos || equal_sign == 0)
         {
            throw std::runtime_error("Ошибка в строке #" + std::to_string(line_number) + ": " + line);
         }

         std::string key = crop(line.substr(0, equal_sign));
         std::string value = crop(line.substr(equal_sign + 1));

         if (!current_section.empty())
         {
            data_buffer[current_section][key] = value;
         }
         else
         {
            throw std::runtime_error("Ошибка в строке #" + std::to_string(line_number) + ": " + line);
         }
      }
   }

   file.close();
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
      throw std::runtime_error(section + " не найдена или не содержит значений");
   }

   auto var_iter = section_iter->second.find(var);
   if (var_iter == section_iter->second.end())
   {
      std::cout << "Доступные переменные в этой секции:" << std::endl;

      var_iter = section_iter->second.begin();
      while (var_iter != section_iter->second.end())
      {
         std::cout << var_iter->first << std::endl;
         var_iter++;
      }

      throw std::runtime_error("Переменная не найдена " + section + ": " + var);
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
