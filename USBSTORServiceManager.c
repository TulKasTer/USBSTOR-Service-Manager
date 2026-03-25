/*
    File Name: USBPortStateManager.c
    Description: 
        This program is designed to block USB ports on a Windows system.
        It modifies the registry to disable USB storage devices, preventing unauthorized access to the system via USB drives.
        The program requires administrative privileges to run successfully.


        Author: TulKasTer
        License: MIT License
        Created Date: 2026-03-03
        Last Modified Date: 2026-03-25
*/

#ifdef _WIN32

    // Include necessary headers
    #include <stdio.h>
    #include <windows.h>
    #include <strsafe.h>
    #include <string.h>
    #include <stdbool.h>
    #include <errno.h>

    // Define registry path and values for USBSTOR
    #define REG_PATH_USBSTOR "SYSTEM\\CurrentControlSet\\Services\\USBSTOR"
    #define REG_VALUE_START "Start"
    #define USBSTOR_ENABLED 3
    #define USBSTOR_DISABLED 4


    // @name: rebootSystemWindows
    // @details: 
        /*
            function reboots the Windows system to apply changes made to the registry for USB port state.
            It prompts the user for confirmation before initiating the reboot process.
        */
    // @param: None
    // @return: None
    // @note: Administrative privileges to reboot the system.
    //! @warning: 
        /* 
            Rebooting the system will close all applications and may result in data loss if unsaved work is present.
            Ensure you have saved all work and closed applications before confirming the reboot.
        */
    // @complexity:
        /*
            Time: O(1) - The function performs a constant number of operations regardless of input size, as it only prompts the user and initiates a reboot if confirmed.
            Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.
        */
    void rebootSystemWindows(void) {
        // Reboot the system to apply changes
        if (MessageBox(NULL, "Changes to USB port state may require a system reboot to take effect. Do you want to reboot now?", "Reboot Required", MB_ICONQUESTION | MB_YESNO) == IDYES) {

            if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER)) {
                printf("Failed to reboot the system. Error code: %lu\n", GetLastError());

            }

        }

    }


    // @name: setUSBPortStateWindows
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
    void setUSBPortStateWindows(BOOL enable) {
        HKEY hKey;
        DWORD dwCurrent, dwSize = sizeof(dwCurrent);
        DWORD dwTarget = enable ? USBSTOR_ENABLED : USBSTOR_DISABLED;

        // Open the registry key for USBSTOR
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH_USBSTOR, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {

            if (RegQueryValueEx(hKey, REG_VALUE_START, NULL, NULL, (LPBYTE)&dwCurrent, &dwSize) == ERROR_SUCCESS) {

                if (dwCurrent == dwTarget) {
                    printf("USB ports are already %s.\n", enable ? "enabled" : "disabled");
                    RegCloseKey(hKey);
                    return;

                }

            } else {
                printf("Failed to query current registry value. Error code: %lu\n", GetLastError());
                RegCloseKey(hKey);
                return;

            }


            // Set the value to enable or disable USB storage

            if (RegSetValueEx(hKey, REG_VALUE_START, 0, REG_DWORD, (const BYTE*)&dwTarget, sizeof(dwTarget)) == ERROR_SUCCESS) {
                printf("USB ports have been %s.\n", enable ? "enabled" : "disabled");
                rebootSystemWindows();

            } else {
                printf("Failed to set registry value. Error code: %lu\n", GetLastError());

            }

            RegCloseKey(hKey);

        } else {
            printf("Failed to open registry key. Error code: %lu\n", GetLastError());

        }

    }


    // @name: checkUSBPortStatusWindows
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
    void checkUSBPortStatusWindows(void) {
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


    // @name: printUsage
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
    void printUsage(void) {
        printf("Usage: USBPortState.exe [enable|disable|status]\n");
        printf("enable - Enable USB ports\n");
        printf("disable - Disable USB ports\n");
        printf("status - Check current USB port status\n");

    }


    // @name: execute
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
    int execute(int argc, char* argv[]){
        if (argc != 2) {
            printUsage();
            return EXIT_FAILURE;

        }

        if (strcmp(argv[1], "enable") == 0) {
            setUSBPortStateWindows(TRUE);

        } else if (strcmp(argv[1], "disable") == 0) {
            setUSBPortStateWindows(FALSE);

        } else if (strcmp(argv[1], "status") == 0) {
            checkUSBPortStatusWindows();

        } else {
            printUsage();
            return EXIT_FAILURE;

        }

        return EXIT_SUCCESS;
    }

#elif __linux__
    // Include necessary headers
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <stdlib.h>

    // Define a global definitions for Linux USB port management
    #define BLACKLIST_DIR "/etc/modprobe.d"
    #define BLACKLIST_FILE BLACKLIST_DIR "/disable-usb-storage.conf"
    #define BLACKLIST_CONTENT "blacklist usb_storage\n"
    #define USB_MODULE "usb_storage"
    #define LSMOD_CMD "lsmod"

    // Define constants for Linux distribution detection
    #define RELEASE_OS "/etc/os-release"
    #define DISTRO_SIZE 32
    #define LINUX_DISTRO_UBUNTU "Ubuntu"
    #define LINUX_DISTRO_DEBIAN "Debian"
    #define LINUX_DISTRO_FEDORA "Fedora"
    #define LINUX_DISTRO_CENTOS "CentOS"
    #define LINUX_DISTRO_ARCH "Arch"
    #define LINUX_DISTRO_RHEL "Red Hat Enterprise Linux"
    #define LINUX_DISTRO_UNKNOWN "Unknown"

    // Define constants for initramfs update commands based on distribution
    #define UPDATE_INITRAMFS_DEBIAN_UBUNTU "update-initramfs -u"
    #define UPDATE_INITRAMFS_FEDORA_CENTOS_RHEL "dracut --force"
    #define UPDATE_INITRAMFS_ARCH "mkinitcpio -P"
    #define UPDATE_INITRAMFS_UNKNOWN "echo 'Unknown Linux distribution. Please update initramfs manually.'"

    // Global variables
    char linuxDistribution[DISTRO_SIZE] = LINUX_DISTRO_UNKNOWN;

    // @name: findLinuxDistribution
    // @details:
        /*
            function detects the Linux distribution by reading the /etc/os-release file and checking for specific keywords associated with popular distributions. It sets the global variable linuxDistribution accordingly, which is used later to determine the appropriate command for updating the initramfs after modifying USB port settings.
        */
    // @param:
        /*
            linuxDistribution - A character array that will be populated with the name of the detected Linux distribution.
        */
    // @return: None
    // @note: The function relies on the presence of the /etc/os-release file, which is common in most Linux distributions. If the file is missing or does not contain recognizable distribution information, the linuxDistribution variable will be set to "Unknown".
    //! @warning: None
    // @complexity:
        /*
            Time: O(n) - The function reads through the lines of the /etc/os-release file, where n is the number of lines in the file. The search for distribution keywords is performed on each line, resulting in a linear time complexity.
            Space: O(1) - The function uses a fixed amount of space for variables and does not allocate additional memory based on input size.
        */
    static void findLinuxDistribution(char *linuxDistribution) {
        FILE* file = fopen(RELEASE_OS, "r");

        if (file == NULL) {
            perror("Failed to open os-release file");
            return;

        }

        char line[256];

        while (fgets(line, sizeof(line), file)) {

            if (strstr(line, "Ubuntu") != NULL) {
                strncpy(linuxDistribution, LINUX_DISTRO_UBUNTU, DISTRO_SIZE - 1);
                linuxDistribution[DISTRO_SIZE - 1] = '\0';
                break;

            } else if (strstr(line, "Debian") != NULL) {
                strncpy(linuxDistribution, LINUX_DISTRO_DEBIAN, DISTRO_SIZE - 1);
                linuxDistribution[DISTRO_SIZE - 1] = '\0';
                break;

            } else if (strstr(line, "Fedora") != NULL) {
                strncpy(linuxDistribution, LINUX_DISTRO_FEDORA, DISTRO_SIZE - 1);
                linuxDistribution[DISTRO_SIZE - 1] = '\0';
                break;

            } else if (strstr(line, "CentOS") != NULL) {
                strncpy(linuxDistribution, LINUX_DISTRO_CENTOS, DISTRO_SIZE - 1);
                linuxDistribution[DISTRO_SIZE - 1] = '\0';
                break;

            } else if (strstr(line, "Arch") != NULL) {
                strncpy(linuxDistribution, LINUX_DISTRO_ARCH, DISTRO_SIZE - 1);
                linuxDistribution[DISTRO_SIZE - 1] = '\0';
                break;

            } else if (strstr(line, "Red Hat") != NULL) {
                strncpy(linuxDistribution, LINUX_DISTRO_RHEL, DISTRO_SIZE - 1);
                linuxDistribution[DISTRO_SIZE - 1] = '\0';
                break;

            }

        }

        fclose(file);
    }


    // @name: rebuidInitramfsLinux
    // @details:
        /*
            function updates the initramfs after modifying USB port settings to ensure that changes take effect on the next boot. It determines the appropriate command to update the initramfs based on the detected Linux distribution and executes it using the system() function.
        */
    // @param:
        /*
            linuxDistribution - A character array that contains the name of the detected Linux distribution, which is used to determine the appropriate command for updating the initramfs.
        */
    // @return: None
    // @note: The function relies on the presence of the /etc/os-release file for distribution detection. If the distribution is unknown, it will prompt the user to update the initramfs manually.
    //! @warning: None
    // @complexity:
        /*
            Time: O(1) - The function performs a constant number of operations regardless of input size, as it only checks the distribution name and executes a single command based on that.
            Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.
        */
    static void rebuildInitramfsLinux(char *linuxDistribution) {
        findLinuxDistribution(linuxDistribution);

        char command[256];

        if (strcmp(linuxDistribution, LINUX_DISTRO_UBUNTU) == 0 || strcmp(linuxDistribution, LINUX_DISTRO_DEBIAN) == 0) {
            snprintf(command, sizeof(command), "%s", UPDATE_INITRAMFS_DEBIAN_UBUNTU);

        } else if (strcmp(linuxDistribution, LINUX_DISTRO_FEDORA) == 0 || strcmp(linuxDistribution, LINUX_DISTRO_CENTOS) == 0 || strcmp(linuxDistribution, LINUX_DISTRO_RHEL) == 0) {
            snprintf(command, sizeof(command), "%s", UPDATE_INITRAMFS_FEDORA_CENTOS_RHEL);

        } else if (strcmp(linuxDistribution, LINUX_DISTRO_ARCH) == 0) {
            snprintf(command, sizeof(command), "%s", UPDATE_INITRAMFS_ARCH);

        } else {
            snprintf(command, sizeof(command), "%s", UPDATE_INITRAMFS_UNKNOWN);

        }

        system(command);
    }


    // @name: setUSBPortStateLinux
    // @details: 
        /*
            function enables or disables USB ports by creating or removing a blacklist file in
        */
    // @param:
        /*
            enable - An integer flag indicating whether to enable (1) or disable (0) USB ports.
        */
    // @return: None
    // @note: None
    //! @warning: None
    // @complexity:
        /*
            Time: O(1) - The function performs a constant number of operations regardless of input size.
            Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.
        */
    void setUSBPortStateLinux(int enable) {
        if (enable) {
            // Remove the blacklist file to enable USB storage

            if (remove(BLACKLIST_FILE) == 0) {
                printf("USB ports have been enabled.\n");
                system("modprobe " USB_MODULE " 2>/dev/null");
                rebuildInitramfsLinux(linuxDistribution);

            } else {

                if (errno == ENOENT) {
                    printf("USB ports are already enabled.\n");

                } else {
                    perror("Failed to enable USB ports");

                }
            }
        
        } else {
            struct stat st;

            // Check if the blacklist directory already exists
            if (stat(BLACKLIST_DIR, &st) != 0) {
                mkdir(BLACKLIST_DIR, 0755);

            }

            // Create the blacklist file to disable USB storage
            FILE* file = fopen(BLACKLIST_FILE, "w");

            if (file != NULL) {
                fprintf(file, "%s", BLACKLIST_CONTENT);
                fclose(file);
                printf("USB ports have been disabled.\n");
                system("modprobe -r " USB_MODULE " 2>/dev/null");
                rebuildInitramfsLinux(linuxDistribution);

            } else {
                perror("Failed to disable USB ports");

            }
        }
    }


    // @name: checkUSBPortStatusLinux
    // @details:
        /*
            function checks the current status of USB ports by executing the "lsmod" command and searching for the presence of the "usb_storage" module in the output. It reports whether USB ports are enabled or disabled based on whether the module is currently loaded in the kernel.
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
            Time: O(n) - The function reads through the output of the "lsmod" command, where n is the number of lines in the output. The search for the "usb_storage" module is performed on each line, resulting in a linear time complexity.
            Space: O(1) - The function uses a constant amount of space for variables and does not allocate additional memory based on input size.
        */
    void checkUSBPortStatusLinux(void) {
        char buffer[256];
        int loaded = 0;
        FILE* pipe = popen(LSMOD_CMD, "r");

        if (pipe == NULL) {
            perror("Failed to check USB port status");
            return;

        }

        while (fgets(buffer, sizeof(buffer), pipe)) {

            if (strstr(buffer, USB_MODULE) != NULL) {
                loaded = 1;
                break;

            }

        }
        
        pclose(pipe);

        struct stat st;
        int blacklisted = (stat(BLACKLIST_FILE, &st) == 0);

        printf("Kernel module loaded: %s\n", loaded      ? "YES" : "NO");
        printf("Blacklist file (persists): %s\n", blacklisted ? "YES — disabled on boot" : "NO");

        if (loaded && blacklisted){
            printf("Active now but will be blocked after reboot.\n");
        
        } else if (!loaded && blacklisted) {
            printf("Blocked now and will remain blocked after reboot.\n");
        
        } else if (loaded && !blacklisted) {
            printf("Active and no persistent block configured.\n");
        
        } else {
            printf("Not loaded and no persistent block.\n");

        }

    }


    // @name: printUsage
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
    void printUsage(void) {
        printf("Usage: USBPortState [enable|disable|status]\n");
        printf("enable - Enable USB ports\n");
        printf("disable - Disable USB ports\n");
        printf("status - Check current USB port status\n");
    }


    // @name: execute
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
    int execute(int argc, char* argv[]){
        if (argc != 2) {
            printUsage();
            return EXIT_FAILURE;
        }

        if (strcmp(argv[1], "enable") == 0) {
            setUSBPortStateLinux(1);

        } else if (strcmp(argv[1], "disable") == 0) {
            setUSBPortStateLinux(0);

        } else if (strcmp(argv[1], "status") == 0) {
            checkUSBPortStatusLinux();

        } else {
            printUsage();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }


#elif __APPLE__
    // Include necessary headers
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <sys/stat.h>

    // Define constants for macOS USB storage management
    #define KEXT_DRIVER  "com.apple.iokit.IOUSBMassStorageDriver"
    #define KEXT_CLASS   "com.apple.iokit.IOUSBMassStorageClass"
    #define PLIST_DIR    "/Library/LaunchDaemons"
    #define PLIST_FILE   "/Library/LaunchDaemons/com.usbstor.disable.plist"
    #define PLIST_CONTENT \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
        "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" " \
        "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n" \
        "<plist version=\"1.0\"><dict>\n" \
        "   <key>Label</key><string>com.usbstor.disable</string>\n" \
        "   <key>ProgramArguments</key><array>\n" \
        "       <string>/bin/sh</string><string>-c</string>\n" \
        "       <string>kextunload -b " KEXT_DRIVER " 2>/dev/null; " \
                        "kextunload -b " KEXT_CLASS  " 2>/dev/null</string>\n" \
        "   </array>\n" \
        "   <key>RunAtLoad</key><true/>\n" \
        "</dict></plist>\n"

    // @name: plistExists
    // @details:
        /*
            Helper function that checks whether the LaunchDaemon plist file
            used to persistently block USB storage is present on disk.
        */
    // @param: None
    // @return: int - 1 if the plist file exists, 0 otherwise.
    // @note: None
    //! @warning: None
    // @complexity:
        /*
            Time: O(1) - Single stat() call.
            Space: O(1) - No dynamic allocation.
        */
    static int plistExists(void) {
        struct stat st;
        return stat(PLIST_FILE, &st) == 0;
    }


    // @name: setUSBPortStateMacOS
    // @details:
        /*
            Enables or disables USB storage on macOS.
            Disable: unloads the kext immediately and installs a LaunchDaemon
                    that re-runs the unload on every boot.
            Enable:  removes the LaunchDaemon and loads the kext immediately.
        */
    // @param:
        /*
            enable - An integer flag: 1 to enable USB storage, 0 to disable it.
        */
    // @return: None
    // @note: Requires root privileges. On macOS 11+ the system volume is sealed,
    //        so kext modifications are session-only; the LaunchDaemon ensures
    //        persistence across reboots.
    //! @warning:
        /*
            Unloading the kext will immediately disconnect any mounted USB storage
            devices. Ensure no data is being written before disabling.
        */
    // @complexity:
        /*
            Time: O(1) - Constant number of file and system operations.
            Space: O(1) - No dynamic allocation.
        */
    void setUSBPortStateMacOS(int enable) {
        if (enable) {
            /* --- ENABLE --- */

            /* 1. Remove LaunchDaemon so it won't re-disable on next boot */
            if (plistExists()) {
                system("launchctl unload " PLIST_FILE " 2>/dev/null");

                if (remove(PLIST_FILE) == 0) {
                    printf("USB ports have been enabled.\n");

                } else {
                    perror("Failed to remove LaunchDaemon plist");
                    return;
                }

            } else {
                printf("USB ports are already enabled.\n");
                return;
            }

            /* 2. Load kext immediately (best-effort — may fail on Big Sur+) */
            int rc = system("kextload -b " KEXT_DRIVER " 2>/dev/null");
            if (rc != 0)
                system("kextload -b " KEXT_CLASS " 2>/dev/null");

            printf("[hint] A reboot ensures the kext is fully restored.\n");

        } else {
            /* --- DISABLE --- */

            if (plistExists()) {
                printf("USB ports are already disabled.\n");
                return;
            }

            /* 1. Unload kext immediately */
            int rc = system("kextunload -b " KEXT_DRIVER " 2>/dev/null");
            if (rc != 0)
                system("kextunload -b " KEXT_CLASS " 2>/dev/null");

            /* 2. Write LaunchDaemon for persistence across reboots */
            struct stat st;
            if (stat(PLIST_DIR, &st) != 0)
                mkdir(PLIST_DIR, 0755);

            FILE *f = fopen(PLIST_FILE, "w");
            if (!f) {
                perror("Failed to write LaunchDaemon plist");
                return;
            }

            fputs(PLIST_CONTENT, f);
            fclose(f);

            /* Set correct ownership and permissions */
            system("chown root:wheel " PLIST_FILE);
            system("chmod 644 " PLIST_FILE);
            system("launchctl load " PLIST_FILE " 2>/dev/null");

            printf("USB ports have been disabled.\n");
        }
    }


    // @name: checkUSBPortStatusMacOS
    // @details:
        /*
            Reports the current USB storage state on macOS by checking:
            1. Whether the kext is currently loaded (live state).
            2. Whether the LaunchDaemon plist is present (persistent state).
        */
    // @param: None
    // @return: None
    // @note: None
    //! @warning: None
    // @complexity:
        /*
            Time: O(1) - Single system() call and single stat() call.
            Space: O(1) - No dynamic allocation.
        */
    void checkUSBPortStatusMacOS(void) {
        int loaded     = (system("kextstat 2>/dev/null | grep -qi 'USBMassStorage'") == 0);
        int persistent = plistExists();

        printf("Kext loaded (active now)  : %s\n", loaded     ? "YES" : "NO");
        printf("LaunchDaemon (persists)   : %s\n", persistent ? "YES — disabled on boot" : "NO");

        if (loaded && persistent) {
            printf("Active now but will be blocked after reboot.\n");

        } else if (!loaded && persistent) {
            printf("Blocked now and will remain blocked after reboot.\n");

        } else if (loaded && !persistent) {
            printf("Active and no persistent block configured.\n");

        } else {
            printf("Not loaded and no persistent block.\n");

        }
    }


    // @name: printUsage
    // @details:
        /*
            Prints usage instructions for the macOS binary.
        */
    // @param: None
    // @return: None
    // @note: None
    //! @warning: None
    // @complexity:
        /*
            Time: O(1) - Space: O(1)
        */
    void printUsage(void) {
        printf("Usage: USBPortState [enable|disable|status]\n");
        printf("enable  - Enable USB storage\n");
        printf("disable - Disable USB storage\n");
        printf("status  - Show current state\n");
    }


    // @name: execute
    // @details:
        /*
            Parses command-line arguments and dispatches to the appropriate
            macOS USB management function.
        */
    // @param:
        /*
            argc - Number of command-line arguments.
            argv - Array of argument strings.
        */
    // @return: int - EXIT_SUCCESS on success, EXIT_FAILURE on bad arguments.
    // @note: Requires root privileges for enable/disable operations.
    //! @warning: None
    // @complexity:
        /*
            Time: O(1) - Space: O(1)
        */
    int execute(int argc, char* argv[]) {
        if (argc != 2) {
            printUsage();
            return EXIT_FAILURE;
        }

        if (strcmp(argv[1], "enable") == 0) {
            setUSBPortStateMacOS(1);

        } else if (strcmp(argv[1], "disable") == 0) {
            setUSBPortStateMacOS(0);

        } else if (strcmp(argv[1], "status") == 0) {
            checkUSBPortStatusMacOS();

        } else {
            printUsage();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

#else
    #error "Unsupported platform."
#endif


// main function
int main(int argc, char* argv[]){
    return execute(argc, argv);
}