// http://stackoverflow.com/questions/10596052/stdvector-to-java-util-vector-code-generation-with-swig
%{
#include <vector>
#include <stdexcept>
%}

%include <stdint.i>
%include <std_except.i>

namespace std {

    template<class T> class vector {
      public:
        typedef size_t size_type;
        typedef T value_type;
        typedef const value_type& const_reference;
        vector();
        //vector(size_type n);
        //vector(const vector& o);
        //size_type capacity() const;
        //void reserve(size_type n);
        //%rename(isEmpty) empty;
        //bool empty() const;
        void clear();
        //void push_back(const value_type& x);
        %extend {
            const_reference get(int i) const throw (std::out_of_range) {
                return $self->at(i);
            }
            value_type set(int i, const value_type& VECTOR_VALUE_IN) throw (std::out_of_range) {
                const T old = $self->at(i);
                $self->at(i) = VECTOR_VALUE_IN;
                return old;
            }
            int32_t size() const {
              return $self->size();
            }
            void add(const T &x) {
              $self->push_back(x);
            }
            //void removeRange(int32_t from, int32_t to) {
            //  $self->erase($self->begin()+from, $self->begin()+to);
            //}
        }
    };
}

// Java typemaps for autoboxing in return types of generics
%define AUTOBOX(CTYPE, JTYPE)
%typemap(autobox) CTYPE, const CTYPE&, CTYPE& "JTYPE"
%enddef
AUTOBOX(double, Double)
AUTOBOX(float, Float)
AUTOBOX(boolean, Boolean)
AUTOBOX(char, byte)
AUTOBOX(signed char, byte)
AUTOBOX(short, Short)
AUTOBOX(int, Integer)
AUTOBOX(long, Long)
AUTOBOX(SWIGTYPE, $typemap(jstype,$1_basetype))

//%typemap(javabase) std::vector "java.util.AbstractList<$typemap(autobox,$1_basetype::value_type)>"
//%typemap(javabase) std::vector "java.lang.Object"
//%typemap(javainterface) std::vector "java.util.RandomAccess"
%typemap(jstype) std::vector get "$typemap(autobox,$1_basetype)"
%typemap(jstype) std::vector set "$typemap(autobox,$1_basetype)"
%typemap(jstype) std::vector &VECTOR_VALUE_IN "$typemap(autobox,$1_basetype)"
%typemap(javacode) std::vector %{
  /*
  public $javaclassname(java.util.Collection<$typemap(autobox,$1_basetype::value_type)> e) {
    //this.reserve(e.size());
    for($typemap(autobox,$1_basetype::value_type) value: e) {
      //this.push_back(value);
      this.add(value);
    }
  }
  */
  public $javaclassname($typemap(autobox,$1_basetype::value_type)[] e) {
    //this.reserve(e.size());
    for($typemap(autobox,$1_basetype::value_type) value: e) {
      //this.push_back(value);
      this.add(value);
    }
  }
  public $typemap(autobox,$1_basetype::value_type)[] toArray() {
    $typemap(autobox,$1_basetype::value_type)[] result = new $typemap(autobox,$1_basetype::value_type)[this.size()];
    for(int i=0; i<this.size(); i++)
    {
        result[i] = this.get(i);
    }
    return result;
  }
%}
