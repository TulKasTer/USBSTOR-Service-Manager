/*
    File Name: USBPortState.c
    Description: 
        This program is designed to block USB ports on a Windows system.
        It modifies the registry to disable USB storage devices, preventing unauthorized access to the system via USB drives.
        The program requires administrative privileges to run successfully.


        Author: TulKasTer
        License: MIT License
        Created Date: 2026-03-03
        Last Modified Date: 2026-03-03
*/

// Include necessary headers
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winreg.h>
#include <stdbool.h>

// Define registry path and values for USBSTOR
#define REG_PATH_USBSTOR _T("SYSTEM\\CurrentControlSet\\Services\\USBSTOR")
#define REG_VALUE_START _T("Start")
#define USBSTOR_ENABLED 3
#define USBSTOR_DISABLED 4

// setUSBPortState
// @details: function enables or disables USB ports by modifying the registry value for USBSTOR.
// @param:
    /*
        enable - A boolean value indicating whether to enable (TRUE) or disable (FALSE) USB ports.
        disable - A boolean value indicating whether to disable (TRUE) or enable (FALSE) USB ports.    
    */
// @return: None
// @note: Administrative privileges to modify the registry.
//! @warning: 
    /* 
        Modifying the registry can have unintended consequences.
        Ensure you understand the changes being made and have a backup of your registry before running this program.
    */
// @complexity:
    /*
        Time: O(1) - The function performs a constant number of operations regardless of input size.
        Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.

    */
void setUSBPortState(BOOL enable) {
    HKEY hKey;
    DWORD dwValue = enable ? USBSTOR_ENABLED : USBSTOR_DISABLED;

    // Open the registry key for USBSTOR
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH_USBSTOR, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        // Set the value to enable or disable USB storage

        if (RegSetValueEx(hKey, REG_VALUE_START, 0, REG_DWORD, (const BYTE*)&dwValue, sizeof(dwValue)) == ERROR_SUCCESS) {
            printf("USB ports have been %s.\n", enable ? "enabled" : "disabled");

        } else {
            printf("Failed to set registry value. Error code: %lu\n", GetLastError());

        }
        RegCloseKey(hKey);

    } else {
        printf("Failed to open registry key. Error code: %lu\n", GetLastError());
    }

}

// checkUSBPortStatus
// @details: 
    /*
        function checks the current status of USB ports by querying the registry value for USBSTOR.
        It reports whether USB ports are enabled or disabled based on the value retrieved from the registry.
    */
// @param:
    /*
        None - This function does not take any parameters.  
    */
// @return: None
// @note: Administrative privileges to modify the registry.
//! @warning: 
    /* 
        Modifying the registry can have unintended consequences.
        Ensure you understand the changes being made and have a backup of your registry before running this program.
    */
// @complexity:
    /*
        Time: O(1) - The function performs a constant number of operations regardless of input size.
        Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.

    */
void checkUSBPortStatus() {
    HKEY hKey;
    DWORD dwValue;
    DWORD dwSize = sizeof(dwValue);

    // Open the registry key for USBSTOR
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH_USBSTOR, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        
        // Query the value to check if USB storage is enabled or disabled
        if (RegQueryValueEx(hKey, REG_VALUE_START, NULL, NULL, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS) {

            if (dwValue == USBSTOR_ENABLED) {
                printf("USB ports are currently enabled.\n");

            } else if (dwValue == USBSTOR_DISABLED) {
                printf("USB ports are currently disabled.\n");

            } else {
                printf("Unknown USB port status. Value: %lu\n", dwValue);
            }

        } else {
            printf("Failed to query registry value. Error code: %lu\n", GetLastError());
        }

        RegCloseKey(hKey);

    } else {
        printf("Failed to open registry key. Error code: %lu\n", GetLastError());
    }

}

// printUsage
// @details: 
    /*
        function prints the usage instructions for the program, detailing how to use the command-line arguments to enable, disable, or check the status of USB ports.
    */
// @param:
    /*
        None - This function does not take any parameters.  
    */
// @return: None
// @note: None
//! @warning: None
// @complexity:
    /*
        Time: O(1) - The function performs a constant number of operations regardless of input size.
        Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.

    */
void printUsage() {
    printf("Usage: USBPortState.exe [enable|disable|status]\n");
    printf("enable - Enable USB ports\n");
    printf("disable - Disable USB ports\n");
    printf("status - Check current USB port status\n");
}


// execute
// @details: 
    /*
        function serves as the entry point for the program, parsing command-line arguments to determine whether to enable, disable, or check the status of USB ports, and then calling the appropriate functions based on the user's input.
    */
// @param:
    /*
        argc - The number of command-line arguments passed to the program.
        argv - An array of strings representing the command-line arguments.
    */
// @return: int - The exit code of the program, where 0 indicates success and non-zero indicates an error.
// @note: Administrative privileges to modify the registry when enabling or disabling USB ports.
//! @warning: 
    /* 
        Modifying the registry can have unintended consequences.
        Ensure you understand the changes being made and have a backup of your registry before running this program.
    */
// @complexity:
    /*
        Time: O(1) - The function performs a constant number of operations regardless of input size.
        Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.

    */
void execute(int argc, char* argv[]){
    if (argc != 2) {
        printUsage();
        return;
    }

    if (strcmp(argv[1], "enable") == 0) {
        setUSBPortState(TRUE);

    } else if (strcmp(argv[1], "disable") == 0) {
        setUSBPortState(FALSE);

    } else if (strcmp(argv[1], "status") == 0) {
        checkUSBPortStatus();

    } else {
        printUsage();
    }
}

// main function
int main(int argc, char* argv[]){
    execute(argc, argv);
    return 0;
}