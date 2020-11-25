#ifndef TASKTRACKERPROJECT_MEMORY_H
#define TASKTRACKERPROJECT_MEMORY_H

namespace core::mem {
  template <class T>
  T * AllocRawBuffer(int32_t size){
    return new T[size];
  }
}

#endif // TASKTRACKERPROJECT_MEMORY_H
