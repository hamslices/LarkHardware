# Lark - Thermal Print Engine Controller

**Version:** 01.00.01  
**Author:** HamSlices  
**License:** CERN Open Hardware Licence v2 - Strongly Reciprocal  

This repository contains the complete hardware design files for the Lark, a controller board specifically designed for the **FUJITSU FTP-68EMCL112-R** 8-inch thermal print mechanism.

The goal of this project is to provide a fully open-source hardware solution for driving this excellent thermal printer, suitable for custom projects, prototyping, or integration into larger systems.

## Repository Contents

This repository includes all the necessary resources to build and case the hardware:

*   **Lark Controller Board**: The core of the project, designed to drive the Fujitsu thermal printer.
*   **Front I/O Board**: A separate PCB for user-facing controls and connectors.
*   **STLink-V2 Adapter**: A small adapter PCB to easily connect an STLink-V2 for programming and debugging.
*   **3D Printable Enclosures**: Includes custom cases for the Lark board, the Front I/O board, and a test enclosure for the print mechanism itself.

## Directory Structure

Here is a breakdown of the repository's structure:

*   **`/3D`**: Contains 3D models of the various hardware components and enclosures.
*   **`/CubeMX_Lark`**: The STM32CubeMX project file (`.ioc`) for configuring the Lark's microcontroller.
*   **`/Demo Case`**: Files for the 3D printable enclosures for the Lark board, Front I/O, and the print mechanism.
*   **`/img`**: Images, screenshots, and logos related to the project.
*   **`/Kicad Project Files`**: The source files for all hardware designed in KiCad.
    *   **`/FrontIO`**: KiCad project for the Front I/O board.
    *   **`/Lark`**: KiCad project for the main Lark controller board.
    *   **`/STLink_adapter`**: KiCad project for the STLink-V2 adapter.
*   **`/lib`**: Custom KiCad schematic symbols and PCB footprints created for this project.
*   **`/Production_Files`**: Ready-to-manufacture files (Gerbers and BOMs) for all PCBs.

## Getting Started

To work with the hardware design files, you will need to install the latest version of [KiCad](https://www.kicad.org/).

The 3D models are provided in STEP format and can be viewed with any compatible software.

The production files located in the `/Production_Files` directory are ready to be sent to a PCB manufacturer for fabrication and assembly.

## Designed By

The Lark Thermal Print Engine Controller and all associated hardware in this repository were designed by hamslices.

## Software

The firmware for the Lark controller is licensed under the **GPL v3** and is maintained in a separate repository.

## License

The hardware design files in this repository (schematics, PCB layouts, 3D models, etc.) are licensed under the **CERN Open Hardware Licence v2 - Strongly Reciprocal (CERN-OHL-S)**.

A full copy of the license is available in the `LICENSE` file in this repository. In essence, this license allows you to use, study, modify, and share the hardware designs, provided that you share any modifications or derivative works under the same license.

---
*Lark Print Engine © HamSlices 2025*
