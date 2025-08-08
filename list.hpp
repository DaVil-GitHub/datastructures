#pragma once

/**
 * @version 0.1
 * @date    2025/07/25
 * 
 * Implementation of a simply-linked list and corresponding iterators.
 * 
 * Not reviewed yet.
 * 
 * Notes:
 * - _Ty must be trivially constructable
 * 
 */

#include <cassert>
#include <cstdint>
#include <initializer_list>

template<class _Ty>
class LinkedList
{
  //HELPERS
private:
  static constexpr const char* ERROR_BOUNDS = "Tried to reach outside of the list";

  struct Node
  {
    _Ty d_element;
    Node* d_next;

    //CONSTRUCTORS
    Node() 
      : d_element()
      , d_next(nullptr) 
    {}

    Node(const _Ty& element) 
      : d_element(element)
      , d_next(nullptr)
    {}
    Node(const _Ty& element, const Node* next) 
      : Node(element, next) 
    {}

    Node(_Ty&& element) 
      : d_element(element)
      , d_next(nullptr)
    {}
    Node(_Ty&& element, Node* previous, const Node* next) 
      : Node(element) 
    {}

    //OPERATORS

    //ACCESSORS
    inline       _Ty& get()       { return d_element; }
    inline const _Ty& get() const { return d_element; }

    inline       Node* next() { return d_next; }
    inline const Node* next() const { return d_next; }

    //MANIPULATORS
    inline void setPreivous(Node* link) { d_previous = link; }
    inline void setNext(Node* link) { d_next = link; }
  };

public:
  class iterator
  {
    friend class LinkedList<_Ty>;

  private:
    //DATA
    Node* d_node;

  public:
    //CONSTRUCTORS
    iterator() 
      : d_node(nullptr)
    {}

    iterator(Node* node)
      : d_node(node)
    {}

    //OPERATORS
    inline _Ty& operator*() { return d_node->get(); }
    inline iterator& operator=(Node* node) 
    { 
      this->d_node = node;
      return *this; 
    }
    inline iterator& operator++() 
    {
      assert(this->d_node != nullptr, ERROR_BOUNDS);
      this->d_node = d_node->next();
      return *this;
    }
    inline iterator& operator--()
    {
      assert(this->d_node != nullptr, ERROR_BOUNDS);
      this->d_node = d_node->previous();
      return *this;
    }

    //ACCESSORS
    inline _Ty& get() { return d_node->get(); }
    
    //MANIPULATORS
    inline iterator& set(Node* node) 
    { 
      this->d_node = node; 
      return *this; 
    }
  };
  class const_iterator
  {
  private:
    //DATA
    const Node* d_node;

  public:
    //CONSTRUCTORS
    const_iterator()
      : d_node(nullptr)
    {}

    const_iterator(const Node* node)
      : d_node(node)
    {}

    //OPERATORS
    inline const _Ty& operator*() const { return d_node->get(); }
    inline const_iterator& operator=(const Node* node) 
    {
      this->d_node = node;
      return *this; 
    }
    inline const_iterator& operator++()
    {
      assert()
      this->d_node = d_node->next();
      return *this;
    }
    inline const_iterator& operator--()
    {
      assert(this->d_node != nullptr, ERROR_BOUNDS);
      this->d_node = d_node->previous();
    }

    //ACCESSORS
    inline const _Ty& get() const { return d_node->get(); }

    //MANIPULATORS
    inline const_iterator& set(const Node* node)
    {
      this->d_node = node;
      return *this;
    }
  };

  //DATA
private:
  Node* d_first;
  Node* d_last;
  uint64_t d_size;

  //CONSTRUCTORS
public:
  LinkedList() 
    : d_first(nullptr)
    , d_last(nullptr)
    , d_size(0)
  {}
  LinkedList(std::initializer_list<_Ty>&& data) 
    : d_first(nullptr)
    , d_last(nullptr)
    , d_size(data.size())
  {    
    Node* buffer;
    Node** elementPtr = &(this->d_first);

    for (_Ty&& dataIt : data)
    {
      buffer = new Node(dataIt);
      *elementPtr = buffer;
      elementPtr = &(*elementPtr->d_next);
    }

    this->d_last = buffer;
  }
  ~LinkedList() 
  {
    iterator current = this->d_first;
    iterator next = current.d_node->d_next;

    while (current != this->end())
    {
      delete(*(current->d_node));
      current.d_node = next.d_node;
      next++;
    }
  }

  //OPERATORS
  inline LinkedList& operator=(std::initializer_list&& data) 
  { 
    *this = LinkedList(data); 
    return *this; 
  }

  //ACCESSORS

  /**
   * Returns address to first node to allow initialization of iterators
   * 
   * @return Returns Node*(d_first)
   */
  inline       Node* first()       { return d_first; }
  inline const Node* first() const { return d_first; }

  inline uint64_t size() const { return d_size; }

  /**
   * Returns nullptr to allow comparison-based iteration over list.
   * 
   * @return Returns nullptr
   * 
   */
  inline constexpr       Node* end()       { return nullptr; }
  inline constexpr const Node* end() const { return nullptr; }

  //MANIPULATORS
  /**
   * Inserts a node with an object of type _Ty at the index refered to.   
   * Time complexity of t <= O(d_size) 
   * 
   * @param index The position in the list, at which the node is inserted
   * @param element The object to be inserted into the list
   *
   */
  inline void insert(uint64_t index, const _Ty& element) 
  {
    assert(index < this->d_size + 1, ERROR_BOUNDS);

    Node* node = new Node(element);

    iterator previous = nullptr;
    iterator current = this->d_first;

    for (uint64_t i = 0; i < index; i++)
    {
      previous.d_node = current.d_node;
      current++;
    }

    if (previous.d_node != nullptr) 
      previous.d_node->d_next = node;
    node->d_next = current.d_node;

    this->d_first = (this->d_first) * (index > 0) + node * (index == 0);                    // Update d_first if index = 0
    this->d_last = (this->d_last) * (index < this->d_size) + node * (index > this->d_size); // Update d_last if index = d_size 
    this->d_size++;
  }
  inline void insert(uint64_t index, _Ty&& element)
  {
    assert(index < this->d_size + 1, ERROR_BOUNDS);

    Node* node = new Node(element);

    iterator previous = nullptr;
    iterator current = this->d_first;

    for (uint64_t i = 0; i < index; i++)
    {
      previous.d_node = current.d_node;
      current++;
    }

    if (previous.d_node != nullptr)
      previous.d_node->d_next = node;
    node->d_next = current.d_node;

    this->d_first = (this->d_first) * (index > 0) + node * (index == 0);                    // Update d_first if index = 0
    this->d_last = (this->d_last) * (index < this->d_size) + node * (index > this->d_size); // Update d_last if index = d_size 
    this->d_size++;
  }

  /**
   * Inserts a node with an object of type _Ty at the position refered to by the iterator.
   * 
   * @param it The iterator pointing to a node
   * @param element The object to be inserted into the list
   * 
   * Time complexity of t <= O(d_size)
   */
  inline void insert(iterator& it, const _Ty& element)
  {
    Node* node = new Node(element);


  }
  inline void insert(iterator& it, _Ty&& element)
  {
    Node* node = new Node(element);
  }

  inline void insert_front(const _Ty& element)
  { 
    Node* node = new Node(element, this->d_first);
    this->d_first = node;
    this->d_size++;
  }
  inline void insert_front(_Ty&& element)
  {
    Node* node = new Node(element, this->d_first);
    this->d_first = node;
    this->d_size++;
  }
  
  inline void insert_back(const _Ty& element)
  {
    Node* node = new Node(element);
    this->d_last->d_next = node; // Linking new node to the end
    this->d_last = node;
    this->d_size++;
  }
  inline void insert_back(_Ty&& element)
  { 
    Node* node = new Node(element);
    this->d_last->d_next = node;
    this->d_last = node;
    this->d_size++;
  }
};