#pragma once
//*******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Two interfaces for OpenHoldems scraper-DLL
//.  * object-oriented interface for OpenHoldem.
//   * simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************
//
// The scraper.DLL depends on
//   * NumericalFunctions.DLL
//   * StringFunctions.DLL
//   * WindowFunction.DLL
// These DLLs are expected to be in the same directory.
//
//*******************************************************************************

#include <afxwin.h>
#include "LibDef.h"
#include "CTablemap\CTablemap.h"
#include "CTablemap\CTableMapAccess.h"

//*******************************************************************************
//
// Object-oriented interface for OpenHoldem.
//
//*******************************************************************************

class CBasicScraper;

// Accessor for the singleton of CBasicScraper
// Singleton gets initialized via the accessor-function once needed
SCRAPER_DLL_API CBasicScraper* BasicScraper();

class SCRAPER_DLL_API CBasicScraper {
public:
  CBasicScraper();
  ~CBasicScraper();
public:
  // Loads a tablemap (and automatically unloads the previous one)
  bool LoadTablemap(const char* path);
  void ConnectScraperToWindow(HWND window);
  //!!!void TakeScreenshot();
  CString ScrapeRegion(const CString name);
public:
  CTablemap* Tablemap() { return &_tablemap; }
  CTablemapAccess* TablemapAccess() { return &_tablemap_access; }
public:
  // For replay-frames
  const HBITMAP	entire_window_cur() { return _entire_window_cur; }
private:
  void CreateBitmaps(void);
  void DeleteBitmaps(void);
  bool IsIdenticalScrape();
private:
  bool ProcessRegion(RMapCI r_iter);
private:
  void delete_entire_window_cur();
private:
  CTablemap _tablemap;
  CTablemapAccess _tablemap_access;
private:
  HBITMAP	_entire_window_last;
  HBITMAP	_entire_window_cur;
private:
  HWND _connected_window;
private:
  // Counter of GDI objects (potential memorz leak)
  // Should be 0 at end of program -- will be checked.
  int _leaking_GDI_objects;
  // Used for potential optimizations
  int _total_region_counter;
  int _identical_region_counter;
};

//*******************************************************************************
//
// Simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************

// Assigns a table to the scraper
// The order of ConnectScraperToWindow and LoadTablemap
// does not matter, but both of them must have been called
// before ScrapeRegion.
SIMPLE_SCRAPER_DLL_API void ConnectScraperToWindow(HWND window);

// Loads a tablemap (and automatically unloads the previous one).
// The tablemap may contain custom regions.
// The symbol s$islobby or s$ispopup can be used to turn the
// tablemap-completeness-checker off.
SIMPLE_SCRAPER_DLL_API bool LoadTablemap(const char* path);

// Takes a screenshot of the connected window / table.
// Thereafter ScrapeRegion can be called.
//!!!SIMPLE_SCRAPER_DLL_API void TakeScreenshot();

// Result-buffer has to be managed by the caller.
// Returned results are usually numbers and player-names,
// So any reasonable buffer should do OK, we recommend 255 bytes.
SIMPLE_SCRAPER_DLL_API void ScrapeRegion(const char* in_name, const int in_result_buffer_size, char* out_result);