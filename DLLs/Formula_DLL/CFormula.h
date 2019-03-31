#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Container-class for all formula related functionality
//
//******************************************************************************

#include "LibDef.h"

class CAutoplayerLogic;
class CAutoplayerTrace;
class CFormulaParser;
class CFunctionCollection;

class FORMULA_DLL_API CFormula {
public:
  CFormula();
  ~CFormula();
public:
  CAutoplayerLogic* AutoplayerLogic() {
    return _autoplayer_logic;
  }
  CAutoplayerTrace* AutoplayerTrace() {
    return _autoplayer_trace;
  }
  CFormulaParser* FormulaParser() {
    return _formula_parser;
  }
  CFunctionCollection* FunctionCollection() {
    return _function_collection;
  }
private:
  CAutoplayerLogic* _autoplayer_logic;
  CAutoplayerTrace* _autoplayer_trace;
  CFormulaParser* _formula_parser;
  CFunctionCollection* _function_collection;
};

FORMULA_DLL_API CFormula* Formula();