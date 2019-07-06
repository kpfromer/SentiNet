

namespace callbacks
{

//Multi threading support
typedef void* (*entry_func)(void*);

template<class T>
struct entry_context
{
  T* object;
  void* context;
}

template<class T>
void* create_entry(T* object_, void* context_)
{
  entry_context<T>* cont = new entry_context;
  cont->object = object_;
  cont->context = context_;
  return (void*)cont;
}

}
