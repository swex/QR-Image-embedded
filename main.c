#include "QR_Encode.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc<3) {
    printf("qrencode <input file> <output file> <QR Code version>\n");
    return 1;
  }

  int version=3;
  if(argc>3) {
    version = atoi(argv[3]);
  }

  FILE *inputfile = fopen(argv[1],"r");
  char inputdata[10000];
  int n;
  for(n=0;(!feof(inputfile)) && (n < 10000);n++) {
    int c = getc(inputfile);
    inputdata[n] = c;
    inputdata[n+1]=0;
  }

  BYTE QR_m_data[3917]; //max possible bits resolution 177*177/8+1

  // int EncodeData(int nLevel, int nVersion , LPCSTR lpsSource, int sourcelen, unsigned char QR_m_data[])
  int QR_width=EncodeData(version,0,inputdata,0,QR_m_data);
  FILE *f=fopen(argv[2],"w");
  int size=((QR_width*QR_width)/8)+(((QR_width*QR_width)%8)?1:0);
  printf("\nwriting file %i bytes size \n",size);
  fwrite(QR_m_data,size,1,f);
  fclose(f);


  // This code dumps the QR code to the screen as ASCII.
  printf("QR Code width: %u\n",QR_width);

  int bit_count=0;
  for(n=0;n<size;n++) {
    int b=0;
    for(b=7;b>=0;b--) {
      if((bit_count%QR_width) == 0) printf("\n");
      if((QR_m_data[n] & (1 << b)) != 0) { printf("1"); }
                                    else { printf("0"); }
      bit_count++;
    }
  }

  return 0;
}
