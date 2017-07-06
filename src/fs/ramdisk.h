//
// Created by Zixiong Liu on 6/28/17.
//

#ifndef MYOS_RAMDISK_H
#define MYOS_RAMDISK_H
#include <boost/container/vector.hpp>
#include "vfs.h"

using namespace boost::container;
class Ramdisk : public VFS {
    struct FileStruct {
        string filename;
        bool is_dir;
        char* start;
        offset_t length;
    };

    class RamdiskFileHandle : public FileHandle {
        friend class Ramdisk;
        offset_t offset;
        FileStruct* fs;
        RamdiskFileHandle (Ramdisk* _vfs, FileStruct* _fs) : FileHandle(_vfs), fs(_fs), offset(0) {}
    public:
        virtual bool is_null () const {
            return (fs == nullptr);
        }

    };

    boost::container::vector<FileStruct> files;
public:
    Ramdisk (uint64_t start, uint64_t end, const char* _path);
    virtual FileHandle* file_open(string _path);
    virtual void file_close(FileHandle* handle) {
        delete handle;
    };
    virtual int32_t file_read(FileHandle* handle, char* buf, offset_t length);
    virtual int32_t file_write(FileHandle* handle, char* buf, offset_t length){};
    virtual int32_t file_seek(FileHandle* handle, offset_t offset);
    virtual offset_t file_size(FileHandle* handle);
    virtual boost::container::vector<string> file_ls(FileHandle* handle) {};
};

struct tar_header {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};

#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */

#endif //MYOS_RAMDISK_H
