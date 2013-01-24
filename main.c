#include "QR_Encode.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");
  if (argc<3) {
    wprintf(L"qrencode <input string> <output file> [QR Code level] [QR Code version]\n");
    wprintf(L"level and version default to 3 and 0\n");
    return 1;
  }

  int level=3;
  if(argc>3) {
    level = atoi(argv[3]);
  }

  int version=0;
  if(argc>4) {
    version = atoi(argv[4]);
  }

  // Read in the input data from file, terminate with 0.
  /*FILE *inputfile = fopen(argv[1],"r");
  char inputdata[10000];
  int n;
  for(n=0;(!feof(inputfile)) && (n < 10000);n++) {
    int c = getc(inputfile);
    inputdata[n] = c;
    inputdata[n+1]=0;
  }
*/
  
  // **** This calls the library and encodes the data
  // *** length is taken from NULL termination, however can also be passed by parameter.
  BYTE QR_m_data[3917]; //max possible bits resolution 177*177/8+1
  int QR_width=EncodeData(3,version,argv[1],0,QR_m_data);

  // Write the data to the output file
  FILE *f=fopen(argv[2],"w");
  int size=((QR_width*QR_width)/8)+(((QR_width*QR_width)%8)?1:0);
  wprintf(L"\nwriting file %i bytes size \n",size);
  fwrite(QR_m_data,size,1,f);
  fclose(f);


  // This code dumps the QR code to the screen as ASCII.
  wprintf(L"QR Code width: %u\n",QR_width);
  
  int bit_count=0;
  wprintf(L"%i",size);
  int n;
  for(n=0;n<size;n++) {
    int b=0;
    for(b=7;b>=0;b--) {
      
      if((bit_count%QR_width) == 0) wprintf(L"\n");
      if (((n+1)*8)-b>QR_width*QR_width){break;}
      if((QR_m_data[n] & (1 << b)) != 0) {wprintf(L"\u2588");wprintf(L"\u2588"); }
                                    else {wprintf(L" ");wprintf(L" "); }
      bit_count++;
    }
  }
  //wprintf(L"\n");
  return 0;
}
