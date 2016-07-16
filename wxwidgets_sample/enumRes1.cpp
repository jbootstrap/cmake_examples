#include <wx/wx.h>
int main()
{
   wxInitializer wx;
   if( !wx.IsOk() ) return -1;
   // threw a run-time exception
   //wxMessageBox(wxT("Press OK"), wxT("Prompt"), wxOK);
   printf("ABC\n");
   return 0;
}
