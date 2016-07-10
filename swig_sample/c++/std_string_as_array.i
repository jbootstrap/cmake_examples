/* -----------------------------------------------------------------------------
 * std_string_as_array.i
 * ----------------------------------------------------------------------------- */

%{
#include <string>
%}

namespace std {

%naturalvar string;

class string;

// string
%typemap(jni) string "jbyteArray"
%typemap(jtype) string "byte[]"
%typemap(jstype) string "byte[]"
%typemap(javadirectorin) string "$jniinput"
%typemap(javadirectorout) string "$javacall"

%typemap(in) string
%{ if(!$input) {
     SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null byte array");
     return $null;
   }
   jbyte* $1_elems = jenv->GetByteArrayElements($input, 0);
   if (!$1_elems) return $null;
   { int nSize = jenv->GetArrayLength($input);
     for (int i=0; i<nSize; i++) {
       $1.push_back($1_elems[i]);
     }
   }
   jenv->ReleaseByteArrayElements($input, $1_elems, 0); %}

%typemap(out) string 
%{ $result = jenv->NewByteArray((int)$1.size());
   jbyte *bytes = jenv->GetByteArrayElements($result, 0);
   for(int i=0; i<(int)$1.size(); i++) {
     bytes[i] = $1[i];
   }
   jenv->ReleaseByteArrayElements($result, bytes, 0); %}

%typemap(javain) string "$javainput"

%typemap(javaout) string {
    return $jnicall;
  }

// const string &
%typemap(jni) const string & "jbyteArray"
%typemap(jtype) const string & "byte[]"
%typemap(jstype) const string & "byte[]"
%typemap(javadirectorin) const string & "$jniinput"
%typemap(javadirectorout) const string & "$javacall"

%typemap(in) const string &
%{ if(!$input) {
     SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null byte array");
     return $null;
   }
   jbyte* $1_elems = jenv->GetByteArrayElements($input, 0);
   if (!$1_elems) return $null;
   $*1_ltype $1_str;
   { int nSize = jenv->GetArrayLength($input);
     for (int i=0; i<nSize; i++) {
       $1_str.push_back($1_elems[i]);
     }
   }
   $1 = &$1_str;
   jenv->ReleaseByteArrayElements($input, $1_elems, 0); %}

%typemap(out) const string & 
%{ $result = jenv->NewByteArray((int)(*$1).size());
   jbyte *bytes = jenv->GetByteArrayElements($result, 0);
   for(int i=0; i<(int)(*$1).size(); i++) {
     bytes[i] = (*$1)[i];
   }
   jenv->ReleaseByteArrayElements($result, bytes, 0); %}

%typemap(javain) const string & "$javainput"

%typemap(javaout) const string & {
    return $jnicall;
  }

}
