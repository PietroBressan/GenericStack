#ifndef STACK_H
#define STACK_H


#include <ostream>  
#include <cassert>
#include <stdexcept>     

/**
  @brief GenericStack<T>
  
  Classe che implementa uno stack di elementi generici T.
*/
template <typename T>
class GenericStack {

public:
  
  class const_iterator;

  typedef unsigned int size_type;

  /**
    Costruttore base della classe GenericStack<T>.
    Attenzione questo costruttore sovrascrive il costruttore di defalut GenericStack<T>::GenericStack(),
    è pertanto obbligatorio in fase di creazione specificare una dimensione.

    @param Dimensione della lista

    @post _stack_size = (Dimensione della lista + 1)
    @post _current_size = 0

    @throw std::bad_alloc L'eccezione è lanciata quando l'allocazione di memoria per lo stack fallisce
  */
  explicit GenericStack(const size_type _stack_size) : _stack_size(0), _current_size(0), _stack(nullptr) {
    
    try{
      _stack = new T[_stack_size + 1];
      this->_stack_size = _stack_size + 1;
      _current_size = 0;
    }catch(const std::bad_alloc &ex){
      _stack = nullptr;
      //Informo l'utente della mancata allocazione di memoria
      throw ex;
    }

 }

  /**
    Copy constructor della classe GenericStack<T>.

    @param Reference costante al GenericStack da copiare

    @post _stack_size = other._stack_size;
    @post _current_size = other._current_size

    @throw std::bad_alloc L'eccezione è lanciata quando l'allocazione di memoria per lo stack fallisce
  */
  GenericStack(const GenericStack &other) : _stack_size(0), _current_size(0), _stack(nullptr){

    try{
      _stack = new T[other._stack_size];
      _stack_size = other._stack_size;
      _current_size = 0;
    }catch(const std::bad_alloc &ex){
      _stack = nullptr;
      throw ex;
    }

    GenericStack tmp(other._stack_size - 1);
    const_iterator itr = other.begin();
    const_iterator end = other.end();

    //Ci serviamo di un oggetto temporaneo così da ristabilire l'ordine corretto dello stack.
    for(; itr != end; --itr){
      tmp.push(*itr);
    }

    itr = tmp.begin();
    end = tmp.end();
    for(; itr != end; --itr){
      push(*itr);
    }
  }

  /**
    Costruttore della classe GenericStack<T> a partire da due const_iterator.

    @param iteratore che punta all'inizio della seguenza dati
    @param iteratore che punta alla fine della seguenza dati
    @post _stack_size = numero di elementi tra begin e end + 1
    @post _current_size = numero di elementi tra begin e end

    @throw std::bad_alloc L'eccezione è lanciata quando l'allocazione di memoria per lo stack fallisce
  */
  template <typename CIter>
  GenericStack(const CIter start, const CIter stop) : _stack_size(0), _current_size(0), _stack(nullptr){
    
    //sfrutto l'operatore di differenza definito nella classe const_iterator
    _stack_size = (stop - start) + 1;
    try{
      _stack = new T[_stack_size];
      _current_size = 0;
    }catch(const std::bad_alloc &ex){
      _stack = nullptr;
      _stack_size = 0;
      throw ex;
    }

    GenericStack tmp(_stack_size - 1);
    CIter itr;
    CIter stack_end;
    
    //Gestione del caso limite in cui vengono invertiti, nel passaggio degli argomenti,
    //l'iteratore di fine e quello di inizio.
    //Grazie all'operatore > possiamo rendercene conto.
    if(stop > start){
       itr = start;
       stack_end = stop;
    }else{
       itr = stop;
       stack_end = start;
    }
    
    for(; itr != stack_end; --itr){
      //Gestione di tipi di dato differenti (qualora possibile)
      tmp.push(static_cast<T>(*itr));
    }

    const_iterator tmp_itr = tmp.begin();
    const_iterator tmp_stack_end = tmp.end();
    for(; tmp_itr != tmp_stack_end; --tmp_itr){
      push(*tmp_itr);
    }
  }

  /**
    Metodo generico per il refactor di un GenericStack<T> già esistente a partire da due const_iterator.

    @param iteratore che punta all'inizio di una seguenza dati
    @param iteratore che punta alla fine della seguenza dati
    @post _stack_size = numero di elementi tra begin e end + 1
    @post _current_size = numero di elementi tra begin e end

    @throw std::bad_alloc L'eccezione è lanciata quando l'allocazione di memoria per lo stack fallisce
  */
  template <typename CIter>
  void refactor(const CIter begin, const CIter end){

    GenericStack tmp(begin, end);
    flush();
    std::swap(this->_stack, tmp._stack);
    std::swap(this->_stack_size, tmp._stack_size);
    std::swap(this->_current_size, tmp._current_size);

  }

  /**
    Operatore di assegnamento per la classe GenericStack<T>.

    @param reference ad un altro oggetto GenericStack
    @return reference allo stack stesso, dopo essere stato modificato
    @post _stack_size = other._stack_size
    @post _current_size = other._current_size
  */
  GenericStack& operator=(const GenericStack &other) {
    if(this!=&other) {
      GenericStack tmp(other);

      std::swap(this->_stack, tmp._stack);
      std::swap(this->_stack_size, tmp._stack_size);
      std::swap(this->_current_size, tmp._current_size);    
    }
    return *this;
  }

  /**
    Metodo per il ritorno del numero di elementi attualmente nella struttura dati.

    @return numero di elementi contenuti nella struttura dati
  */
  size_type current_stack_size() const {
    return _current_size;
  }

  /**
    Metodo per il ritorno della dimensione della struttura dati.

    @return dimensione della struttura dati
  */
  size_type size() const {
    return _stack_size - 1;
  }

  /**
    Metodo l'inserimento in cima allo stack di un nuovo elemento.
    
    @param reference all''oggetto da inserire
    @return dimensione della struttura dati

    @throw std::out_of_range L'eccezione è lanciata quando si prova ad inserire un elemento ma lo stack è pieno
  */
  void push(const T &element){
    ++_current_size;
    if (_current_size == _stack_size){
      --_current_size;
      throw std::out_of_range("Push out of range.");
    }
    _stack[_current_size] = element;
  }

  /**
    Metodo per prelevare l'elemento in cima dello stack.
    Prelevare l'elemento comporta la sua eliminazione dallo stack.
    
    @return reference costante all'oggetto prelevato

    @throw std::out_of_range L'eccezione è lanciata quando si prova a prelevare un elemento ma lo stack è vuoto
  */
  const T& pop() {

    if(_current_size == 0){
      throw std::out_of_range ("Pop out of range.");
    }
    T &tmp = _stack[_current_size];
    --_current_size;

    return tmp;
  }

  /**
    Metodo per svuotare lo stack.
    
    @post _current_size = 0
  */
  void flush() {
    _current_size = 0;
  }

  /**
    Metodo generico per valutare un elemento T secondo un predicato P.
    
    @return true se l'elemento soddisfa il predicato, false altrimenti
  */
  template <typename P>
  bool checkif(const T &element, const P comp_policy) const{
        return comp_policy(element);
}
  
  /**
    Distruttore di GenericStack<T>.
    
    @post _stack_size = 0
    @post _current_size = 0
  */
  ~GenericStack() {
    delete[] _stack;
    _stack_size = 0;
    _current_size = 0;
  }
  
  class const_iterator {
     
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T                         value_type;
    typedef ptrdiff_t                 difference_type;
    typedef const T*                  pointer;
    typedef const T&                  reference;

  
    const_iterator() : _itr_location(nullptr), _current_pos(0) {}
    
    const_iterator(const const_iterator &other) : _itr_location(other._itr_location), _current_pos(other._current_pos) {}

    const_iterator& operator=(const const_iterator &other) {
      if(*this != other){
        _itr_location = other._itr_location;
        _current_pos = other._current_pos;
      }
      return *this;
    }

    ~const_iterator() {
    }

    //Ridefinizione dell'operatore -, utile per la classe GenericStack<T>
    difference_type operator-(const const_iterator &other) const{
      if (other._current_pos >= _current_pos){
        return other._current_pos - _current_pos;
      }
      
       return _current_pos - other._current_pos;
    }

    /**
    @throw std::out_of_range L'eccezione è lanciata quando si prova a ritornare una reference
    dell'valore posizionato in fondo allo stack (valore placeholder)
  */
    reference operator*() const {
      if(_current_pos == 0){
        throw std::out_of_range ("Reference out of range.");
      }
      return *_itr_location;
    }

    //Ridefinizione degli operatori <>, utili alla classe GenericStack<T>
    bool operator<(const const_iterator &other) const{
      return _current_pos > other._current_pos;
    }

    bool operator>(const const_iterator &other) const{
      return other < *this;
    }
    
    /**
    @throw std::out_of_range L'eccezione è lanciata quando si prova a ritornare un puntatore
    al'valore posizionato in fondo allo stack (valore placeholder)
  */
    pointer operator->() const {
      if(_current_pos == 0){
        throw std::out_of_range ("Pointer out of range.");
      }
      return *_itr_location;
    }
    
    /**
    @throw std::out_of_range L'eccezione è lanciata quando si prova a posizionare l'iteratore
    su una locazione di memoria che non fa parte dello stack
    */
    const_iterator operator--(int) {
      if(_current_pos == 0){
        throw std::out_of_range ("Iterator out of range.");
      }
      const_iterator tmp(*this);
      --_itr_location;
      --_current_pos;
      return tmp;
    }

    /**
    @throw std::out_of_range L'eccezione è lanciata quando si prova a posizionare l'iteratore
    su una locazione di memoria che non fa parte dello stack
    */
    const_iterator& operator--() {
      if(_current_pos == 0){
        throw std::out_of_range ("Iterator out of range.");
      }
      --_itr_location;
      --_current_pos;
      return *this;
    }

    bool operator==(const const_iterator &other) const {
      return (_itr_location == other._itr_location);
    }
    
    bool operator!=(const const_iterator &other) const {
      return (_itr_location != other._itr_location);
    }

  private:

    friend class GenericStack; 

    const_iterator(const T* _itr_location, size_type _current_pos) {

      this->_itr_location = _itr_location; 
      this->_current_pos = _current_pos;
    }
    
    const T *_itr_location;
    size_type _current_pos;
    
  };

  //Ritorna un iteratore che punta alla cima dello stack
  const_iterator begin() const {
    return const_iterator((_stack + _current_size), _current_size);
  }

  //Ritorna un iteratore che punta al fondo dello stack
  const_iterator end() const {
    return const_iterator(_stack, 0);
  }

private:

  size_type _stack_size;
  size_type _current_size;
  T* _stack;
  };

/**
    Ridefinizione dell'operatore di stream, affinchè accetti in input un GenericStack<T>.
    Prima di utilizzarlo assicurarsi che per il tipo di dato T sia definito l'operatore di stream.
    
    @param lo stream di output
    @param l'oggetto GenericStack da mandare in output.

    @return lo stream di output
  */
template <typename T>
std::ostream &operator<<(std::ostream &os, const GenericStack<T> &stack) {
    typename GenericStack<T>::const_iterator itr = stack.begin();
    typename GenericStack<T>::const_iterator stack_end = stack.end();
    for(; itr != stack_end; --itr){
      os << *itr << " ";
    }
    os << std::endl; 
    return os;
}

#endif
