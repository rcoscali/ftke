#include <stdio.h>
#include <string.h>
#include "kernel_log.h"
#include "logo.c"

int main(int argc, char* argv[])
{
    struct FB fb = {0};
	log_init();
   
	load_565rle_image("/ftk.rle");    

	return 0;
}
