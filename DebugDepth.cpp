//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DebugDepth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Bt_F4_dbDClick(TObject *Sender)
{
	F4depth = StrToInt(Ed_F4_dbD->Text);
	Form4->Close();
}
//---------------------------------------------------------------------------
