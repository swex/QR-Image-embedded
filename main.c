#include "QR_Encode.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
if (argc<2) return 1;
BYTE QR_m_data[3917]; //max possible bits resolution 177*177/8+1
FILE *f;
BYTE *p;
p=QR_m_data;
int QR_width=EncodeData(3,0,argv[1],0,p);
f=fopen("file.tmp","w");
int size=((QR_width*QR_width)/8)+(((QR_width*QR_width)%8)?1:0);
printf("\nwriting file %i bytes size \n",size);
fwrite(QR_m_data,size,1,f);
fclose(f);


return 0;
}
