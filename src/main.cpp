#include  <iostream>
#include <filesystem>


#include <boost/locale.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

int main(int, char**) {
	
	boost::locale::generator gen;
	gen.add_messages_domain("hello");
	{
	std::string localeDir = std::filesystem::path(boost::dll::program_location().string()).remove_filename() / "locale";
	gen.add_messages_path(localeDir);
	}

	auto lang = {"en.UTF-8", "es.UTF-8", "ca.UTF-8", "ru.UTF-8"};
	
	for(auto&& l:lang){

		auto g = gen(l);
		std::locale::global(g);
		std::cout.imbue(std::locale());
		std::cout << boost::locale::translate("flashlight") << std::endl;
		for(auto&& n: {1,2,3,4,5}){
			std::cout << boost::locale::format(boost::locale::translate("You have deleted {1} file", "You have deleted {1} files", n)) % n << std::endl;

		}
	}
	return 0;
}
