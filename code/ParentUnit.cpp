//---------------------------------------------------------------------------

#include <vcl.h>
//#include "GlobalDeclarations.h"
#pragma hdrstop

#include "ParentUnit.h"
#include "DMUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TParentForm *ParentForm;
//---------------------------------------------------------------------------
__fastcall TParentForm::TParentForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString TParentForm::addYe(AnsiString s1)
{
  char s2[60];
  int i,j;
  for(i=1,j=0;i<=s1.Length();i++)
     if(s1[i]=='�' || s1[i]=='�' ||s1[i]=='�' || s1[i]=='�')
     {
       switch(s1[i])
       {
        case '�': s2[j++]='�'; break;
        case '�': s2[j++]='�'; break;
        case '�': s2[j++]='�'; break;
        case  '�': s2[j++]='�'; break;
       }
      s2[j++]='�';
      s2[j++]='�';
      s2[j++]='�';
     }
     else s2[j++]=s1[i];

   s2[j]='\0';
   return(AnsiString(s2));
}
//---------------------------------------------------------------------------
