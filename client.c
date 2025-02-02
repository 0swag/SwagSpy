#include <windows.h>
#include <stdio.h>
#include <strings.h>

void driveEnum()
{
  char finalOutput[2048];
  char temp[2048];
  char buf[MAX_PATH];
  int dType;
  char* dTypeStr;
  DWORD bufSize = GetLogicalDriveStrings(MAX_PATH, buf);
  if(bufSize <= 0)
  {
    printf("error getting drives\n");
  }
  
  char* drive = buf;
  while(*drive != '\0')
  {
    dType = GetDriveType(drive);
    switch(dType)
    {
      case DRIVE_UNKNOWN:
        dTypeStr = "Unknown ❓";
        break;
      case DRIVE_NO_ROOT_DIR:
        dTypeStr = "Invalid root directory ⚠️";
        break;
      case DRIVE_REMOVABLE:
        dTypeStr = "Removable drive 💾";
        break;
      case DRIVE_FIXED:
        dTypeStr = "Fixed drive (HDD/SSD, etc.) ";
        break;
      case DRIVE_REMOTE:
        dTypeStr = "Network drive 🌐";
        break;
      case DRIVE_CDROM:
        dTypeStr = "CD-ROM 💿";
        break;
      case DRIVE_RAMDISK:
        dTypeStr = "RAM disk";
        break;
      default:
        dTypeStr = "Unknown ❓";
        break;
    }
    snprintf(temp, sizeof(temp), "Drive: %s | Type: %s\n", drive, dTypeStr);
    strcat(finalOutput, temp);

    drive += strlen(drive) + 1;
  }
  printf("%s", finalOutput);
}

int main()
{
  char* webhookMsg;
  driveEnum();
  return 0;
}
