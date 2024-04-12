//---------------------------------------------------------------------------
//���� ���� ���� �� ��� ��� ���� ���� �� ���� ���� ����� ��� � ��� ���
//�� ��� ��� ���� �� ���� �� ����� ���� �� ���� ���������� ��� ���� ��� ������� ���� �� ������ � ... ���� ������ ��
//��� ������ ���� �� ���� ��� ���� ���.
#include <vcl.h>
#pragma hdrstop

#include "RadehBandyUnit.h"
#include "RadehBandyRepUnit.h"
#include "FilteringUnit.h"
#include "DMUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ParentUnit"
#pragma resource "*.dfm"
TRadehBandyForm *RadehBandyForm;
struct TNode
        {
         AnsiString code;
//         AnsiString EMailAddress;
         int FacultyID;
         int GroupID;
         int TeacherID;
        };
typedef TNode *TNodeP;
//---------------------------------------------------------------------------
__fastcall TRadehBandyForm::TRadehBandyForm(TComponent* Owner)
        : TParentReportsForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRadehBandyForm::DoReport()
{
  TTreeNode *node = RotbehTV->Selected;
  AnsiString code=TNodeP(node->Data)->code,
        name=node->Text ;
  TNodeP nodeP=TNodeP(node->Data);

  if(code.Length()==0) return;
  if(code[code.Length()]=='0')//������� �� ���Ԑ��
  {
    Others->Add(FindEmail(DM->UnitInfo->FieldByName("EducationalAssistantID")->AsInteger));
    nodeP->TeacherID=DM->UnitInfo->FieldByName("UnitBossID")->AsInteger;
  }
  ToAddress->Add(FindEmail(nodeP->TeacherID));

  switch(code[1])
  {
    case '1':
        report("���� ��� ���� ������� �� �� ���� ����� ������","��� �������","FacultyName as Name","","FacultyName",true);
        break;
    case '2':
        if(code.Length()>1)
          switch(code[2])
          {
            case '0':
                report("���� ��� ���� ������ �� ���� ����� ������","��� ����","GroupName as Name","","GroupName",true);
                break;
            case '1':
                report("���� ��� ���� ������ �� ������� "+node->Text+" �� ���� ����� ������ ","��� ����","GroupName as Name","FacultyID="+IntToStr(nodeP->FacultyID),"GroupName",true);
                break;
          }
        break;
    case '3':
        if(code.Length()>1)
          switch(code[2])
          {
            case '0':
                report("���� ��� ���� ������ �� ���� ����� ������","��� �����","TeacherID,LastName,FirstName","","TeacherID,LastName,FirstName",false);
                break;
            case '1':
                report("���� ��� ���� ������ �� ������� "+node->Text+" �� ���� ����� ������","��� �����","TeacherID,LastName,FirstName","FacultyID="+IntToStr(nodeP->FacultyID),
                        "TeacherID,LastName,FirstName",false);
                break;
            case '2':
                report("���� ��� ���� ������ �� ���� "+node->Text+" �� ���� ����� ������","��� �����","TeacherID,LastName,FirstName"," GroupID="+IntToStr(nodeP->GroupID),
                        "TeacherID,LastName,FirstName",false);
                break;
          }
        break;
  }
}
//---------------------------------------------------------------------------
void TRadehBandyForm::report(AnsiString title,AnsiString locName,AnsiString sqlSelect,
                AnsiString sqlWhere,AnsiString sqlGroupBy,bool AezaeVisibility)
{
//  TTreeNode *node = RotbehTV->Selected;
//  TNodeP nodeP=TNodeP(node->Data);
//  ListBox1->Items->Add(nodeP->EMailAddress);
  TStrings *sql=new TStringList;
  TRadehBandyRep *QRep=new TRadehBandyRep(this);
  QRep->TitleLabel->Caption=title;
  QRep->LocNameQRLabel->Caption=locName;
  QRep->AezaeQRDBText->Enabled=AezaeVisibility;
  QRep->AezaeQRLabel->Enabled=AezaeVisibility;
  sql->Clear();
  sql->Add("SELECT "+sqlSelect);
  sql->Add(",AVG(LessonAverage)as Miangin ,MIN(LessonAverage) as Minimum,MAX(LessonAverage) as Maximum,COUNT(ID) as TDoroos,COUNT(DISTINCT TeacherID) as TTeachers");
  sql->Add(",SUM(IsLessonInRange) as NoofLessonsInRange, SUM(IsLessonUnderRange) as NoofLessonsUnderRange");

  if(AezaeVisibility)
  {
    QRep->QRExpr1->Expression="Query1.Name";
  }
  else
  {
    QRep->QRExpr1->Expression="Query1.LastName+' '+Query1.FirstName";
  }
  sql->Add(" FROM Flat1");
  if(sqlWhere.Length()>0)
          sql->Add(" WHERE "+sqlWhere);
  sql->Add(" GROUP BY "+sqlGroupBy);
  sql->Add(" ORDER BY Miangin DESC");
  QRep->Query1->Close();
  QRep->Query1->SQL->Clear();
  QRep->Query1->SQL->AddStrings(sql);
//  Memo1->Lines->AddStrings(sql);
//  Memo1->Refresh();
//  Memo1->Update();
  QRep->Query1->Open();
  QRep->NoDateAttachMemo->Enabled=NOVisibilityCheckBox->Checked;
  QRep->EhteramQRMemo->Enabled=EhteramCheckBox->Checked;
  PrintOrSave(QRep);
  if(SendMethodRadioGroup->ItemIndex==2) SendMail();
//  QRep->PreviewModal();
  delete QRep;
  delete sql;
}
//---------------------------------------------------------------------------
void __fastcall TRadehBandyForm::FormCreate(TObject *Sender)
{
        // let's add the node to the tree
 AnsiString text;
 TTreeNode* tempnode;
 //1: ���Ԙ�� ��
 TNodeP nodep=new TNode;
 nodep->code="10";
 tempnode=RotbehTV->Items->AddObject(RotbehTV->Selected,"���� ���� �������  �� �� ���Ԑ��",nodep);
//2: ���� ��
//20: ���� �� �� ���Ԑ��
//21: ���� �� �� ���Ԙ��
// RotbehTV->Selected=NULL;
 nodep=new TNode;
 nodep->code="";
 tempnode=RotbehTV->Items->AddObject(RotbehTV->Selected,"���� ���� ���� ��",nodep);
 tempnode->Selected = true;

 nodep=new TNode;
 nodep->code="20";
 tempnode=RotbehTV->Items->AddChildObject(RotbehTV->Selected,"���� ���� ������ �� ���Ԑ��",nodep);
 nodep=new TNode;
 nodep->code="";
 tempnode=RotbehTV->Items->AddChildObject(RotbehTV->Selected,"���� ���� ������ �� �������  ��",nodep);
 tempnode->Selected = true;

 Faculties->First();
 while(!Faculties->Eof)
 {
    nodep=new TNode;
    nodep->code="21";
    nodep->FacultyID=Faculties->FieldByName("FacultyID")->AsInteger;
    nodep->TeacherID=Faculties->FieldByName("FacultyBossID")->AsInteger;
    text=Faculties->FieldByName("FacultyName")->AsString;
    tempnode = RotbehTV->Items->AddChildObject(RotbehTV->Selected,text,nodep);
    Faculties->Next();
 }
//���� ���� ������
//30: �� ���Ԑ��
//31: �� �������
//32: �� ����
 RotbehTV->Selected=NULL;
 nodep=new TNode;
 nodep->code="";
 tempnode=RotbehTV->Items->AddObject(RotbehTV->Selected,"���� ���� ������",nodep);
 tempnode->Selected = true;
 nodep=new TNode;
 nodep->code="30";
 tempnode=RotbehTV->Items->AddChildObject(RotbehTV->Selected,"���� ���� ������ �� ���Ԑ��",nodep);
 nodep=new TNode;
 nodep->code="";
 tempnode=RotbehTV->Items->AddChildObject(RotbehTV->Selected,"���� ���� ������ �� ������� ��",nodep);
 tempnode->Selected = true;
 Faculties->First();
 while(!Faculties->Eof)
 {
    nodep=new TNode;
    nodep->code="31";
    nodep->FacultyID=Faculties->FieldByName("FacultyID")->AsInteger;
    nodep->TeacherID=Faculties->FieldByName("FacultyBossID")->AsInteger;
    text=Faculties->FieldByName("FacultyName")->AsString;
    tempnode = RotbehTV->Items->AddChildObject(RotbehTV->Selected,text,nodep);
    Faculties->Next();
 }
 nodep=new TNode;
 nodep->code="";
 tempnode=RotbehTV->Items->AddObject(RotbehTV->Selected,"���� ���� ������ �� ������",nodep);
 tempnode->Selected = true;
 Groups->First();
 while(!Groups->Eof)
 {
    nodep=new TNode;
    nodep->code="32";
    nodep->FacultyID=Groups->FieldByName("FacultyID")->AsInteger;
    nodep->GroupID=Groups->FieldByName("GroupID")->AsInteger;
    nodep->TeacherID=Groups->FieldByName("GroupBossID")->AsInteger;
    text=Groups->FieldByName("GroupName")->AsString;
    tempnode = RotbehTV->Items->AddChildObject(RotbehTV->Selected,text,nodep);
    Groups->Next();
 }
}
//---------------------------------------------------------------------------
/*   TTreeNode *CurItem = RotbehTV->Items->GetFirstNode();
while (CurItem)
{
  ListBox1->Items->Add(CurItem->Text+TNodeP(CurItem->Data)->code);
  CurItem = CurItem->GetNext();
}
*/
//---------------------------------------------------------------------------
void __fastcall TRadehBandyForm::FormShow(TObject *Sender)
{
//  FilteringForm->isItFirstTime=true;
//  FilteringForm->ShowModal();
}
//---------------------------------------------------------------------------
void TRadehBandyForm::Hazf(TTreeNode *node)
{
  if(node)
  {
//    ListBox1->Items->Add(node->Text);
    Hazf(node->GetNext());
    delete node->Data;
  }
}
//---------------------------------------------------------------------------
void __fastcall TRadehBandyForm::FormDestroy(TObject *Sender)
{
  Hazf(RotbehTV->Items->GetFirstNode());
}
//---------------------------------------------------------------------------
void __fastcall TRadehBandyForm::Button1Click(TObject *Sender)
{
  Label2->Caption= TNodeP(RotbehTV->Selected->Data)->code;
}
//---------------------------------------------------------------------------

void __fastcall TRadehBandyForm::SelectionBitBtnClick(TObject *Sender)
{
  FilteringForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TRadehBandyForm::RotbehTVClick(TObject *Sender)
{
  TTreeNode *node = RotbehTV->Selected;
  TNodeP nodeP=TNodeP(node->Data);
  if(nodeP->code!="") Label2->Caption=node->Text;
  else Label2->Caption="";
}
//---------------------------------------------------------------------------

