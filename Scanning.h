//---------------------------------------------------------------------------

#ifndef ScanningH
#define ScanningH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//user libs
#include <IOUtils.hpp>
#include <Dialogs.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <vector>
#include <shellapi.h>
#include <DateUtils.hpp>
#include <map>
#include "IniFiles.hpp"
#include <fileCtrl.hpp>



#include "AddDepth.h"


//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialog1;
	TGroupBox *GroupBox1;
	TMemo *Me_filePrint;
	TGroupBox *GB_template;
	TButton *Bt_sh_path;
	TButton *Bt_sh_extension;
	TButton *Bt_sh_name;
	TButton *Bt_sh_separator;
	TComboBox *CB_date;
	TComboBox *CB_time;
	TEdit *Pn_template;
	TComboBox *CB_sizez;
	TCheckBox *ChB_utc0;
	TComboBox *CB_crc;
	TButton *Bt_saveTemplate;
	TEdit *Ed_templateName;
	TComboBox *CB_iniTempls;
	TButton *Bt_deleteTempl;
	TProgressBar *ProgressBar1;
	TButton *Bt_refresh;
	TButton *Bt_sh_relPath;
	TSaveDialog *SaveDialog1;
	TMainMenu *MM;
	TMenuItem *MM_file;
	TMenuItem *MM_filr_openFile;
	TMenuItem *MM_filr_openFolder;
	TMenuItem *MM_settings;
	TMenuItem *MM_settings_lang;
	TMenuItem *N1;
	TMenuItem *English1;
	TPanel *Panel1;
	TButton *Bt_saveToFile;
	TButton *Bt_clearMemory;
	TButton *Bt_hide;
	TMenuItem *MM_settings_templClear;
	TButton *Bt_sh_nameE;
	TButton *Bt_sh_rn;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Bt_sh_pathClick(TObject *Sender);
	void __fastcall Bt_sh_nameClick(TObject *Sender);
	void __fastcall Bt_sh_extensionClick(TObject *Sender);
	void __fastcall Bt_sh_sizeClick(TObject *Sender);
	void __fastcall Bt_sh_separatorClick(TObject *Sender);
	void __fastcall CB_dateChange(TObject *Sender);
	void __fastcall CB_timeChange(TObject *Sender);
	void __fastcall Bt_sh_clearClick(TObject *Sender);
	void __fastcall Bt_refrshClick(TObject *Sender);
	void __fastcall Bt_clearMemoryClick(TObject *Sender);
	void __fastcall CB_sizezChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CB_crcChange(TObject *Sender);
	void __fastcall Ed_templateNameClick(TObject *Sender);
	void __fastcall Bt_saveTemplateClick(TObject *Sender);
	void __fastcall CB_iniTemplsChange(TObject *Sender);
	void __fastcall Bt_deleteTemplClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Pn_templateChange(TObject *Sender);
	void __fastcall Bt_RefreshClick(TObject *Sender);
	void __fastcall Bt_sh_relPathClick(TObject *Sender);
	void __fastcall Bt_saveToFileClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall English1Click(TObject *Sender);
	void __fastcall MM_filr_openFileClick(TObject *Sender);
	void __fastcall MM_filr_openFolderClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Pn_templateMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Bt_hideClick(TObject *Sender);
	void __fastcall MM_settings_templClearClick(TObject *Sender);
	void __fastcall Pn_templateClick(TObject *Sender);
	void __fastcall Pn_templateExit(TObject *Sender);
	void __fastcall Bt_sh_nameEClick(TObject *Sender);
	void __fastcall Bt_sh_rnClick(TObject *Sender);


private:	// User declarations
	struct avaFileInfo
	{
		UnicodeString fullFileName;
		UnicodeString fileName;
		UnicodeString filePath;
		UnicodeString fileRelativePath;
		UnicodeString fileExtension;
		long size;
		int crc;
		TDateTime dateNtime;
		TDateTime dateNtimeUTC0;
		UnicodeString nameNExt;

	};

	struct AvaLanguage
	{
		UnicodeString Bt_sh_path;
		UnicodeString Bt_sh_extension;
		UnicodeString Bt_sh_relPath;
		UnicodeString Bt_sh_name;
		UnicodeString Bt_sh_separator;
		UnicodeString Bt_sh_clear;
		UnicodeString CB_crc;
		UnicodeString CB_date;
		UnicodeString CB_time;
		UnicodeString CB_sizez;
		UnicodeString Bt_refresh;
		UnicodeString Ed_templateName;
		UnicodeString Bt_saveTemplate;
		UnicodeString CB_iniTempls ;
		UnicodeString Bt_deleteTempl;
		UnicodeString Bt_clearMemory;
		UnicodeString Bt_hide;
		UnicodeString Bt_saveToFile;
		UnicodeString Bt_sh_nameE;
		UnicodeString GB_template;

		UnicodeString MM_file;
		UnicodeString MM_filr_openFile;
		UnicodeString MM_filr_openFolder;

		UnicodeString MM_settings;
		UnicodeString MM_settings_lang;
		UnicodeString MM_settings_templClear;

	};

	struct AvaLanguage curLang;

	std::vector<struct avaFileInfo> dogList;


	UnicodeString MAINtemlate;

	void SeparatePath(UnicodeString &name);
	void ScanDirectory(UnicodeString &path, int depth, int curDepth, UnicodeString &parent);
	void processingFileInfo(UnicodeString &file, UnicodeString &parent);
	void ClearMemoes();
	void resetMainDP();
	void ParametrsSettings();
	UnicodeString parcer(UnicodeString & file, int ind);
	void SystemPatterns(UnicodeString &pattern, UnicodeString &result, int ind);
	void printFilesByPattern();
	UnicodeString ChecksumToHexString(int checksum);
	void turnLang();
	void fillLanguage(int iCurLang);
	void computeCRC();


	bool IsInt(const UnicodeString& str);
	bool Check0Depth(UnicodeString &path);

	long GetFileSizeAva(UnicodeString &file);

	TDateTime avaGetDataTime(UnicodeString &file, bool bUTC0);


	void InitTable();
	uint_least32_t GetCRC32(const wchar_t *file_name, DWORD &sz_file);


	//void __fastcall WmDropFiles(TWMDropFiles &Message);
	void virtual __fastcall WMDropFiles(TWMDropFiles &message);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
	END_MESSAGE_MAP(TForm);

	TIniFile *Ini;

	int lang;

	int lastIndex;
	int cursorPos;
	bool bClick;


public:		// User declarations
	__fastcall TForm1(TComponent* Owner);





};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------


#endif
