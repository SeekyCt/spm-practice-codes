#include <common.h>
#include <wii/os.h>

#include <sys/iosupport.h>

namespace clibcompat {

static ssize_t stdio_write_r(struct _reent *r, void *fd, const char *ptr, size_t len)
{
    (void) r;
    (void) fd;

    wii::os::OSReport(ptr);

    return len;
}

static const devoptab_t dotab_stdio =
{
    .name = "stdio",
    .structSize = 0,

    .open_r = nullptr,
    .close_r = nullptr,
    .write_r = stdio_write_r,
    .read_r = nullptr,
    .seek_r = nullptr,
    .fstat_r = nullptr,
    .stat_r = nullptr,
    .link_r = nullptr,
    .unlink_r = nullptr,
    .chdir_r = nullptr,
    .rename_r = nullptr,
    .mkdir_r = nullptr,

    .dirStateSize = 0,
    .diropen_r = nullptr,
    .dirreset_r = nullptr,
    .dirnext_r = nullptr,
    .dirclose_r = nullptr,
    .statvfs_r = nullptr,
    .ftruncate_r = nullptr,
    .fsync_r = nullptr,

    .deviceData = nullptr,

    .chmod_r = nullptr,
    .fchmod_r = nullptr,
    .rmdir_r = nullptr,
    .lstat_r = nullptr,
    .utimes_r = nullptr,

    .fpathconf_r = nullptr,
    .pathconf_r = nullptr,

    .symlink_r = nullptr,
    .readlink_r = nullptr
};

void stdioPatch()
{
    devoptab_list[STD_OUT] = &dotab_stdio;
    devoptab_list[STD_ERR] = &dotab_stdio;
}

}
