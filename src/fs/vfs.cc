//
// Created by Zixiong Liu on 6/30/17.
//

#include "vfs.h"
#include "../multiboot.h"
#include "ramdisk.h"
#include <boost/container/vector.hpp>

using Vec = boost::container::vector<VFS*>;
static Vec* mounted_filesystems = nullptr;

void global_filesystem_init () {
    mounted_filesystems = new boost::container::vector<VFS*>();
    // load the initrd
    VFS *ramdisk = new Ramdisk(multiboot::ramdisk_start, multiboot::ramdisk_end, "/ramdisk");
    mounted_filesystems->push_back(ramdisk);
}

FileHandle *global_file_open(const char *path) {
    Path full_path {path};
    for (VFS* iter: *mounted_filesystems) {
        if (iter->path.contains(full_path)) {
            return iter->file_open(path);
        }
    }
    return nullptr;
}
