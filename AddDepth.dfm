object Form2: TForm2
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  ClientHeight = 83
  ClientWidth = 132
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bt_F2_dbP: TButton
    Left = 16
    Top = 43
    Width = 97
    Height = 25
    Caption = 'Enter'
    TabOrder = 0
    OnClick = Bt_F2_dbPClick
  end
  object Ed_F2_dbP: TEdit
    Left = 16
    Top = 16
    Width = 97
    Height = 21
    TabOrder = 1
    Text = 'Enter Depth'
    OnClick = Ed_F2_dbPClick
    OnKeyPress = Ed_F2_dbPKeyPress
  end
end
