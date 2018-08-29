//
// MATLAB Compiler: 6.4 (R2017a)
// Date: Wed Aug 29 10:29:59 2018
// Arguments:
// "-B""macro_default""-W""cpplib:getPicFeature""-T""link:lib""-d""D:\Project_de
// s\matlab\wheat_diagnose\for_c++\getPicFeature\for_testing""-v""D:\Project_des
// \matlab\wheat_diagnose\for_c++\getPicFeature.m"
//

#ifndef __getPicFeature_h
#define __getPicFeature_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_getPicFeature
#define PUBLIC_getPicFeature_C_API __global
#else
#define PUBLIC_getPicFeature_C_API /* No import statement needed. */
#endif

#define LIB_getPicFeature_C_API PUBLIC_getPicFeature_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_getPicFeature
#define PUBLIC_getPicFeature_C_API __declspec(dllexport)
#else
#define PUBLIC_getPicFeature_C_API __declspec(dllimport)
#endif

#define LIB_getPicFeature_C_API PUBLIC_getPicFeature_C_API


#else

#define LIB_getPicFeature_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_getPicFeature_C_API 
#define LIB_getPicFeature_C_API /* No special import/export declaration */
#endif

extern LIB_getPicFeature_C_API 
bool MW_CALL_CONV getPicFeatureInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_getPicFeature_C_API 
bool MW_CALL_CONV getPicFeatureInitialize(void);

extern LIB_getPicFeature_C_API 
void MW_CALL_CONV getPicFeatureTerminate(void);



extern LIB_getPicFeature_C_API 
void MW_CALL_CONV getPicFeaturePrintStackTrace(void);

extern LIB_getPicFeature_C_API 
bool MW_CALL_CONV mlxGetPicFeature(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_getPicFeature
#define PUBLIC_getPicFeature_CPP_API __declspec(dllexport)
#else
#define PUBLIC_getPicFeature_CPP_API __declspec(dllimport)
#endif

#define LIB_getPicFeature_CPP_API PUBLIC_getPicFeature_CPP_API

#else

#if !defined(LIB_getPicFeature_CPP_API)
#if defined(LIB_getPicFeature_C_API)
#define LIB_getPicFeature_CPP_API LIB_getPicFeature_C_API
#else
#define LIB_getPicFeature_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_getPicFeature_CPP_API void MW_CALL_CONV getPicFeature(int nargout, mwArray& high_f, const mwArray& picPath);

#endif
#endif
