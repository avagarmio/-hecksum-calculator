//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddDepth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;


//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{

	BorderStyle = bsSingle; // Запрещает изменение размера окна
	Position = poScreenCenter; // Форма появится по центру экрана

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Bt_F2_dbPClick(TObject *Sender)
{
	if(IsInt(Ed_F2_dbP->Text) && StrToInt(Ed_F2_dbP->Text) >= 0){

		F2depth = StrToInt(Ed_F2_dbP->Text);
		Form2->Close();

	}
	else
		ShowMessage("Error: incorrect value");
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Ed_F2_dbPClick(TObject *Sender)
{

	Ed_F2_dbP->Text = "";
}
//---------------------------------------------------------------------------

bool TForm2::IsInt(const UnicodeString& str)	//проверка является ли числом
{
	if (str.IsEmpty())
		return false;

	try
	{
		str.ToDouble();
		return true;
	}
	catch (EConvertError&)
	{
		return false;
	}
}


void TForm2::fillLanguage(int iCurLang)
{
	switch(iCurLang)
	{
	case 0:
		curLang.Ed_F2_dbP = L"Введите глубину";
		curLang.Bt_F2_dbP = "Продолжить";
	break;

	case 1:
		curLang.Ed_F2_dbP = "Enter depth";
		curLang.Bt_F2_dbP = "Enter";
	break;
	default:
		return;
	}
}

void TForm2::turnLang()
{
	Bt_F2_dbP->Caption = curLang.Ed_F2_dbP;
	Ed_F2_dbP->Text = curLang.Ed_F2_dbP;
}


void __fastcall TForm2::FormShow(TObject *Sender)
{
//	int o = Ini->ReadInteger("Last", "Language", 1);
    F2depth  = 0;
	fillLanguage(lang);
	turnLang();
}
//---------------------------------------------------------------------------



void __fastcall TForm2::Ed_F2_dbPKeyPress(TObject *Sender, wchar_t &Key)
{
	if(Key == VK_RETURN) Bt_F2_dbP->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormCreate(TObject *Sender)
{
	F2depth  = 0;
}
//---------------------------------------------------------------------------

