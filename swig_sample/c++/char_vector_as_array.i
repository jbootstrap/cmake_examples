/* -----------------------------------------------------------------------------
 * char_array_as_array.i
 * ----------------------------------------------------------------------------- */

%{
#include "SwigClass.h"
%}

namespace std {

class char_vector;

// std::char_vector
%typemap(jni) char_vector "jbyteArray"
%typemap(jtype) char_vector "byte[]"
%typemap(jstype) char_vector "byte[]"
%typemap(javain) char_vector "$javainput"
%typemap(javaout) char_vector { return $jnicall; }

%typemap(in) char_vector
%{ if(!$input) {
     SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null byte array");
     return $null;
   }
   jbyte* $1_elems = jenv->GetByteArrayElements($input, 0);
   if (!$1_elems) return $null;
   int $1_size = jenv->GetArrayLength($input);
   $1.reserve($1_size);
   for (int i=0; i<$1_size; i++) $1.push_back($1_elems[i]);
   jenv->ReleaseByteArrayElements($input, $1_elems, 0); %}

%typemap(out) char_vector 
%{ $result = jenv->NewByteArray((int)$1.size());
   jbyte *bytes = jenv->GetByteArrayElements($result, 0);
   for(int i=0; i<(int)$1.size(); i++) bytes[i] = $1[i];
   jenv->ReleaseByteArrayElements($result, bytes, 0); %}

}
