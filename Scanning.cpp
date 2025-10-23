 //---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Scanning.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define QUANT 1024 // Читать по 1024 байт файла
#define CRCPOLY 0xEDB88320UL
#define CRCINIT 0xFFFFFFFFUL

uint_least32_t crc_table[256];
bool create_crc_table = false;


TForm1 *Form1;
TIniFile *Ini = new TIniFile("D:/options.ini");

//extern TIniFile *Ini;


//---------------------------------------------------------------------------

//ФУНКЦИИ ФОРМЫ
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Ini = new TIniFile(ExtractFilePath(Application->ExeName)+"options.ini");

	DragAcceptFiles(Form1->Handle, true); // Разрешить перетаскивание на форму

	TStringList* inde = new TStringList();

	Ini->ReadSection("Saved", inde);
	lang = Ini->ReadInteger("Last", "Language", 1);


	for(int i = 0; i < inde->Count; i++)
	{
		CB_iniTempls->Items->Add(inde->Strings[i]);
	}

	Constraints->MinWidth = 923;  // Минимальная ширина
	Constraints->MinHeight = 400; // Минимальная высота

}


void __fastcall TForm1::FormDestroy(TObject *Sender)	//уничтожение формы
{

	DragAcceptFiles(Form1->Handle, false); // Отключить при закрытии

}


void __fastcall TForm1::FormCreate(TObject *Sender)		//стартовые настройки
{
	cursorPos = 0;
	lastIndex = 0;
	bClick = false;
	ClearMemoes();
	ParametrsSettings();
	Pn_template->Text = Ini->ReadString("Last", "Last", "NONE");


	Bt_refresh->Enabled = false;

	printFilesByPattern();

	fillLanguage(lang);
	turnLang();

	GB_template->Height = 57;
}
//---------------------------------------------------------------------------


//ФУЕКЦИИ КНОПОК/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_pathClick(TObject *Sender)
{
	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[Path]", cursorPos);
	cursorPos = o + 6;

//	MAINtemlate = MAINtemlate +  "[Path]";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_nameClick(TObject *Sender)
{
	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[Name]", cursorPos);
	cursorPos = o + 6;

//	Pn_template->Text = Pn_template->Text + "[Name]";
//	MAINtemlate = MAINtemlate + "[Name]";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_extensionClick(TObject *Sender)
{
	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[Extension]", cursorPos);

	cursorPos = o + 11;
//	Pn_template->Text = Pn_template->Text + "[Extension]";
//	MAINtemlate = MAINtemlate + "[Extension]";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_sizeClick(TObject *Sender)
{
//	Pn_template->Text = Pn_template->Text + "[Size]";
//	MAINtemlate = MAINtemlate +  "[Size]";

	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[Size]", cursorPos);
	cursorPos = o + 6;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_separatorClick(TObject *Sender)
{
//	Pn_template->Text = Pn_template->Text + "[Tab]";
//	MAINtemlate = MAINtemlate + "[Tab]";

	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[Tab]", cursorPos);
	cursorPos = o + 5;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CB_dateChange(TObject *Sender)
{
	int o = cursorPos;

	if(CB_date->Text != "")
	{
		Pn_template->Text = Pn_template->Text.Insert("[" + CB_date->Text + "]", cursorPos);
		UnicodeString p = "[" + CB_date->Text + "]";
		cursorPos = p.Length() + o;


//		Pn_template->Text = Pn_template->Text + ("[" + CB_date->Text + "]" );
//		MAINtemlate = MAINtemlate + ("[" + CB_date->Text + "]" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CB_timeChange(TObject *Sender)
{
	int o = cursorPos;


	if(CB_time->Text != "")
	{
		Pn_template->Text = Pn_template->Text.Insert("[" + CB_time->Text + "]", cursorPos);
		UnicodeString p = "[" + CB_time->Text + "]";
		cursorPos = p.Length() + o;


//		Pn_template->Text = Pn_template->Text + ("[" + CB_time->Text + "]" );
//		MAINtemlate = MAINtemlate + ("[" + CB_time->Text + "]" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CB_sizezChange(TObject *Sender)
{
	int o = cursorPos;


	if(CB_sizez->Text != "")
	{
		Pn_template->Text = Pn_template->Text.Insert("[" + CB_sizez->Text + "]", cursorPos);
		UnicodeString p ="[" + CB_sizez->Text + "]";
		cursorPos = p.Length() + o;

//		Pn_template->Text = Pn_template->Text + ("[" + CB_sizez->Text + "]" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_relPathClick(TObject *Sender)
{
	int o = cursorPos;

	Pn_template->Text = Pn_template->Text.Insert("[RelPath]", cursorPos);
	cursorPos = o + 9;

//	Pn_template->Text = Pn_template->Text + "[RelPath]";
}


void __fastcall TForm1::Bt_sh_clearClick(TObject *Sender)
{
	Pn_template->Text = "";
}
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------


//ОСТАЛЬНЫЕ ФУНКЦИИ

//---------------------------------------------------------------------------






void __fastcall TForm1::WMDropFiles(TWMDropFiles &Message)	//Ффункция drug и кинуть
{
	Me_filePrint->Clear();
	HDROP hDrop = (HDROP)Message.Drop;
	const int MAX_PATH_0 = 4096;
	wchar_t filename[MAX_PATH_0] = {0};

	int depth = 0;

    // Получаем общее количество перетащенных файлов
	UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);


	try
    {
		// Обрабатываем каждый файл в цикле
		for (UINT i = 0; i < fileCount; i++)
		{

            // Получаем полный путь к файлу
			if (0 != DragQueryFile(hDrop, i, filename, MAX_PATH_0))
			{

				UnicodeString uniFilename = filename;
				if(DirectoryExists(uniFilename))
				{
					if(!Check0Depth(uniFilename)){
						if(!Form2)
							Form2 = new TForm2(this);

						Form2->lang = lang;
						Form2->ShowModal();

						depth = Form2->F2depth;
					}
					UnicodeString path = uniFilename;
					ScanDirectory(path, depth, 0, path);
				}
				else if(FileExists(uniFilename))   {processingFileInfo(uniFilename, uniFilename); }
			}
		}
		computeCRC();

		printFilesByPattern();

	}
    catch(...)
	{
        Application->MessageBox(L"Ошибка обработки файлов!", L"Ошибка", MB_ICONERROR);
    }

    // Освобождаем ресурсы
    DragFinish(hDrop);
	Message.Result = 0; // Сообщаем системе об обработке

}


//---------------------------------------------------------------------------


void TForm1::ClearMemoes()	//чистим мемосы
{
//	Me_pathPrint->Clear();
	Me_filePrint->Clear();
}
//---------------------------------------------------------------------------


void TForm1::ParametrsSettings()  //запуск с парамерам
{
	int depth = -1;
	UnicodeString path = "";

	for(int i = 1; i < ParamCount()+1; i++) 
	{
		if(IsInt(ParamStr(i)))
			depth = StrToInt(ParamStr(i));
	}

	for(int i = 1; i < ParamCount()+1; i++)
	{
		path = ParamStr(i);
		if(DirectoryExists(path))
		{
			if(Check0Depth(path))
			{
            	ScanDirectory(path, 0, 0, path);
            }

			else{
				if(depth == -1)
				{
					if(!Form2)
						Form2 = new TForm2(this);

					Form2->lang = lang;
					Form2->ShowModal();

					depth = Form2->F2depth;
				}
				path = ParamStr(i);

				ScanDirectory(path, depth, 0, path);
			}

		}

		else if(FileExists(ParamStr(i)))
		{
			UnicodeString p = ParamStr(i);
			processingFileInfo(p, p);

		}
	}

	computeCRC();

}


//---------------------------------------------------------------------------



bool TForm1::Check0Depth(UnicodeString &path)
{
	TStringDynArray dirs = TDirectory::GetDirectories(path, "*.*", TSearchOption::soTopDirectoryOnly);

	if(dirs.Length <= 0)
		return true;

	else return false;

}


void TForm1::resetMainDP()
{

}


long TForm1::GetFileSizeAva(UnicodeString &file)
{
	TFileStream *rrr = new TFileStream(file, fmOpenRead | fmShareDenyNone);
    long fileSize = rrr->Size; // Сохраняем размер перед удалением
    delete rrr; // Освобождаем память
	return fileSize;
}


 bool TForm1::IsInt(const UnicodeString& str)	//проверка является ли числом
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
//	//#0
//
//	unsigned long a = 111;
//	a = filesize(L"D:\\D\\scanIt\\t0.00.txt");
//	ShowMessage(a);


//---------------------------------------------------------------------------


void TForm1::ScanDirectory(UnicodeString &path, int depth, int curDepth, UnicodeString &parent)	//функция сканирование директории
{
	if(curDepth > depth) return;

//	ShowMessage(path);

	TStringDynArray dirs;
	TStringDynArray files;

	if(FileExists(path))
		processingFileInfo(path, parent);
	else if(DirectoryExists(path))
	{
		dirs = TDirectory::GetDirectories(path, "*", TSearchOption::soTopDirectoryOnly);
		files = TDirectory::GetFiles(path, "*", TSearchOption::soTopDirectoryOnly);
	}
	else
	{
		ShowMessage("Error");
		return;
	}

	for(int i = 0; i < files.Length; i++)
	{
		processingFileInfo(files[i], parent);

	}

	for(int i = 0; i < dirs.Length; i++)
	{
		ScanDirectory(dirs[i], depth, curDepth+1, parent);
	}
}


void TForm1::processingFileInfo(UnicodeString &file, UnicodeString &parent)
{
	struct avaFileInfo dog;

	DWORD fileSize;
//	uint_least32_t crc = GetCRC32(L"D:\\D\\scanIt\\t0.1.txt", fileSize);
//
// извлечение файла без расширения

	UnicodeString onlyName = ExtractFileName(file);
	UnicodeString onlyPath = ExtractFilePath(file);

	int i;
	for(i = onlyName.Length(); i > 0; i--)
	{
		if(onlyName[i] == '.')
			break;
	}
	onlyName = onlyName.SubString(1, i-1);

	dog.fullFileName = file;
	int h =  (int)onlyPath.Length();



	dog.fileRelativePath = onlyPath.SubString(parent.Length()+2, onlyPath.Length() );
	dog.fileRelativePath = dog.fileRelativePath.SubString(1, dog.fileRelativePath.Length()-1);


	dog.nameNExt = ExtractFileName(file);
	dog.fileName = onlyName;
	dog.fileExtension = ExtractFileExt(file).SubString(2, 100);
	dog.filePath = ExtractFilePath(file);
	dog.size = GetFileSizeAva(file);
//	dog.crc = (int) GetCRC32(file.c_str(), fileSize);
	dog.dateNtime = avaGetDataTime(file, false);
	dog.dateNtimeUTC0 = avaGetDataTime(file, true);

	dogList.push_back(dog);
}

void TForm1::printFilesByPattern()
{
	UnicodeString l;

	for(int i = 0; i < dogList.size(); i++)
	{
		l = parcer(dogList[i].fullFileName, i);
		Me_filePrint->Lines->Add(l);

		ProgressBar1->Max      = dogList.size();
		ProgressBar1->Position = i;
		Application->ProcessMessages();
	}
	ProgressBar1->Position = 0;
	Bt_refresh->Enabled = false;
}

UnicodeString TForm1::parcer(UnicodeString & file, int ind)
{
	UnicodeString result;
	UnicodeString temp = MAINtemlate;

	while(temp.Length() > 0)
	{
		UnicodeString pattern = "";

		if(temp.Pos("[") != 1 && temp.Pos("[") != 0)
		{
			pattern = temp.SubString(1, temp.Pos("[")-1);
			SystemPatterns(pattern, result, ind);
			temp = temp.SubString(temp.Pos("["), temp.Length());
		}
		else if(temp.Pos("[") == 0)
		{
			SystemPatterns(temp, result, ind);
			temp = "";
		}
		else if(temp.Pos("[") != 0 && temp.Pos("]") == 0 )
		{
        	SystemPatterns(temp, result, ind);
			temp = "";
		}
		else
		{
			pattern = temp.SubString(1, temp.Pos("]"));
			SystemPatterns(pattern, result, ind);
			temp = temp.SubString(temp.Pos("]")+ 1, temp.Length());
		}
	}
	return (result);
}

void TForm1::SystemPatterns(UnicodeString &pattern, UnicodeString &result, int ind)
{	
	UnicodeString o = pattern.SubString(2, pattern.Length() -2 );

	if(pattern == "[Path]")
		result = result + dogList[ind].filePath;

	else if(pattern == "[RelPath]")
	{
		result = result + dogList[ind].fileRelativePath;
	}

	else if(pattern == "[Name]")
		result = result + dogList[ind].fileName;

	else if(pattern == "[NameE]")
		result = result + dogList[ind].nameNExt;

	else if(pattern == "[Size]")
	{
			long y = dogList[ind].size;
			UnicodeString str = IntToStr((int)y);
			result = result + str;
	}
	else if(pattern == "[Tab]")
		result = result + "\t";
	else if(pattern == "[rn]")
		result = result + "\r\n";
	else if(pattern == "[Extension]")
		result = result + dogList[ind].fileExtension;

	else if(CB_date->Items->IndexOf(o) != -1 || CB_time->Items->IndexOf(o) != -1)
	{
			o = StringReplace(
			o,
			L"mm",
			L"nn",
			TReplaceFlags() << rfReplaceAll
			);

		if(ChB_utc0->Checked)
			result = result + (FormatDateTime(o, dogList[ind].dateNtimeUTC0));
		else
			result = result + (FormatDateTime(o, dogList[ind].dateNtime));
	}

	else if(CB_crc->Items->IndexOf(o) != -1)
	{
		result = result + ChecksumToHexString(dogList[ind].crc);
	}



	else if(CB_sizez->Items->IndexOf(o) != -1)
	{
		std::map<UnicodeString, int> sizes;
		sizes["Byte"] = 1;
		sizes["Kilobyte"] = 1024;
		sizes["Megabyte"] = 1024*1024;
		sizes["Gigabyte"] = 1024*1024*1024;

		result = result + IntToStr(  ((int) (dogList[ind].size / sizes[o]))  );


	}

	
	
	else
		result = result + pattern;

}


void __fastcall TForm1::Bt_refrshClick(TObject *Sender)
{
	Me_filePrint->Clear();
	printFilesByPattern();
}
//---------------------------------------------------------------------------

TDateTime TForm1::avaGetDataTime(UnicodeString &file, bool bUTC0)
{
	WIN32_FIND_DATA fileData;
	HANDLE hFind = FindFirstFile(file.c_str(), &fileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
		FindClose(hFind);

		FILETIME ftWrite = fileData.ftLastWriteTime;
        SYSTEMTIME stUTC, stLocal;

		// Конвертируем время файла из UTC в локальное время
		FileTimeToSystemTime(&ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

		// Выводим время изменения

		/*FormatDateTime("dd.mm.yyyy hh:nn:ss",
			EncodeDateTime(stLocal.wYear, stLocal.wMonth, stLocal.wDay,
						  stLocal.wHour, stLocal.wMinute, stLocal.wSecond, 0));*/


		if(bUTC0){
			return FormatDateTime(
			"dd.mm.yyyy hh:nn:ss",
			EncodeDateTime(stUTC.wYear, stUTC.wMonth, stUTC.wDay, stUTC.wHour, stUTC.wMinute, stUTC.wSecond, 0)
			);
		}else{
			return FormatDateTime(
			"dd.mm.yyyy hh:nn:ss",
			EncodeDateTime(stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond, 0)
			);
		}




	}
    else
    {
		ShowMessage("Файл не найден!");
		return 0;
	}

}





void __fastcall TForm1::Bt_clearMemoryClick(TObject *Sender)
{
	if (MessageDlg("Очистить память?",
				  mtConfirmation,
				  TMsgDlgButtons() << mbYes << mbNo,
				  0) == mrYes)
	{
		// Код, который выполняется при нажатии "Да"
		Me_filePrint->Clear();
		dogList.clear();
		lastIndex = 0;
	}
	else
	{
		// Код, который выполняется при нажатии "Нет" (можно оставить пустым)
		return;
	}




}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------


UnicodeString TForm1::ChecksumToHexString(int checksum) {
    return IntToHex(checksum, 0); // 0 - минимальное количество цифр
}


void __fastcall TForm1::FormShow(TObject *Sender)
{

//	L"D:\D\scanIt\";


//	UnicodeString o = "rr.nn";
//	o = StringReplace(
//	o,
//	L"nn",
//	L"mm",
//	TReplaceFlags() << rfReplaceAll
//	);
//
//	ShowMessage(o);
//
//
//	DWORD fileSize;
//	uint_least32_t crc = GetCRC32(L"D:\\D\\scanIt\\t0.1.txt", fileSize);
//	ShowMessage(IntToHex( (int)crc, 8));


//	UnicodeString result = HexCharToUnicodeString(crc); // Вернет "A" (U+0041)

//	UnicodeString hexStr = ChecksumToHexString(crc);
//	ShowMessage(hexStr);
//

//	ShowMessage( Ini->ReadString("Saved", "A", "DEF"));


}
//---------------------------------------------------------------------------

void TForm1::InitTable()
{
 uint_least32_t crc;
 for (int i = 0; i < 256; ++i){
  crc = i;
  for (int j = 0; j < 8; ++j)
   crc = crc & 1 ?(crc >> 1) ^ CRCPOLY :crc >> 1;

  crc_table[i] = crc;
 }
 create_crc_table = true;
}
//---------------------------------------------------------------------------

uint_least32_t TForm1::GetCRC32(const wchar_t *file_name, DWORD &sz_file)
{
	HANDLE h_file;
	h_file = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if(h_file == INVALID_HANDLE_VALUE)
		return 0;

	if (!create_crc_table)
		InitTable();

	unsigned char buf[QUANT];
	DWORD sz_buf;
	sz_file = 0;
	uint_least32_t crc = CRCINIT;
	for (;;) {
		ReadFile(h_file, buf, QUANT, &sz_buf, NULL);
		for (size_t i = 0; i < sz_buf; ++i) {
			crc = crc_table[(crc ^ buf[i]) & 0xFF] ^ (crc >> 8);
		}
		sz_file += sz_buf;
		if (sz_buf != QUANT)
		break;
	 }
		CloseHandle(h_file);

	 return crc ^ CRCINIT;
}

void __fastcall TForm1::CB_crcChange(TObject *Sender)
{

	int o = cursorPos;

	if(CB_crc->Text != "")
	{
		Pn_template->Text = Pn_template->Text.Insert("[" + CB_crc->Text + "]", cursorPos);
		UnicodeString p = "[" + CB_date->Text + "]";
		cursorPos = p.Length() + o;


//		Pn_template->Text = Pn_template->Text + ("[" + CB_date->Text + "]" );
//		MAINtemlate = MAINtemlate + ("[" + CB_date->Text + "]" );
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Ed_templateNameClick(TObject *Sender)
{
	if(Ed_templateName->Text == "Template Name")
		Ed_templateName->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_saveTemplateClick(TObject *Sender)
{
	Ini->WriteString("Saved", Ed_templateName->Text, Pn_template->Text);
	CB_iniTempls->Items->Add(Ed_templateName->Text);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CB_iniTemplsChange(TObject *Sender)
{
	Pn_template->Text = ( Ini->ReadString("Saved", CB_iniTempls->Text, ""));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_deleteTemplClick(TObject *Sender)
{
	TStringList* inde = new TStringList();

	Ini->ReadSection("Saved", inde);

	for(int i = 0; i < inde->Count; i++)
	{
		UnicodeString p =  inde->Strings[i];


		if(p == CB_iniTempls->Text){
			Ini->DeleteKey("Saved", CB_iniTempls->Text);
			CB_iniTempls->DeleteSelected();
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	Ini->WriteString("Last", "Last", MAINtemlate);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Pn_templateChange(TObject *Sender)
{
	MAINtemlate = Pn_template->Text;
	Bt_refresh->Enabled = true;
//	cursorPos = Pn_template->SelStart+1;
}

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_RefreshClick(TObject *Sender)
{
	Me_filePrint->Clear();
	printFilesByPattern();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_saveToFileClick(TObject *Sender)
{


// Настройка фильтров для диалога сохранения
	SaveDialog1->Filter = "Text files (*.txt)|*.txt|CSV files (*.csv)|*.csv|All files (*.*)|*.*";
	SaveDialog1->DefaultExt = "txt"; // Расширение по умолчанию

  // Execute a save file dialog.
  if (SaveDialog1->Execute())
	// First, check if the file exists.
	if (FileExists(SaveDialog1->FileName))
	  // If it exists, throw an exception.
	  throw(Exception("File already exists. Cannot overwrite."));
	else
	  // Otherwise, save the memo box lines into the file.
	  Me_filePrint->Lines->SaveToFile(SaveDialog1->FileName);

}
//---------------------------------------------------------------------------



void TForm1::turnLang()
{
	Bt_sh_path->Caption = curLang.Bt_sh_path;
	Bt_sh_extension->Caption = curLang.Bt_sh_extension;
	Bt_sh_relPath->Caption = curLang.Bt_sh_relPath;
	Bt_sh_name->Caption = curLang.Bt_sh_name;
	Bt_sh_separator->Caption = curLang.Bt_sh_separator;
//	Bt_sh_clear->Caption = curLang.Bt_sh_clear;
	CB_crc->Text = curLang.CB_crc;
	CB_date->Text = curLang.CB_date;
	CB_time->Text = curLang.CB_time;
	CB_sizez->Text = curLang.CB_sizez;
	Bt_refresh->Caption = curLang.Bt_refresh;
	Ed_templateName->Text = curLang.Ed_templateName;
	Bt_saveTemplate->Caption = curLang.Bt_saveTemplate;
	CB_iniTempls->Text = curLang.CB_iniTempls;
	Bt_deleteTempl->Caption =  curLang.Bt_deleteTempl;
	Bt_clearMemory->Caption = curLang.Bt_clearMemory;
	Bt_hide->Caption = curLang.Bt_hide;
	Bt_saveToFile->Caption = curLang.Bt_saveToFile;
	Bt_sh_nameE->Caption = curLang.Bt_sh_nameE;
	GB_template->Caption = curLang.GB_template;

	MM_file->Caption = curLang.MM_file;
	MM_filr_openFile->Caption =	curLang.MM_filr_openFile;
	MM_filr_openFolder->Caption = curLang.MM_filr_openFolder;

	MM_settings->Caption = curLang.MM_settings;
	MM_settings_lang->Caption = curLang.MM_settings_lang;
	MM_settings_templClear->Caption =curLang.MM_settings_templClear;
}


void TForm1::fillLanguage(int iCurLang)
{

	switch (iCurLang)
	{
	case 0:
		curLang.Bt_sh_path = "Абсолютный путь";
		curLang.Bt_sh_extension = "Расширение";
		curLang.Bt_sh_relPath = "Относительный путь";
		curLang.Bt_sh_name = "Имя";
		curLang.Bt_sh_separator = "Таб";
		curLang.Bt_sh_clear = "Очистить шаблон";
		curLang.CB_crc = "Формат CRC";
		curLang.CB_date = "Формат даты";
		curLang.CB_time = "Формат времени";
		curLang.CB_sizez = "Формат размера";
		curLang.Bt_refresh = "Обновить";
		curLang.Ed_templateName = "Имя шаблона";
		curLang.Bt_saveTemplate = "Сохранить шаблон";
		curLang.CB_iniTempls = "Сохранённые шаблоны";
		curLang.Bt_deleteTempl = "Удалить";
		curLang.Bt_clearMemory = "Очистить память";
		curLang.Bt_hide = "Скрыть";
		curLang.Bt_saveToFile = "Сохранить";
		curLang.Bt_sh_nameE = "Имя + расш.";
		curLang.GB_template = "Шаблон";

		curLang.MM_file = "Файл";
		curLang.MM_filr_openFile = "Открыть файл";
		curLang.MM_filr_openFolder = "Открыть папку";

		curLang.MM_settings = "Настройки";
		curLang.MM_settings_lang = "Язык";
		curLang.MM_settings_templClear = "Очистить шаблон";
	break;


	case 1:
		curLang.Bt_sh_path = "Absolute path";
		curLang.Bt_sh_extension = "Extension";
		curLang.Bt_sh_relPath = "Relative path";
		curLang.Bt_sh_name = "Name";
		curLang.Bt_sh_separator = "Tab";
		curLang.Bt_sh_clear = "Clear template";
		curLang.CB_crc = "CRC format";
		curLang.CB_date = "Date format";
		curLang.CB_time = "Time format";
		curLang.CB_sizez = "Size format";
		curLang.Bt_refresh = "Refresh";
		curLang.Ed_templateName = "Template name";
		curLang.Bt_saveTemplate = "Save template";
		curLang.CB_iniTempls = "Saved templates";
		curLang.Bt_deleteTempl = "Delete";
		curLang.Bt_clearMemory = "Clear memory";
		curLang.Bt_hide = "Hide";
		curLang.Bt_saveToFile = "Save";
		curLang.Bt_sh_nameE = "Name + ext";
		curLang.GB_template = "Template";

		curLang.MM_file = "File";
		curLang.MM_filr_openFile = "Open file";
		curLang.MM_filr_openFolder = "Open folder";

		curLang.MM_settings = "Settings";
		curLang.MM_settings_lang = "Language";
		curLang.MM_settings_templClear = "Clear template";
	break;

	}
}


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void TForm1::computeCRC()
{
	DWORD fileSize;
	ProgressBar1->Max = dogList.size();
	int o = dogList.size();



	for(int i = lastIndex; i < o; i++)
	{
		ProgressBar1->Position = i;
		dogList[i].crc = (int) GetCRC32(dogList[i].fullFileName.c_str(), fileSize);
		Application->ProcessMessages();

	}
	ProgressBar1->Position = 0;
	Bt_refresh->Enabled = false;

	lastIndex = dogList.size();
}






void __fastcall TForm1::N1Click(TObject *Sender)
{
	fillLanguage(0);
	turnLang();
	Ini->WriteInteger("Last", "Language", 0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::English1Click(TObject *Sender)
{
	fillLanguage(1);
	turnLang();
	Ini->WriteInteger("Last", "Language", 1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MM_filr_openFileClick(TObject *Sender)
{
	  // Execute an open file dialog.
  if (OpenDialog1->Execute())
	// First, check if the file exists.
	if (FileExists(OpenDialog1->FileName))
	{
	  // If it exists, load the data into the memo box.
//	  Me_filePrint->Lines->Add(ExtractFileName(OpenDialog1->FileName));
//	  Me_pathPrint->Lines->Add(ExtractFilePath(OpenDialog1->FileName));

		UnicodeString o = OpenDialog1->FileName;
		ScanDirectory(o, 0, 0, o);
		computeCRC();
		Me_filePrint->Clear();
		printFilesByPattern();
	}
	else
	  // Otherwise, throw an exception.
	  throw(Exception("File does not exist."));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MM_filr_openFolderClick(TObject *Sender)
{
	UnicodeString path = L"";
	(SelectDirectory(L"msg", L"", path, TSelectDirExtOpts()<<sdShowFiles));
//	{


		int depth = -1;

		if(DirectoryExists(path))
		{
			if(Check0Depth(path) == true)
			{
				Me_filePrint->Clear();
				ScanDirectory(path, 0, 0, path);
				computeCRC();
				printFilesByPattern();
			}

			else
			{
				if(!Form2){
					Form2 = new TForm2(this);
				}
				Form2->lang = lang;
				Form2->ShowModal();
				depth = Form2->F2depth;
				ScanDirectory(path, depth, 0, path);
				computeCRC();
				printFilesByPattern();
			}
		}

		else
		{

		}
//	}

}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormResize(TObject *Sender)
{
	Bt_refresh->Left  = (Pn_template->Left+Pn_template->Width) + 6;
		Bt_hide->Width = Form1->ClientWidth - Bt_hide->Left*2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Pn_templateMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	GB_template->Height = 241;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Bt_hideClick(TObject *Sender)
{
	GB_template->Height = 57;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::MM_settings_templClearClick(TObject *Sender)
{
//	MAINtemlate = "";
	Pn_template->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Pn_templateClick(TObject *Sender)
{
	cursorPos = Pn_template->SelStart+1;
	bClick = true;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Pn_templateExit(TObject *Sender)
{
	cursorPos = Pn_template->SelStart+1;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Bt_sh_nameEClick(TObject *Sender)
{
	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[NameE]", cursorPos);
	cursorPos = o + 7;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Bt_sh_rnClick(TObject *Sender)
{
	//	Pn_template->Text = Pn_template->Text + "[Tab]";
//	MAINtemlate = MAINtemlate + "[Tab]";

	int o = cursorPos;
	Pn_template->Text = Pn_template->Text.Insert("[rn]", cursorPos);
	cursorPos = o + 5;

}
//---------------------------------------------------------------------------

