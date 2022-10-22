#include "winversion.h"

typedef void (WINAPI *RtlGetVersionFunc)(OSVERSIONINFOEXW *);

BOOL GetVersion2(OSVERSIONINFOEX *os);

BOOL GetOSDisplayString(LPTSTR osname, LPTSTR osver, DWORD* osbuild)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bOsVersionInfoEx = GetVersion2(&osvi);
	if (bOsVersionInfoEx == FALSE)
	{
		bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
	}

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), 
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);


	if ((bOsVersionInfoEx == TRUE) && ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId))
	{
		*osbuild = osvi.dwBuildNumber;
		_stprintf( osver,TEXT("%d.%d"), osvi.dwMajorVersion, osvi.dwMinorVersion);

//		_tcscpy(osname, TEXT("Microsoft "));

		// Test for the specific product.
		if ( osvi.dwMajorVersion == 10 )
		{
			pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), 
				"GetProductInfo");

			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			if( osvi.dwMinorVersion == 0 )
			{
				char edition[64];

				if( osvi.wProductType == VER_NT_WORKSTATION )
				{
					switch( dwType )
					{
						case PRODUCT_PROFESSIONAL:
						case PRODUCT_PROFESSIONAL_N:
							_tcscpy(edition, TEXT("Pro"));
							break;
						case PRODUCT_ENTERPRISE:
						case PRODUCT_ENTERPRISE_E:
						case PRODUCT_ENTERPRISE_N:
							_tcscpy(edition, TEXT("Enterprise"));
							break;
						case PRODUCT_CORE:
						case PRODUCT_CORE_N:
						case PRODUCT_CORE_SINGLELANGUAGE:
							_tcscpy(edition, TEXT("Home"));
							break;
						case PRODUCT_EDUCATION:
						case PRODUCT_EDUCATION_N:
							_tcscpy(edition, TEXT("Education"));
							break;
						case PRODUCT_PRO_WORKSTATION:
						case PRODUCT_PRO_WORKSTATION_N:
							_tcscpy(edition, TEXT("Pro For Workstations"));
							break;
						case PRODUCT_MOBILE_CORE:
							_tcscpy(edition, TEXT("Mobile"));
							break;
						case PRODUCT_MOBILE_ENTERPRISE:
							_tcscpy(edition, TEXT("Mobile Enterprise"));
							break;
						case PRODUCT_IOTUAP:
							_tcscpy(edition, TEXT("IoT Core"));
							break;
						case PRODUCT_IOTENTERPRISE:
							_tcscpy(edition, TEXT("IoT Enterprise"));
							break;
					}

					switch(osvi.dwBuildNumber)
					{
					case 22621:
						_stprintf(osname, TEXT("Windows 11 %s 22H2"), edition);
						break;
					case 22000:
						_stprintf(osname, TEXT("Windows 11 %s 21H2"), edition);
						break;
					case 19045:
						_stprintf(osname, TEXT("Windows 10 %s 22H2"), edition);
						break;
					case 19044:
						_stprintf(osname, TEXT("Windows 10 %s 21H2"), edition);
						break;
					case 19043:
						_stprintf(osname, TEXT("Windows 10 %s 21H1"), edition);
						break;
					case 19042:
						_stprintf(osname, TEXT("Windows 10 %s 20H2"), edition);
						break;
					case 19041:
						_stprintf(osname, TEXT("Windows 10 %s 2004"), edition);
						break;
					case 18363:
						_stprintf(osname, TEXT("Windows 10 %s 1909"), edition);
						break;
					case 18362:
						_stprintf(osname, TEXT("Windows 10 %s 1903"), edition);
						break;
					case 17763:
						_stprintf(osname, TEXT("Windows 10 %s 1809"), edition);
						break;
					case 17134:
						_stprintf(osname, TEXT("Windows 10 %s 1803"), edition);
						break;
					case 16299:
						_stprintf(osname, TEXT("Windows 10 %s 1709"), edition);
						break;
					case 15063:
						_stprintf(osname, TEXT("Windows 10 %s 1703"), edition);
						break;
					case 14393:
						_stprintf(osname, TEXT("Windows 10 %s 1607"), edition);
						break;
					case 10586:
						_stprintf(osname, TEXT("Windows 10 %s 1511"), edition);
						break;
					case 10240:
						_stprintf(osname, TEXT("Windows 10 %s 1507"), edition);
						break;
					}
				}
				else
				{
					switch(osvi.dwBuildNumber)
					{
					case 20348:
						_stprintf(osname, TEXT("Windows Server 2022"));
						break;
					case 17763:
						_stprintf(osname, TEXT("Windows Server 2019"));
						break;
					case 14393:
						_stprintf(osname, TEXT("Windows Server 2016"));
						break;
					}
				}
			}
		}

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					 _tcscat(osname, TEXT("Windows Vista "));
				else _tcscat(osname, TEXT("Windows Server 2008 " ));
			}
			else if ( osvi.dwMinorVersion == 1 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					 _tcscat(osname, TEXT("Windows 7 "));
				else _tcscat(osname, TEXT("Windows Server 2008 R2 " ));
			}
			else if ( osvi.dwMinorVersion == 2 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					 _tcscat(osname, TEXT("Windows 8 "));
				else _tcscat(osname, TEXT("Windows Server 2012 "));
			}
			else if ( osvi.dwMinorVersion == 3 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					 _tcscat(osname, TEXT("Windows 8.1 "));
				else _tcscat(osname, TEXT("Windows Server 2012 R2 "));
			}
			
			pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), 
				"GetProductInfo");

			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			switch( dwType )
			{
				case PRODUCT_PROFESSIONAL:
				case PRODUCT_PROFESSIONAL_E:
				case PRODUCT_PROFESSIONAL_N:
					_tcscat(osname, TEXT("Professional" ));
					break;
				case PRODUCT_BUSINESS:
				case PRODUCT_BUSINESS_N:
					_tcscat(osname, TEXT("Business Edition" ));
					break;
				case PRODUCT_ULTIMATE:
				case PRODUCT_ULTIMATE_E:
				case PRODUCT_ULTIMATE_N:
					_tcscat(osname, TEXT("Ultimate Edition" ));
					break;
				case PRODUCT_HOME_PREMIUM:
				case PRODUCT_HOME_PREMIUM_E:
				case PRODUCT_HOME_PREMIUM_N:
					_tcscat(osname, TEXT("Home Premium Edition" ));
					break;
				case PRODUCT_HOME_BASIC:
				case PRODUCT_HOME_BASIC_E:
				case PRODUCT_HOME_BASIC_N:
					_tcscat(osname, TEXT("Home Basic Edition" ));
					break;
				case PRODUCT_HOME_SERVER:
					_tcscat(osname, TEXT("Home Server" ));
					break;
				case PRODUCT_HOME_PREMIUM_SERVER:
					_tcscat(osname, TEXT("Home Premium Server" ));
					break;
				case PRODUCT_ENTERPRISE:
				case PRODUCT_ENTERPRISE_E:
				case PRODUCT_ENTERPRISE_N:
					_tcscat(osname, TEXT("Enterprise Edition" ));
					break;
				case PRODUCT_STARTER:
				case PRODUCT_STARTER_E:
				case PRODUCT_STARTER_N:
					_tcscat(osname, TEXT("Starter Edition" ));
					break;
				case PRODUCT_CLUSTER_SERVER:
				case PRODUCT_CLUSTER_SERVER_V:
					_tcscat(osname, TEXT("Cluster Server Edition" ));
					break;
				case PRODUCT_DATACENTER_SERVER:
				case PRODUCT_DATACENTER_SERVER_V:
					_tcscat(osname, TEXT("Datacenter Edition" ));
					break;
				case PRODUCT_DATACENTER_SERVER_CORE:
				case PRODUCT_DATACENTER_SERVER_CORE_V:
					_tcscat(osname, TEXT("Datacenter Edition (core installation)" ));
					break;
				case PRODUCT_ENTERPRISE_SERVER:
				case PRODUCT_ENTERPRISE_SERVER_V:
					_tcscat(osname, TEXT("Enterprise Edition" ));
					break;
				case PRODUCT_ENTERPRISE_SERVER_CORE:
				case PRODUCT_ENTERPRISE_SERVER_CORE_V:
					_tcscat(osname, TEXT("Enterprise Edition (core installation)" ));
					break;
				case PRODUCT_ENTERPRISE_SERVER_IA64:
					_tcscat(osname, TEXT("Enterprise Edition for Itanium-based Systems" ));
					break;
				case PRODUCT_SB_SOLUTION_SERVER:
				case PRODUCT_SB_SOLUTION_SERVER_EM:
				case PRODUCT_SERVER_FOR_SB_SOLUTIONS:
				case PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM:
				case PRODUCT_SERVER_FOR_SMALLBUSINESS:
				case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:
				case PRODUCT_SMALLBUSINESS_SERVER:
					_tcscat(osname, TEXT("Small Business Server" ));
					break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE:
					_tcscat(osname, TEXT("Small Business Server Premium Edition" ));
					break;
				case PRODUCT_STANDARD_SERVER:
				case PRODUCT_STANDARD_SERVER_SOLUTIONS:
				case PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE:
				case PRODUCT_STANDARD_SERVER_V:
					_tcscat(osname, TEXT("Standard Edition" ));
					break;
				case PRODUCT_STANDARD_SERVER_CORE:
				case PRODUCT_STANDARD_SERVER_CORE_V:
					_tcscat(osname, TEXT("Standard Edition (core installation)" ));
					break;
				case PRODUCT_WEB_SERVER:
				case PRODUCT_WEB_SERVER_CORE:
					_tcscat(osname, TEXT("Web Server Edition" ));
					break;
				case PRODUCT_STORAGE_ENTERPRISE_SERVER:
				case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:
				case PRODUCT_STORAGE_EXPRESS_SERVER:
				case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:
				case PRODUCT_STORAGE_STANDARD_SERVER:
				case PRODUCT_STORAGE_STANDARD_SERVER_CORE:
				case PRODUCT_STORAGE_WORKGROUP_SERVER:
				case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:
					_tcscat(osname, TEXT("Storage Server" ));
					break;
				case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL:
				case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC:
				case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT:
				case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC:
					_tcscat(osname, TEXT("Essential Business Server" ));
					break;
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			if( GetSystemMetrics(SM_SERVERR2) )
				_tcscat(osname, TEXT( "Windows Server 2003 R2, "));
			else if ( osvi.wSuiteMask==VER_SUITE_STORAGE_SERVER )
				_tcscat(osname, TEXT( "Windows Storage Server 2003"));
			else if ( osvi.wSuiteMask==VER_SUITE_WH_SERVER )
				_tcscat(osname, TEXT( "Windows Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
						si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				_tcscat(osname, TEXT( "Windows XP Professional x64 Edition"));
			}
			else _tcscat(osname, TEXT("Windows Server 2003, "));

			// Test for the server type.
			if ( osvi.wProductType != VER_NT_WORKSTATION )
			{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
				{
					 if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						 _tcscat(osname, TEXT( "Datacenter Edition for Itanium-based Systems" ));
					 else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						 _tcscat(osname, TEXT( "Enterprise Edition for Itanium-based Systems" ));
				}

				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					 if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						 _tcscat(osname, TEXT( "Datacenter x64 Edition" ));
					 else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						 _tcscat(osname, TEXT( "Enterprise x64 Edition" ));
					 else _tcscat(osname, TEXT( "Standard x64 Edition" ));
				}

				else
				{
					 if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						 _tcscat(osname, TEXT( "Compute Cluster Edition" ));
					 else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						 _tcscat(osname, TEXT( "Datacenter Edition" ));
					 else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						 _tcscat(osname, TEXT( "Enterprise Edition" ));
					 else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						 _tcscat(osname, TEXT( "Web Edition" ));
					 else _tcscat(osname, TEXT( "Standard Edition" ));
				}
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			_tcscat(osname, TEXT("Windows XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				_tcscat(osname, TEXT( "Home Edition" ));
			else _tcscat(osname, TEXT( "Professional" ));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			_tcscat(osname, TEXT("Windows 2000 "));

			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				_tcscat(osname, TEXT( "Professional" ));
			}
			else 
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					_tcscat(osname, TEXT( "Datacenter Server" ));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					_tcscat(osname, TEXT( "Advanced Server" ));
				else _tcscat(osname, TEXT( "Server" ));
			}
		}

		if ( osvi.dwMajorVersion < 5)
		{
			_tcscat(osname, TEXT("Windows NT "));

			TCHAR ver[20];

			_stprintf( ver,TEXT("%d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
			_tcscat(osname, ver);

			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				_tcscat(osname, TEXT( "Workstation" ));
			}
			else 
			{
				_tcscat(osname, TEXT( "Server" ));
			}
		}

		 // Include service pack (if any) and build number.
/*
		if( _tcslen(osvi.szCSDVersion) > 0 )
		{
			 _tcscat(osname, TEXT(" ") );
			 _tcscat(osname, osvi.szCSDVersion);
		}

		TCHAR buf[80];

		_stprintf( buf, TEXT(" (build %d)"), osvi.dwBuildNumber);
		_tcscat(osname, buf);
*/
		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				_tcscat(osname, TEXT( ", 64-bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				_tcscat(osname, TEXT(", 32-bit"));
		}
		
		return TRUE; 
	}

	else
	{
		if (bOsVersionInfoEx == FALSE)
		{
			DWORD dwVersion = GetVersion();
			osvi.dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
			osvi.dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

			// Get the build number.
			if (dwVersion < 0x80000000)
			{
				osvi.dwBuildNumber = (DWORD)(HIWORD(dwVersion));
			}
		}

		*osbuild = osvi.dwBuildNumber;
		_stprintf( osver,TEXT("%d.%d"), osvi.dwMajorVersion, osvi.dwMinorVersion);
		
		if(osvi.dwPlatformId == 1) { // Win 9x/ME
//			_tcscpy(osname, TEXT("Microsoft "));
			if(osvi.dwMajorVersion == 4) {
				if(osvi.dwMinorVersion == 0) {
					_tcscat(osname, TEXT("Windows 95"));
					if(osvi.dwBuildNumber > 950 && osvi.dwBuildNumber < 1080)
						_tcscat(osname, TEXT(" SP1"));
					else if(osvi.dwBuildNumber >= 1080)
						_tcscat(osname, TEXT(" OSR2"));
					else if(osvi.dwBuildNumber >= 1212)
						_tcscat(osname, TEXT(" OSR2.5"));
				}
				if(osvi.dwMinorVersion == 10) {
					_tcscat(osname, TEXT("Windows 98"));
					if(osvi.dwBuildNumber > 1998 && osvi.dwBuildNumber < 2183)
						_tcscat(osname, TEXT(" SP1"));
					else if(osvi.dwBuildNumber > 2183)
						_tcscat(osname, TEXT(" SE"));
				}
				if(osvi.dwMinorVersion == 90) {
					_tcscat(osname, TEXT("Windows ME"));
				}
			}
			return TRUE;
		}

		else if(osvi.dwPlatformId == 0 || osvi.dwPlatformId == 2) { // Win NT 3.x / Win32s
			_tcscpy(osname, TEXT("Windows "));
			if(osvi.dwBuildNumber <= 528) {
				_tcscat(osname, TEXT("NT 3.1"));
				return TRUE;
			}
			else if(osvi.dwBuildNumber <= 807) {
				_tcscat(osname, TEXT("NT 3.5"));
				return TRUE;
			}
			else if(osvi.dwBuildNumber <= 1057) {
				_tcscat(osname, TEXT("NT 3.51"));
				return TRUE;
			}
			else {
				_tcscat(osname, TEXT("3.1"));
			}
			return TRUE;
		}

		else if(osvi.dwPlatformId == 3) { // Win CE
			_tcscpy(osname, TEXT("Windows CE "));
			TCHAR ver[20];

			_stprintf( ver,TEXT("%d.%d"), osvi.dwMajorVersion, osvi.dwMinorVersion);
			_tcscat(osname, ver);
			return TRUE;
		}

//		printf( "This sample does not support this version of Windows.\n");

		return FALSE;
	}
}

// RtlGetVersion‚ÍUnicode”Å‚Ì‚Ý
BOOL GetVersion2(OSVERSIONINFOEX *os)
{
#ifdef UNICODE
    OSVERSIONINFOEXW *osw = os;
#else
    OSVERSIONINFOEXW o;
    OSVERSIONINFOEXW *osw = &o;
#endif

    HMODULE hMod = LoadLibrary(TEXT("ntdll.dll"));
    if(!hMod)
    {
        return FALSE;
    }

    RtlGetVersionFunc pRtlGetVersion = (RtlGetVersionFunc)GetProcAddress(hMod, "RtlGetVersion");
    if(pRtlGetVersion == 0)
    {
        FreeLibrary(hMod);
        return FALSE;
    }

    ZeroMemory(osw,sizeof(*osw));
    osw->dwOSVersionInfoSize = sizeof(*osw);
    pRtlGetVersion(osw);
#ifndef UNICODE
    os->dwOSVersionInfoSize = sizeof(*os);
    os->dwMajorVersion    = osw->dwMajorVersion;
    os->dwMinorVersion    = osw->dwMinorVersion;
    os->dwBuildNumber     = osw->dwBuildNumber;
    os->dwPlatformId      = osw->dwPlatformId;
    os->wServicePackMajor = osw->wServicePackMajor;
    os->wServicePackMinor = osw->wServicePackMinor;
    os->wSuiteMask        = osw->wSuiteMask;
    os->wProductType      = osw->wProductType;

	size_t num;
	wcstombs_s(&num,
		os->szCSDVersion,  sizeof(os->szCSDVersion),
		osw->szCSDVersion, sizeof(osw->szCSDVersion));
#endif

    FreeLibrary(hMod);

    return TRUE;
}


/*
int __cdecl _tmain()
{
	 TCHAR szOS[BUFSIZE];

	 if( GetOSDisplayString( szOS ) )
			_tprintf( TEXT("\n%s\n"), szOS );
}
*/
