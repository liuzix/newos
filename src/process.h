#ifndef MYOS_PROCESS_H
#define MYOS_PROCESS_H

#include <boost/container/vector.hpp>
#include <map>
#include "fs/vfs.h"

using namespace boost::container;
namespace user {

struct user_file_handle {
    int fd;
    FileHandle* file_handle;
};

struct user_proc_data{
    vector<user_file_handle> file_descriptors;

};

}

#endif