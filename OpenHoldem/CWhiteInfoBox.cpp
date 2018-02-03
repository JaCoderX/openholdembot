//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CWhiteInfoBox.h"

#include "..\DLLs\Symbols_DLL\CEngineContainer.h"
#include "CHandresetDetector.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineChipAmounts.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineGameType.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineUserchair.h"
#include "..\DLLs\Tablestate_DLL\TableState.h"
#include "..\DLLs\StringFunctions_DLL\string_functions.h"

CWhiteInfoBox *p_white_info_box = NULL;

CWhiteInfoBox::CWhiteInfoBox() {
}

CWhiteInfoBox::~CWhiteInfoBox() {
}

void CWhiteInfoBox::Draw(RECT client_rect, LOGFONT logfont, CDC *pDC,
                         CPen *black_pen, CBrush *white_brush) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
	RECT		rect = {0};
	CFont		*oldfont = NULL, cFont;
	int			left = 0, top = 0, right = 0, bottom = 0;
		
	bool sym_playing		= TableState()->User()->HasKnownCards();
	// "White box" in the OpenHoldem-GUI with basic important info
	const int k_basic_height = 2;				// pixels
	const int k_extra_height_per_line = 16;	// pixels
	const int k_number_of_default_lines = 4;	// hand-number, game-type, ante, pot
	int height = k_basic_height 
		+ k_extra_height_per_line * k_number_of_default_lines;
	if (kMaxLogSymbolsForWhiteBox > 0)	{
		// Extra lines for symbol-logging
		height += k_extra_height_per_line * kMaxLogSymbolsForWhiteBox;
	}
  // Figure placement of box
	left = client_rect.right/2-70;
	top = 0;
	right = client_rect.right/2+70;
	bottom = top+height;

	pTempPen = (CPen*)pDC->SelectObject(&black_pen);
	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
	pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

	pDC->SetBkMode(OPAQUE);
	pDC->Rectangle(left, top, right, bottom);

	// Set font basics
	logfont.lfHeight = -12;
	logfont.lfWeight = FW_NORMAL;
	cFont.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&cFont);
	pDC->SetTextColor(COLOR_BLACK);

	// Set rectangle
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

  CString info_txt = InfoText();
	// Draw it
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(info_txt.GetString(), info_txt.GetLength(), &rect, NULL);

	// Restore original pen, brush and font
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldfont);
	cFont.DeleteObject();
}

CString CWhiteInfoBox::InfoText() {
  double sym_bblind		= EngineContainer()->symbol_engine_tablelimits()->bblind();
	double sym_sblind		= EngineContainer()->symbol_engine_tablelimits()->sblind();
	double sym_ante			= EngineContainer()->symbol_engine_tablelimits()->ante();
	int sym_lim				  = EngineContainer()->symbol_engine_gametype()->gametype();
	CString sym_handnumber = p_handreset_detector->GetHandNumber();
	double sym_pot			= EngineContainer()->symbol_engine_chip_amounts()->pot();
  CString result, s;
	// handnumber
	if (sym_handnumber != "") {
		s.Format("  Hand #: %s\n", sym_handnumber);
	}	else 	{
		s.Format("  Hand #: -\n");
	}
	result.Append(s);

  // blinds, game-type
	CString format_string;
  if (IsInteger(sym_sblind) && IsInteger(sym_bblind)) {
    // Display as integer numbers
		format_string = "  %s %.0f/%.0f/%.0f\n";
  }
  else {
		// Fractional nunbers: use 2.00 digits  
		format_string = "  %s %.2f/%.2f/%.2f\n";
	}
	s.Format(format_string,
		EngineContainer()->symbol_engine_gametype()->GetGameTypeAsString(),
		sym_sblind, sym_bblind, EngineContainer()->symbol_engine_tablelimits()->bigbet());
	result.Append(s);

	// ante
	if (sym_ante != 0) {
		s.Format("  Ante: %s\n", Number2CString(sym_ante));
		result.Append(s);
	}

	// Pot
	s.Format("  Pot: %s\n", Number2CString(sym_pot));
	result.Append(s);

  // logged symbols
	if (kMaxLogSymbolsForWhiteBox > 0) {
    result.Append("  ");
    result.Append(_custom_log_message);
	}
  return result;
}