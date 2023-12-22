#include <common.h>

#define MAX_MAGNITUDE 80.0f
namespace mod::gcpadpatches
{

extern "C" {
    void marioUpdateKeyDataPatch();
}

} // namespace mod::gcpadpatches
