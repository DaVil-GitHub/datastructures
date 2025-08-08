/**
 * @version 0.1
 * @date    2025/07/20
 *
 * Simple implementation of an static array. For the array, memory is allocated from free storage.
 * Not reviewed yet.
 * 
 * Notes:
 * - _Ty must be easily 
 * - _Ty::operator delete is expected to have nullptr-exception handling
 */

#pragma once

#include <assert.h>
#include <cstdint>
#include <cstring>
#include <initializer_list>

template<class _Ty, uint64_t t_size>
class Array
{
  static_assert<t_size != 0, "Array doesn't allow size of 0.">;
private:
  //DATA
  _Ty* d_dataPtr;

public:
  //CONSTRUCTORS
  Array()
    : d_dataPtr( new _Ty[t_size])
  {
    assert(d_dataPtr != nullptr, "Array failed to allocate memory from free storage");
  }
  Array(_Ty& value, uint64_t amount)
    : Array()
  {
    assert(amount <= t_size, "Tried to assign more values to Array than the size!");
    for (uint64_t index = 0; index < amount; index++)
    {
      this->operator[](index) = value;
    }
  }
  Array(_Ty&& value, uint64_t amount)
    : Array()
  {
    assert(amount <= t_size, "Tried to assign more values to Array than the size!");
    for(uint64_t index = 0; index < amount; index++)
    {
      this->operator[](index) = value;
    }
  }
  Array(std::initializer_list<_Ty>&& data)
    : Array()
  {
    assert(data.size() <= t_size, "Tried to assign more values to Array than the size!");
    std::memcpy(this->d_dataPtr, data.begin(), sizeof(_Ty) * data.size());
  }
  ~Array()
  {
    delete[] d_dataPtr; //Delete has nullptr-exception handling
  }

  //OPERATORS
  inline       _Ty& operator[](uint64_t index)       { return *(d_dataPtr + sizeof(_Ty) * index); }
  inline const _Ty& operator[](uint64_t index) const { return *(d_dataPtr + sizeof(_Ty) * index); };

  inline Array& operator=(std::initializer_list<_Ty>&& data)
  {
    assert(data.size <= t_size, "Tried to assign more values to Array than the size!");
    std::memcpy(this->d_dataPtr, data.begin(), sizeof(_Ty) * data.size());
  }

  //ACCESSORS
    //Data accessors
  inline constexpr uint64_t size()           { return t_size; }
  inline       _Ty* location()       { return d_dataPtr; }
  inline const _Ty* location() const { return d_dataPtr; }
    //Element accessors
  inline       _Ty& front()       { return *(d_dataPtr); }
  inline const _Ty& front() const { return *(d_dataPtr); }
  inline       _Ty& back()        { return *(d_dataPtr + sizeof(_Ty) * t_size); }
  inline const _Ty& back()  const { return *(d_dataPtr + sizeof(_Ty) * t_size); }
  inline       _Ty& getElement(uint64_t index)       { return *(d_dataPtr + sizeof(_Ty) * index); }
  inline const _Ty& getElement(uint64_t index) const { return *(d_dataPtr + sizeof(_Ty) * index); }

  //MANIPULATORS
  inline void setElement(uint64_t index, _Ty&& value)
  {
    this->operator[](index) = value;
  }

  inline void swapElements(uint64_t index1, uint64_t index2)
  {
    uint8_t buffer[sizeof(_Ty)];

    std::memcpy(&buffer,                     &(this->operator[](index1)), sizeof(_Ty)); // Copy first index to buffer
    std::memcpy(&(this->operator[](index1)), &(this->operator[](index2)), sizeof(_Ty)); // Copy second index to first index
    std::memcpy(&(this->operator[](index2)), &buffer,                     sizeof(_Ty)); // Copy buffer to second index
  }

  inline void fillWith(const _Ty& value)
  {
    for (uint64_t index = 0; index < t_size; index++)
      std::memcpy(this->d_dataPtr, &value, sizeof(_Ty));
  }
};