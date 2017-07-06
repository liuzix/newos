//
// Created by Zixiong Liu on 6/28/17.
//

#ifndef MYOS_VFS_H
#define MYOS_VFS_H
#include <boost/container/vector.hpp>

#include "../lib/printf.h"
#include "path.h"


typedef uint32_t offset_t;
void global_filesystem_init ();
class FileHandle;

class VFS {
public:
    Path path;
    virtual FileHandle* file_open(string path) = 0;
    virtual void file_close(FileHandle* handle) = 0;
    virtual int32_t file_read(FileHandle *handle, char* buf, offset_t length) = 0;
    virtual int32_t file_write(FileHandle* handle, char* buf, offset_t length) = 0;
    virtual int32_t file_seek(FileHandle* handle, offset_t offset) = 0;
    virtual offset_t file_size(FileHandle* handle) = 0;
    virtual boost::container::vector<string> file_ls(FileHandle* handle) = 0;

    VFS (const char* _path) : path(_path) {}

};

FileHandle* global_file_open(const char* path);

class FileHandle {
    friend class VFS;
public:
    virtual bool is_null () const = 0;

    void file_close() {
        this->vfs->file_close(this);
    };
    int32_t file_read(char* buf, offset_t length) {
        return this->vfs->file_read(this, buf, length);
    }
    int32_t file_write(char* buf, offset_t length){
        return this->vfs->file_write(this, buf, length);
    };
    int32_t file_seek(offset_t offset) {
        return this->vfs->file_seek(this, offset);
    };

    offset_t file_size() {
        return this->vfs->file_size(this);
    };
    boost::container::vector<string> file_ls() {
        return this->vfs->file_ls(this);
    };
private:
    VFS* vfs;
protected:
    FileHandle (VFS* _vfs) : vfs(_vfs) {}
};



#endif //MYOS_VFS_H
