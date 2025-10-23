//---------------------------------------------------------------------------

#ifndef AddDepthH
#define AddDepthH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>


//#include <Scanning.h>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TButton *Bt_F2_dbP;
	TEdit *Ed_F2_dbP;
	void __fastcall Bt_F2_dbPClick(TObject *Sender);
	void __fastcall Ed_F2_dbPClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Ed_F2_dbPKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	bool IsInt(const UnicodeString& str);

	struct AvaF2Language
	{
		UnicodeString Ed_F2_dbP;
		UnicodeString Bt_F2_dbP;
	};

	struct AvaF2Language curLang;

	void fillLanguage(int iCurLang);
	void turnLang();



public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
	int F2depth;
	int lang;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
