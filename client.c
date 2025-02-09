#include <windows.h>
#include <stdio.h>
#include <strings.h>

void handleOut(char* o)
{
  // replace this with your own exfiltration logic 
  printf("%s", o);
}

char* sysInfo()
{
  //cpuinfo vars
  char cpuInfo[256];
  HKEY hKey;
  DWORD bufferSize = sizeof(cpuInfo);

  //os version vars
  OSVERSIONINFO osvi;
  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));

  char* finalOutput = (char*)malloc(2048);
  //pcname vars
  char temp[2048];
  char computerName[MAX_COMPUTERNAME_LENGTH + 1];
  DWORD size = sizeof(computerName);
  finalOutput[0] = '\0';

  GetComputerName(computerName, &size);

  snprintf(temp, sizeof(temp), "PC Name: %s\n", computerName);
  strcat(finalOutput, temp);

  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  snprintf(temp, sizeof(temp), "Windows %d.%d Build %d\n", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
  strcat(finalOutput, temp);

  if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    if(RegQueryValueEx(hKey, "ProcessorNameString", 0, 0, (LPBYTE)cpuInfo, &bufferSize) == ERROR_SUCCESS)
    {
      RegCloseKey(hKey);
      snprintf(temp, sizeof(temp), "CPU: %s\n", cpuInfo);
      strcat(finalOutput, temp);
    }
  }
  
  return finalOutput;
}

char* driveEnum()
{
  // get username

  char username[256];
  DWORD usernameSize = sizeof(username);

  if(GetUserName(username, &usernameSize))
  {
    ;
  }

  ULARGE_INTEGER totUserBytes;
  ULARGE_INTEGER totBytes;
  ULARGE_INTEGER totFreeBytes;

  char* finalOutput = (char*)malloc(2048);
  ZeroMemory(finalOutput, 2048);
  char temp[2048];
  char buf[MAX_PATH];
  int dType;
  char* dTypeStr;
  DWORD bufSize = GetLogicalDriveStrings(MAX_PATH, buf);
  if(bufSize <= 0)
  {
    ExitProcess(1);
  }
  
  char* drive = buf;
  while(*drive != '\0')
  {
    dType = GetDriveType(drive);
    switch(dType)
    {
      case DRIVE_UNKNOWN:
        dTypeStr = "Unknown";
        break;
      case DRIVE_NO_ROOT_DIR:
        dTypeStr = "Invalid root directory";
        break;
      case DRIVE_REMOVABLE:
        dTypeStr = "Removable drive";
        break;
      case DRIVE_FIXED:
        dTypeStr = "Fixed drive (HDD/SSD, etc.)";
        break;
      case DRIVE_REMOTE:
        dTypeStr = "Network drive";
        break;
      case DRIVE_CDROM:
        dTypeStr = "CD-ROM";
        break;
      case DRIVE_RAMDISK:
        dTypeStr = "RAM disk";
        break;
      default:
        dTypeStr = "Unknown";
        break;
    }
    snprintf(temp, sizeof(temp), "Drive: %s | Type: %s\n", drive, dTypeStr);
    strcat(finalOutput, temp);

    if(GetDiskFreeSpaceEx(drive, &totUserBytes, &totBytes, &totFreeBytes) != 0)
    {
      snprintf(temp, sizeof(temp), "\t\tFree space for %s: %.fGB\n\t\tTotal GB: %.fGB\n\t\tTotal free GB: %.fGB\n", username, (double)totUserBytes.QuadPart / (1024 * 1024 * 1024), (double)totBytes.QuadPart / (1024 * 1024 * 1024), (double)totFreeBytes.QuadPart / (1024 * 1024 * 1024));
      strcat(finalOutput, temp);
    }

    drive += strlen(drive) + 1;
  }
  return finalOutput;
}

int main()
{
  char* systeminfo = sysInfo();
  char* drives = driveEnum();
  handleOut(drives);
  handleOut(systeminfo);
  free(systeminfo);
  free(drives);
  Sleep(10);
  return 0;
}
