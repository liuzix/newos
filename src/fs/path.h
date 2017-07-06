//
// Created by Zixiong Liu on 6/29/17.
//
#include <boost/algorithm/string.hpp>
#include <boost/container/string.hpp>
#ifndef MYOS_PATH_H
#define MYOS_PATH_H

using namespace boost::container;
struct Path {
    boost::container::vector<string> strs;
    string get_string_path() {
        string str = "";

        for (auto &cur: strs) {
            str += "/";
            str += cur;
        }
        return str;
    }

    Path(string str) {
        //kprintf("path = %s\n", str.data());
        boost::split(strs, str, boost::is_any_of("/"));
    }

    Path() {}

    Path operator+(const Path &other) const {
        Path ret;
        ret.strs.insert(ret.strs.end(), this->strs.begin(), this->strs.end());
        ret.strs.insert(ret.strs.end(), other.strs.begin(), other.strs.end());
        return ret;
    }

    bool operator==(const Path &other) const {
        if (this->strs.size() != other.strs.size()) return false;
        for (int i = 0; i < this->strs.size(); i++) {
            if (this->strs[i] != other.strs[i]) return false;
        }
        return true;
    }

    bool contains(const Path &other) const {
        if (this->strs.size() > other.strs.size()) return false;
        for (int i = 0; i < this->strs.size(); i++) {
            if (this->strs[i] != other.strs[i]) return false;
        }
        return true;
    }



};

#endif //MYOS_PATH_H
