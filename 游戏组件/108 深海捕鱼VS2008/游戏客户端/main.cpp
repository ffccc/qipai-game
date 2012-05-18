#include <windows.h>

#include "utility/exception.h"
#include "FishApplication.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   char * lpCmdLine,int nCmdShow )
{

  try
  {
      CFishApplication application("fish", lpCmdLine);
      application.exec();
  }
  catch (const base_error &error)
  {
      std::ofstream ofs("log.txt");
      ofs << error.what();
      //MessageBox(0,error.what(), "error", 0);
  }

	return 0;
}

