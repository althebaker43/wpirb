#include "LiveWindow.h"
#include <unistd.h>


LiveWindow* LiveWindow::ourInstance = NULL;


LiveWindow*
LiveWindow::GetInstance()
{
    if (ourInstance == NULL)
    {
        ourInstance = new LiveWindow();
    }

    return ourInstance;
}

void
LiveWindow::DestroyInstance()
{
    if (ourInstance != NULL)
    {
        delete ourInstance;
        ourInstance = NULL;
    }
}

LiveWindow::LiveWindow()
{
}

LiveWindow::~LiveWindow()
{
}

void
LiveWindow::Run()
{
}
