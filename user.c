#include<string.h>
int password(int id,char password[]){// User id's are user1,user2,user3.
  if(id!=1&&id!=2&&id!=3) return 0; 
    char *passkey[]={"1234","cprjt","plot"};
    int k=0;
  if(!strcmp(password,passkey[(int)id-1])) k=1;
  return k;
}