//
// Created by Zixiong Liu on 12/16/16.
//

#ifndef MYOS_BITMAP_H
#define MYOS_BITMAP_H


#include <stddef.h>
#include <stdint.h>
#include "printf.h"
#include "assert.h"


struct bitmap {
    size_t capacity;

    bitmap (size_t len) {
      capacity = len;
      //data = _data;
    }

    bool get(const size_t index) const {
      size_t byte_pos = index >> 3;
      uint8_t byte = data[byte_pos];
      bool ret = (byte >> (index % 8)) & 1;
      //printf("gua!\n");
      //printf("index = %d, data = %d, byte = %d", index, &data[byte_pos], byte);
      return ret;
    }

    bool set(const size_t index, bool val) {
      size_t byte_pos = index >> 3;
      //uint8_t &byte = data[byte_pos];
      uint8_t ival = 1;
      ival = ival << (index % 8);
      //printf("index = %d, data = %d\n", index, &data[byte_pos]);
      data[byte_pos] = val ? (data[byte_pos] | ival) : (data[byte_pos] & ~ival);
      return val;
    }

    size_t scan_and_set() {
      for (size_t i = 0; i < capacity; i++) {
        if (get(i) == false) {
          set(i, true);
          return i;
        }
      }
      return capacity;
    }

    void clear() {
      for (size_t i = 0; i < capacity; i++) {
        data[i / 8] = 0;
      }
    }

private:
  uint8_t data[];
};

#endif //MYOS_BITMAP_H
