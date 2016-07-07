#include "sys.h"

int main(void)
{
    SysInit();
    while(1)
    {
		SysPoll();
    }
	//return 0;
}