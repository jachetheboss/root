// Author: Enric Tejedor CERN  06/2018
// Original PyROOT code by Wim Lavrijsen, LBL

/*************************************************************************
 * Copyright (C) 1995-2018, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

// Bindings
#include "PyROOTPythonize.h"
#include "PyROOTWrapper.h"
#include "RPyROOTApplication.h"

// Cppyy
#include "CPyCppyy/API.h"
#include "../../cppyy/CPyCppyy/src/CallContext.h"
#include "../../cppyy/CPyCppyy/src/ProxyWrappers.h"

// ROOT
#include "TROOT.h"
#include "TSystem.h"
#include "RConfigure.h"

// Standard
#include <string>
#include <sstream>
#include <utility>
#include <vector>

using namespace CPyCppyy;

namespace PyROOT {
PyObject *gRootModule = 0;
}

// Methods offered by the interface
static PyMethodDef gPyROOTMethods[] = {
   {(char *)"AddCPPInstancePickling", (PyCFunction)PyROOT::AddCPPInstancePickling, METH_VARARGS,
    (char *)"Add a custom pickling mechanism for Cppyy Python proxy objects"},
   {(char *)"AddBranchAttrSyntax", (PyCFunction)PyROOT::AddBranchAttrSyntax, METH_VARARGS,
    (char *)"Allow to access branches as tree attributes"},
   {(char *)"AddTClassDynamicCastPyz", (PyCFunction)PyROOT::AddTClassDynamicCastPyz, METH_VARARGS,
    (char *)"Cast the void* returned by TClass::DynamicCast to the right type"},
   {(char *)"AddTObjectEqNePyz", (PyCFunction)PyROOT::AddTObjectEqNePyz, METH_VARARGS,
    (char *)"Add equality and inequality comparison operators to TObject"},
   {(char *)"SetBranchAddressPyz", (PyCFunction)PyROOT::SetBranchAddressPyz, METH_VARARGS,
    (char *)"Fully enable the use of TTree::SetBranchAddress from Python"},
   {(char *)"BranchPyz", (PyCFunction)PyROOT::BranchPyz, METH_VARARGS,
    (char *)"Fully enable the use of TTree::Branch from Python"},
   {(char *)"AddPrettyPrintingPyz", (PyCFunction)PyROOT::AddPrettyPrintingPyz, METH_VARARGS,
    (char *)"Add pretty printing pythonization"},
   {(char *)"GetEndianess", (PyCFunction)PyROOT::GetEndianess, METH_NOARGS, (char *)"Get endianess of the system"},
   {(char *)"GetDataPointer", (PyCFunction)PyROOT::GetDataPointer, METH_VARARGS,
    (char *)"Get pointer to data of a C++ object"},
   {(char *)"GetSizeOfType", (PyCFunction)PyROOT::GetSizeOfType, METH_VARARGS, (char *)"Get size of data-type"},
   {(char *)"AsRVec", (PyCFunction)PyROOT::AsRVec, METH_O, (char *)"Get object with array interface as RVec"},
#ifdef R__HAS_DATAFRAME
   {(char *)"AsRTensor", (PyCFunction)PyROOT::AsRTensor, METH_O, (char *)"Get object with array interface as RTensor"},
   {(char *)"MakeNumpyDataFrame", (PyCFunction)PyROOT::MakeNumpyDataFrameImpl, METH_O,
    (char *)"Make RDataFrame from dictionary of numpy arrays"},
#endif
   {(char *)"InitApplication", (PyCFunction)PyROOT::RPyROOTApplication::InitApplication, METH_VARARGS,
    (char *)"Initialize interactive ROOT use from Python"},
   {(char *)"InstallGUIEventInputHook", (PyCFunction)PyROOT::RPyROOTApplication::InstallGUIEventInputHook, METH_NOARGS,
    (char *)"Install an input hook to process GUI events"},
   {(char *)"_CPPInstance__expand__", (PyCFunction)PyROOT::CPPInstanceExpand, METH_VARARGS,
    (char *)"Deserialize a pickled object"},
   {(char *)"ClearProxiedObjects", (PyCFunction)PyROOT::ClearProxiedObjects, METH_NOARGS,
    (char *)"Clear proxied objects regulated by PyROOT"},
   {NULL, NULL, 0, NULL}};

#define QuoteIdent(ident) #ident
#define QuoteMacro(macro) QuoteIdent(macro)
#define LIBROOTPYZ_NAME "libROOTPythonizations" QuoteMacro(PY_MAJOR_VERSION) "_" QuoteMacro(PY_MINOR_VERSION)
#define LIBCPPYY_NAME "libcppyy" QuoteMacro(PY_MAJOR_VERSION) "_" QuoteMacro(PY_MINOR_VERSION)

#define CONCAT(a, b, c, d) a##b##c##d
#define LIBROOTPYZ_INIT_FUNCTION(a, b, c, d) CONCAT(a, b, c, d)

struct module_state {
   PyObject *error;
};

#define GETSTATE(m) ((struct module_state *)PyModule_GetState(m))

static int rootmodule_traverse(PyObject *m, visitproc visit, void *arg)
{
   Py_VISIT(GETSTATE(m)->error);
   return 0;
}

static int rootmodule_clear(PyObject *m)
{
   Py_CLEAR(GETSTATE(m)->error);
   return 0;
}

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT,       LIBROOTPYZ_NAME,  NULL,
                                       sizeof(struct module_state), gPyROOTMethods,   NULL,
                                       rootmodule_traverse,         rootmodule_clear, NULL};

/// Initialization of extension module libROOTPythonizations

#define PYROOT_INIT_ERROR return NULL
LIBROOTPYZ_INIT_FUNCTION(extern "C" PyObject* PyInit_libROOTPythonizations, PY_MAJOR_VERSION, _, PY_MINOR_VERSION) ()
{
   using namespace PyROOT;

// setup PyROOT
   gRootModule = PyModule_Create(&moduledef);
   if (!gRootModule)
      PYROOT_INIT_ERROR;

   // keep gRootModule, but do not increase its reference count even as it is borrowed,
   // or a self-referencing cycle would be created

   // Make sure libcppyy has been imported
   PyImport_ImportModule(LIBCPPYY_NAME);

   // setup PyROOT
   PyROOT::Init();

   // signal policy: don't abort interpreter in interactive mode
   CallContext::SetGlobalSignalPolicy(!gROOT->IsBatch());

   // inject ROOT namespace for convenience
   PyModule_AddObject(gRootModule, (char *)"ROOT", CreateScopeProxy("ROOT"));

   Py_INCREF(gRootModule);
   return gRootModule;
}
