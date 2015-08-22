
#include <functional>
#include <iostream>
#include <clocale>


#include "jvm.h"
#include "class_file.h"
#include "options.h"
#include "exception.h"

extern void dump_class_file(std::string &classfile);

int main(int, char** args)
{
    std::setlocale(LC_ALL, "");
    
    try {
        ujvm::options_t options(args);
        if (!options.parse()) {
            return 0;
        }

        if (options.dumpclassfile) {
            dump_class_file(options.classfile);
            return 0;
        }
        
        if (options.classversion) {
            ujvm::class_file_t file(options);
            auto version = file.version();
            std::cout << options.classfile + " : " << std::endl;
            std::cout << "major : " << std::get<0>(version) << std::endl;
            std::cout << "minor : " << std::get<1>(version) << std::endl;
            return 0;
        }

        ujvm::jvm_t jvm(options);
        jvm.run();

    } catch (ujvm::exception_t& ex) {
        std::cout << ex.what() << std::endl;
    }
    
    return 0;
}
