#include "ini_parser.h"


int main()
{
   setlocale(LC_ALL, "ru-RU.UTF-8");

   try
   {
      ini_parser parser("file2.ini");
      auto value = parser.get_value<int>("Section2.var1");

      std::cout << "Значение: " << value << std::endl;
   }
   catch (const std::exception& error)
   {
      std::cerr << "Ошибка: " << error.what() << std::endl;
   }

   return 0;
}