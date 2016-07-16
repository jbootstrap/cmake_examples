/* -----------------------------------------------------------------------------
 * std_string_as_array.i
 * ----------------------------------------------------------------------------- */

%{
#include <string>
%}

namespace std {

class string;

// std::string
%typemap(jni) string "jbyteArray"
%typemap(jtype) string "byte[]"
%typemap(jstype) string "byte[]"
%typemap(javain) string "$javainput"
%typemap(javaout) string { return $jnicall; }

%typemap(in) string
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

%typemap(out) string 
%{ $result = jenv->NewByteArray((int)$1.size());
   jbyte *bytes = jenv->GetByteArrayElements($result, 0);
   for(int i=0; i<(int)$1.size(); i++) bytes[i] = $1[i];
   jenv->ReleaseByteArrayElements($result, bytes, 0); %}

}
