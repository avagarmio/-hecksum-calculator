object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'CheckSum+++'
  ClientHeight = 710
  ClientWidth = 987
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MM
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 241
    Width = 987
    Height = 422
    Align = alClient
    TabOrder = 0
    object Me_filePrint: TMemo
      Left = 2
      Top = 15
      Width = 983
      Height = 405
      Align = alClient
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
  object GB_template: TGroupBox
    Left = 0
    Top = 0
    Width = 987
    Height = 241
    Align = alTop
    Caption = 'Template'
    TabOrder = 1
    DesignSize = (
      987
      241)
    object Bt_sh_path: TButton
      Left = 16
      Top = 61
      Width = 122
      Height = 25
      Caption = 'Global Path'
      TabOrder = 0
      OnClick = Bt_sh_pathClick
    end
    object Bt_sh_extension: TButton
      Left = 16
      Top = 123
      Width = 122
      Height = 25
      Caption = 'Extension'
      TabOrder = 1
      OnClick = Bt_sh_extensionClick
    end
    object Bt_sh_name: TButton
      Left = 16
      Top = 185
      Width = 122
      Height = 25
      Caption = 'Name'
      TabOrder = 2
      OnClick = Bt_sh_nameClick
    end
    object Bt_sh_separator: TButton
      Left = 414
      Top = 61
      Width = 116
      Height = 25
      Caption = 'Tab'
      TabOrder = 3
      OnClick = Bt_sh_separatorClick
    end
    object CB_date: TComboBox
      Left = 279
      Top = 63
      Width = 129
      Height = 21
      ParentShowHint = False
      ShowHint = False
      TabOrder = 4
      Text = 'Date format'
      OnChange = CB_dateChange
      Items.Strings = (
        ''
        'D.M.YY'
        'DD'
        'DD.MM.YYYY'
        'MM'
        'YY'
        'YYYY')
    end
    object CB_time: TComboBox
      Left = 279
      Top = 90
      Width = 129
      Height = 21
      TabOrder = 5
      Text = 'Time format'
      OnChange = CB_timeChange
      Items.Strings = (
        ''
        'hh.mm'
        'hh.mm.ss'
        'hh'
        'mm'
        'ss')
    end
    object Pn_template: TEdit
      Left = 16
      Top = 22
      Width = 722
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 6
      OnChange = Pn_templateChange
      OnClick = Pn_templateClick
      OnExit = Pn_templateExit
      OnMouseDown = Pn_templateMouseDown
    end
    object CB_sizez: TComboBox
      Left = 144
      Top = 63
      Width = 129
      Height = 21
      TabOrder = 7
      Text = 'Size format'
      OnChange = CB_sizezChange
      Items.Strings = (
        ''
        'Byte'
        'Kilobyte'
        'Megabyte'
        'Gigabyte')
    end
    object ChB_utc0: TCheckBox
      Left = 279
      Top = 117
      Width = 82
      Height = 17
      Caption = 'UTC0'
      Checked = True
      State = cbChecked
      TabOrder = 8
    end
    object CB_crc: TComboBox
      Left = 536
      Top = 63
      Width = 121
      Height = 21
      ItemIndex = 0
      TabOrder = 9
      OnChange = CB_crcChange
      Items.Strings = (
        ''
        'CRC 32')
    end
    object Bt_saveTemplate: TButton
      Left = 831
      Top = 121
      Width = 145
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Save template'
      TabOrder = 10
      OnClick = Bt_saveTemplateClick
    end
    object Ed_templateName: TEdit
      Left = 831
      Top = 63
      Width = 145
      Height = 21
      Anchors = [akTop, akRight]
      TabOrder = 11
      Text = 'Template Name'
      OnClick = Ed_templateNameClick
    end
    object CB_iniTempls: TComboBox
      Left = 831
      Top = 22
      Width = 145
      Height = 21
      Anchors = [akTop, akRight]
      TabOrder = 12
      Text = 'Saved templs'
      OnChange = CB_iniTemplsChange
    end
    object Bt_deleteTempl: TButton
      Left = 831
      Top = 90
      Width = 145
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Delete'
      TabOrder = 13
      OnClick = Bt_deleteTemplClick
    end
    object Bt_refresh: TButton
      Left = 745
      Top = 22
      Width = 81
      Height = 21
      Caption = 'Reftresh'
      TabOrder = 14
      OnClick = Bt_refrshClick
    end
    object Bt_sh_relPath: TButton
      Left = 16
      Top = 92
      Width = 122
      Height = 25
      Caption = 'Relatibe path'
      TabOrder = 15
      OnClick = Bt_sh_relPathClick
    end
    object Bt_hide: TButton
      Left = 3
      Top = 215
      Width = 946
      Height = 23
      Caption = 'Hide'
      TabOrder = 16
      OnClick = Bt_hideClick
    end
    object Bt_sh_nameE: TButton
      Left = 16
      Top = 154
      Width = 122
      Height = 25
      Caption = 'Name + ext'
      TabOrder = 17
      OnClick = Bt_sh_nameEClick
    end
    object Bt_sh_rn: TButton
      Left = 414
      Top = 90
      Width = 116
      Height = 25
      Caption = 'rn'
      TabOrder = 18
      OnClick = Bt_sh_rnClick
    end
  end
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 693
    Width = 987
    Height = 17
    Align = alBottom
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 0
    Top = 663
    Width = 987
    Height = 30
    Align = alBottom
    TabOrder = 3
    DesignSize = (
      987
      30)
    object Bt_saveToFile: TButton
      Left = 857
      Top = 0
      Width = 126
      Height = 27
      Anchors = [akRight, akBottom]
      Caption = 'Save'
      TabOrder = 0
      OnClick = Bt_saveToFileClick
    end
    object Bt_clearMemory: TButton
      Left = 730
      Top = 0
      Width = 126
      Height = 27
      Anchors = [akRight, akBottom]
      Caption = 'Clear memory'
      TabOrder = 1
      OnClick = Bt_clearMemoryClick
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 560
    Top = 136
  end
  object SaveDialog1: TSaveDialog
    Left = 704
    Top = 144
  end
  object MM: TMainMenu
    Left = 768
    Top = 144
    object MM_file: TMenuItem
      Caption = 'File'
      object MM_filr_openFile: TMenuItem
        Caption = 'Open file'
        OnClick = MM_filr_openFileClick
      end
      object MM_filr_openFolder: TMenuItem
        Caption = 'Open folder'
        OnClick = MM_filr_openFolderClick
      end
    end
    object MM_settings: TMenuItem
      Caption = 'Settings'
      object MM_settings_lang: TMenuItem
        Caption = 'Language'
        object N1: TMenuItem
          Caption = #1056#1091#1089#1089#1082#1080#1081
          OnClick = N1Click
        end
        object English1: TMenuItem
          Caption = 'English'
          OnClick = English1Click
        end
      end
      object MM_settings_templClear: TMenuItem
        Caption = 'Clear template'
        OnClick = MM_settings_templClearClick
      end
    end
  end
end
