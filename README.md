# Blood Pressure Monitor V2

## Build command

```bash
# Build project
make -j12

# Build and flash 
make flash -j12
```

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

- Add environment path for:

  - **GNU make**: 
  - **GNU Arm Embedded Toolchain**
  - **Openocd**

- Verify that softwares is installed successfully:

    ```bash
    git --version
    make --version
    openocd --version
    arm-none-eabi-gcc --version
    ```

