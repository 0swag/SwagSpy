#include <windows.h>
#include <stdio.h>
#include <strings.h>

void driveEnum()
{
  char buf[MAX_PATH];
  DWORD bufSize = GetLogicalDriveStrings(MAX_PATH, buf);
  if(bufSize <= 0)
  {
    printf("error getting drives\n");
  }
  else 
  {
    printf("SIZE: %d\n", bufSize);
  }
  
  char* drive = buf;
  while(*drive != '\0')
  {
    printf("Drive: %s\n", drive);
    drive += strlen(drive) + 1;
  }
}

int main()
{
  driveEnum();
  return 0;
}
