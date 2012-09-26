#include "QR_Encode.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc<2) {
    printf("qrencode <input file> <output file>\n");
    return 1;
  }

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


  // This code dumps the QR code to the screen as ASCII.
  int bit_count=0;
  int n;
  for(n=0;n<size;n++) {
   
    int b=0;
    for(b=7;b>=0;b--) {
      if((bit_count%25) == 0) printf("\n");
      if((QR_m_data[n] & (1 << b)) != 0) { printf("1"); }
                                    else { printf("0"); }
      bit_count++;
    }


  }

  return 0;
}
