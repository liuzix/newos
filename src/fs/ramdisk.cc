//
// Created by Zixiong Liu on 6/28/17.
//

#include "ramdisk.h"
#include "../lib/printf.h"

static unsigned int getsize(const char *in);

Ramdisk::Ramdisk(uint64_t start, uint64_t end, const char* _path) : VFS(_path) {
    tar_header* header_ptr = reinterpret_cast<tar_header*>(start);
    for (;;) {
        if (header_ptr->filename[0] == '\0')
            break;
        FileStruct fs;
        fs.filename = header_ptr->filename;
        if (header_ptr->typeflag[0] == AREGTYPE || header_ptr->typeflag[0] == REGTYPE) {
            fs.length = getsize(header_ptr->size);
            fs.start = (char *) ((((uint64_t) header_ptr) & (~511)) + 512);
            fs.is_dir = false;
            kprintf("filename = %s, size = %d\n", fs.filename.data(), fs.length);
            header_ptr = (tar_header *) ((((uint64_t) fs.start + fs.length) & (~511)) + 512);
        } else if (header_ptr->typeflag[0] == DIRTYPE) {
            fs.length = 0;
            fs.start = nullptr;
            fs.is_dir = true;
            kprintf("directory name = %s\n", fs.filename.data());
            header_ptr = (tar_header *) ((uint64_t) header_ptr + 512);
        } else {
            kprintf("Unsupported tar file type\n");
            assert(false);
        }
        files.push_back(fs);
    }
}

FileHandle* Ramdisk::file_open(string _path) {
    Path to_open(_path);
    for (auto& fs : files) {
        Path temp_path(fs.filename);
        auto full_path = this->path + temp_path;
        if (full_path == to_open) {
            return new RamdiskFileHandle(this, &fs);
        }
    }
    return new RamdiskFileHandle(this, nullptr);
}

int32_t Ramdisk::file_read(FileHandle* handle, char *buf, offset_t length) {
    auto my_handle = static_cast<RamdiskFileHandle*>(handle);
    offset_t copy_length = length < (my_handle->fs->length - my_handle->offset)
                           ? length : (my_handle->fs->length - my_handle->offset);
    memcpy(buf, my_handle->fs->start + my_handle->offset, copy_length);
    my_handle->offset += copy_length;
    return copy_length;
}

offset_t Ramdisk::file_size(FileHandle *handle) {
    auto my_handle = static_cast<RamdiskFileHandle*>(handle);
    return my_handle->fs->length;
}

int32_t Ramdisk::file_seek(FileHandle *handle, offset_t offset) {
    auto my_handle = static_cast<RamdiskFileHandle*>(handle);
    if (offset < my_handle->fs->length) {
        my_handle->offset = offset;
        return offset;
    } else {
        return my_handle->fs->length;
    }
}


static unsigned int getsize(const char *in) {

    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;

}