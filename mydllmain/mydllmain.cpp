// mydllmain.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "mydllmain.h"


// This is an example of an exported variable
MYDLLMAIN_API int nmydllmain=0;

// This is an example of an exported function.
MYDLLMAIN_API int fnmydllmain(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cmydllmain::Cmydllmain()
{
    return;
}
