# UBSSTOR-Services-Manager

Toggle USB storage access on Windows by changing the USBSTOR service setting in the Registry.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0-green.svg)](https://github.com/TulKasTer/UBSSTOR-Services-Manager/releases)
[![Latest Release](https://img.shields.io/github/v/release/TulKasTer/UBSSTOR-Services-Manager)](https://github.com/TulKasTer/UBSSTOR-Services-Manager/releases/latest)
[![Build Status](https://img.shields.io/github/actions/workflow/status/TulKasTer/UBSSTOR-Services-Manager/ci.yml?branch=main)](https://github.com/TulKasTer/UBSSTOR-Services-Manager/actions)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

![Project Banner or Screenshot](https://via.placeholder.com/1200x400?text=USBSTOR+Service+Manager)

---

## Table of Contents

- [About](#about)
- [Features](#features)
- [Demo](#demo)
- [Getting Started](#getting-started)
	- [Prerequisites](#prerequisites)
	- [Installation](#installation)
    - [Release](#release)
- [Usage](#usage)
- [Configuration](#configuration)
- [Project Structure](#project-structure)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [Acknowledgements](#acknowledgements)

---

## About

A small Windows utility to enable, disable or check the status of USB storage devices by updating the `USBSTOR` service value in the Registry. It's meant for administrators who need a quick way to enforce USB storage policies on individual machines without installing extra software.

Built to be simple and scriptable — no GUI, just a tiny command-line tool that you can run locally or from automation.

---

## Features

- Lightweight — single small executable, no dependencies beyond standard Windows APIs
- Toggle USB storage quickly via Registry (`USBSTOR` Start value)
- Check status — query current setting without making changes
- Script-friendly — suitable for automation or inclusion in larger provisioning scripts
- Includes safety notes and Registry backup recommendations

---

## Demo

There is no hosted demo for this CLI tool. Example usage is shown in the Usage section below.

---

## Getting Started

### Prerequisites

- Windows 10 / 11
- C compiler targeting Windows (MinGW/gcc or MSVC)
- Administrator (elevated) shell to run the resulting executable

### Installation

Clone the repository and build from source:

```powershell
git clone https://github.com/TulKasTer/UBSSTOR-Services-Manager.git
cd UBSSTOR-Services-Manager
# If you have make (WSL or GNU make for Windows)
make

# Or compile directly with gcc (PowerShell):
gcc -o USBSTORServiceManager.exe USBSTORServiceManager.c -ladvapi32
```

The build produces `USBSTORServiceManager.exe` (or `USBPortState.exe` in some examples). Use whichever filename your build produces.

### Release

Pre-built artifacts (Windows executable) are published on the project's GitHub Releases page. To download the latest release, visit:

https://github.com/TulKasTer/UBSSTOR-Services-Manager/releases/latest

Your CI workflow uploads the produced binaries to the Releases section — download the Windows executable (named like `USBSTORServiceManager-windows-amd64.exe`) from the release assets and run it from an elevated prompt.

If you prefer to build locally, follow the Installation instructions above.

---

## Usage

Run the executable from an elevated command prompt.

```powershell
.\USBSTORServiceManager.exe enable    # Enable USB storage
.\USBSTORServiceManager.exe disable   # Disable USB storage
.\USBSTORServiceManager.exe status    # Show current status
```

Example:

```powershell
# Disable USB storage now
.\USBSTORServiceManager.exe disable

# Check status
.\USBSTORServiceManager.exe status
```

---

## Configuration

This tool has no runtime configuration file. It operates by setting the `Start` DWORD under `HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR`:

| Value name | Expected | Meaning |
|------------|----------|---------|
| `Start`    | `3`      | USB storage enabled (default)
| `Start`    | `4`      | USB storage disabled |

---

## Project Structure

```
UBSSTOR-Services-Manager/
├── USBSTORServiceManager.c   # Main source
├── Makefile                  # Build helper
├── README.md
├── LICENSE
└── Jenkinsfile               # Optional CI pipeline
```

---

## Roadmap

- [x] Basic enable/disable/status commands

See open issues for more ideas.

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

Maintainer: TulKasTer — open an issue or PR on GitHub: https://github.com/TulKasTer/UBSSTOR-Services-Manager

---

## Acknowledgements

- Windows Registry APIs and Microsoft Docs
- Examples and snippets from community tutorials on managing drivers and services

