/* http://web.mit.edu/svn/src/swig-1.3.25/Examples/java/class/index.html */
%module mydll

%include <std_wstring.i>
%include "swig_string_array.i"
%include "swig_java_arrays.h"

%{
#include "ResultClass.h"
#include "SwigClass.h"
%}

%include "swig_vector.i"
//%template(ByteVector) std::vector<signed char>;
//%template(CharVector) std::vector<char>;
//%template(IntVector) std::vector<int>;
//%template(DoubleVector) std::vector<double>;
//%template(ByteVector) std::vector<signed char>;
%template(StringVector) std::vector<std::wstring>;

%include "ResultClass.h"
%include "SwigClass.h"

%pragma(java) jniclasscode=%{
    static {
        //System.loadLibrary("mydll");
        /*
        try {
            if(System.getProperty("sun.arch.data.model")=="32")
                swigtest1.NativeUtils.loadLibraryFromJar("/native/x86/mydll.dll");
            else
                swigtest1.NativeUtils.loadLibraryFromJar("/native/x64/mydll.dll");
        }
        catch(java.lang.Exception ex) { throw new java.lang.RuntimeException(ex); }
        */
        swigtest1.NativeUtils.loadLibrary("mydll");
    }
%}

%template(vecint) myvector<int>;
%template(vecdouble) myvector<double>;
