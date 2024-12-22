#include <spm/system.h>

extern "C" {

void __syscall_assert_func(const char * filename, int line, const char * function, const char *failedexpr)
{
    spm::system::__assert2(filename, line, failedexpr, function);
}

}