#include <windows.h>
#include "resource.h"

#include "xlbe.h"
#include "FishApplication.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   char * lpCmdLine,int nCmdShow )
{

    try
    {
        srand((unsigned)time(NULL));

        CFishApplication application("¡‘”„∏ﬂ ÷", "fish\\fish.cfg", lpCmdLine);
        application.exec();
    }
    catch (const base_error &error)
    {
        std::ofstream ofs("log.txt");
        ofs << error.what();
    }
    //catch (...)
    //{
    //    MessageBox(0,"fff","ff",0);
    //}

    return 0;
}

