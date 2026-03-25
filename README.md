# USBSTOR-Service-Manager

Cross-platform CLI utility to enable, disable, or check the status of USB storage devices.

- **Windows** — modifies the `USBSTOR` service value in the Registry
- **Linux** — manages the `usb_storage` kernel module via blacklist and initramfs
- **macOS** — controls `IOUSBMassStorageDriver` via kextunload and a LaunchDaemon 

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-1.1.0-green.svg)](https://github.com/TulKasTer/USBSTOR-Service-Manager/releases)
[![Latest Release](https://img.shields.io/github/v/release/TulKasTer/USBSTOR-Service-Manager)](https://github.com/TulKasTer/USBSTOR-Service-Manager/releases/latest)
[![Build Status](https://img.shields.io/github/actions/workflow/status/TulKasTer/USBSTOR-Service-Manager/release.yml?branch=main)](https://github.com/TulKasTer/USBSTOR-Service-Manager/actions)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

---

## Table of Contents

- [About](#about)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Release](#release)
- [Usage](#usage)
- [How it works](#how-it-works)
- [Project Structure](#project-structure)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## About

A small cross-platform utility to enforce USB storage policies on individual machines without installing extra software. It runs from the command line, requires no GUI, and is suitable for automation or inclusion in provisioning scripts.

Originally Windows-only, it now targets all three major desktop platforms from a single C source file using preprocessor conditionals (`#ifdef _WIN32`, `#elif __linux__`, `#elif __APPLE__`).

---

## Features

- Single executable per platform — no runtime dependencies beyond standard system APIs
- **Windows** — reads and writes the `USBSTOR\Start` registry DWORD; prompts for reboot
- **Linux** — writes or removes `/etc/modprobe.d/disable-usb-storage.conf`; auto-detects distro to rebuild initramfs; applies the change immediately via `modprobe`
- **macOS** — unloads `IOUSBMassStorageDriver` immediately; installs or removes a LaunchDaemon for persistence across reboots
- `status` command reports both the live state and the persistent block on all platforms
- Script-friendly — exits with `EXIT_SUCCESS` / `EXIT_FAILURE` for use in automation

---

## Getting Started

### Prerequisites

| Platform | Requirement |
|---|---|
| Windows | Administrator shell; MinGW/gcc or MSVC to build from source |
| Linux | Root shell; `gcc`, `make`; distro package manager for initramfs tools |
| macOS | Root shell; Xcode Command Line Tools (`xcode-select --install`) |

### Installation

Clone and build from source:

```bash
git clone https://github.com/TulKasTer/USBSTOR-Service-Manager.git
cd USBSTOR-Service-Manager
```

**Windows** (PowerShell or WSL):
```powershell
# With make (WSL or GNU make for Windows)
make build-windows

# Or directly with gcc
gcc -o USBSTORServiceManager.exe USBSTORServiceManager.c -ladvapi32
```

**Linux**:
```bash
make build-linux
# or
gcc -o USBSTORServiceManager-linux-amd64 USBSTORServiceManager.c
```

**macOS**:
```bash
make build-macos
# or
clang -o USBSTORServiceManager-macos-amd64 USBSTORServiceManager.c
```

### Release

Pre-built binaries for all three platforms are published on the GitHub Releases page:

https://github.com/TulKasTer/USBSTOR-Service-Manager/releases/latest

| Platform | File |
|---|---|
| Windows x64 | `USBSTORServiceManager-windows-amd64.exe` |
| Linux x64 | `USBSTORServiceManager-linux-amd64` |
| macOS x64 | `USBSTORServiceManager-macos-amd64` |

Download the binary for your platform and run it from an elevated shell.

---

## Usage

All platforms share the same three commands. Run from an elevated shell (Administrator on Windows, `sudo` on Linux/macOS).

```bash
# Enable USB storage
./USBSTORServiceManager enable

# Disable USB storage
./USBSTORServiceManager disable

# Show current state (live + persistent)
./USBSTORServiceManager status
```

**Windows** (elevated PowerShell):
```powershell
.\USBSTORServiceManager-windows-amd64.exe disable
.\USBSTORServiceManager-windows-amd64.exe status
```

**Linux**:
```bash
sudo ./USBSTORServiceManager-linux-amd64 disable
sudo ./USBSTORServiceManager-linux-amd64 status
```

**macOS**:
```bash
sudo ./USBSTORServiceManager-macos-amd64 disable
sudo ./USBSTORServiceManager-macos-amd64 status
```

### Example `status` output (Linux)

```
Kernel module loaded      : YES
Blacklist file (persists) : YES — disabled on boot
[!] Active now but will be blocked after reboot.
```

---

## How it works

Each platform uses a different native mechanism:

### Windows

Reads and writes the `Start` DWORD under `HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR`:

| Value | Meaning |
|---|---|
| `3` | USB storage enabled (demand start) |
| `4` | USB storage disabled |

Changes take effect after reboot. The tool prompts for confirmation before rebooting.

### Linux

Creates or removes `/etc/modprobe.d/disable-usb-storage.conf` containing `blacklist usb_storage`.

After writing or removing the file, it:
1. Applies the change immediately with `modprobe -r usb_storage` or `modprobe usb_storage`
2. Rebuilds the initramfs using the correct command for the detected distro:

| Distro family | Command |
|---|---|
| Debian / Ubuntu | `update-initramfs -u` |
| RHEL / Fedora / CentOS | `dracut --force` |
| Arch | `mkinitcpio -P` |

### macOS

Unloads or loads `com.apple.iokit.IOUSBMassStorageDriver` via `kextunload` / `kextload` for the current session.

For persistence across reboots, it installs or removes a LaunchDaemon at `/Library/LaunchDaemons/com.usbstor.disable.plist` that re-runs the unload on every boot.

> **Note:** macOS 11 Big Sur and later seal the system volume, so modifying kernel extensions directly is not possible. The LaunchDaemon approach works within those constraints.

---

## Project Structure

```
USBSTOR-Service-Manager/
├── .github/
│   └── workflows/
│       └── release.yml           # CI/CD — builds for Windows, Linux, macOS
├── USBSTORServiceManager.c       # Single cross-platform source file
├── Makefile                      # Build targets per platform
├── README.md
└── LICENSE
```

---

## Roadmap

- [x] Windows — Registry-based enable/disable/status
- [x] Linux — blacklist + initramfs + live modprobe
- [x] macOS — kextunload + LaunchDaemon persistence
- [x] Cross-platform CI/CD with GitHub Actions
- [ ] Logging to a file for audit trail
- [ ] Dry-run mode (`--dry-run`) that prints actions without executing them

---

## Contributing

Contributions are welcome. Quick steps:

1. Fork the repo
2. Create a branch: `git checkout -b feature/my-change`
3. Commit and push
4. Open a PR describing the change

If you're unsure about a change, open an issue first.

---

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for details.

---

## Contact

Maintainer: TulKasTer — open an issue or PR on GitHub:
https://github.com/TulKasTer/USBSTOR-Service-Manager