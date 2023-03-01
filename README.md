# Blood Pressure Monitor V2

## Clone project

- **User name**: nguyen_trung
- **Password**: ys2kphguaz2rnbi5x6lh3fpuv7qcv6tuelfz4kyfoy4d6br3ycvq

## Support feature

Visual Studio Code:

- Auto format new codes when saving files
- Debug breakpoints style with Cortex-Debug extension
- Ignore **generated** folder when searching and indexing

Source code:

- Build with Makefile
- Debug with led blink (if led is blinked, code is running without crash, otherwise)
- Debug with trace log through UART
- Detect **Hardfault Exception** through **trace log**
- Detect Lack of Heap memory through **trace log**
- Detect Stack overflow of freeRTOS tasks through **trace log**
- Generate STM32 peripheral configuration with no conflict
- Build smoke test (unit test) with minimum effort
- Reserve APIs prototypes for all components

## Setup environment

- Install these softwares in this [link download](https://hclo365.sharepoint.com/:f:/r/sites/BloodPressureMonitorV2/Shared%20Documents/General/Setup?csf=1&web=1&e=xgt53j)
  - **Visual Studio Code**
  - **Git**
  - **GNU Make**
  - **STM32CubeIDE**
  - **STM32CubeProgrammer**
  - **STM32CubeMX**
  - **GNU Arm Embedded Toolchain**
  - **Openocd**
  - **Cortex-Debug v0.4.7 extension** of **Visual Studio Code**
  - **C/C++ extension** of **Visual Studio Code**

- Add environment path for:

  - **GNU make**
  - **GNU Arm Embedded Toolchain**
  - **Openocd**

- Verify that softwares is installed successfully:

    ```bash
    git --version
    make --version
    openocd --version
    arm-none-eabi-gcc --version
    ```

## Build command

```bash
# Build project
make -j12

# Build and flash 
make flash -j12

## Rebuild
make clean
make -j12

## Build unit test
make clean
make -j12 test=-DTEST_BUTTON
make -j12 test=-DTEST_GLCD
```
