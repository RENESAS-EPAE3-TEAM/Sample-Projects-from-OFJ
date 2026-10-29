# ra8t2_cpk_qspi_fsp590rc_0804 例程说明

## 1. 工程概述

本工程是基于 Renesas RA8T2 的 OSPI_B / QSPI 外部 Flash 示例。工程通过 FSP OSPI_B 驱动配置 QSPI 通信，访问外部 128 Mbit SPI/QSPI NOR Flash，并完成读写校验。

本工程适用于 RA8T2 通过 OSPI0 / CS0 连接 W25Q128 等 QSPI Flash 的开发与调试，可作为外部 Flash 驱动适配的参考。

本工程主要演示以下功能：

- OSPI_B 初始化，以及外部 Flash 的复位和器件 ID 读取。
- 外部 Flash 的 Sector 擦除、256 字节测试数据写入和读回校验。
- 运行时从 1S-1S-1S（SPI）协议切换到 1S-4S-4S（QSPI）协议，并在四线模式下再次校验数据。

## 2. 软件运行环境

| 软件/组件 | 版本或要求 |
| --- | --- |
| 运行环境 | Renesas e2 studio |
| FSP | 5.9.0-rc.0 |
| 编译器 | LLVM for ARM / clang_arm 18.1.3 |
| 调试器 | J-Link / Renesas E2 / E2 Lite 调试器 |

## 3. 支持的开发板 / MCU

- MCU：Renesas RA8T2。
- 开发板：RA8T2 CPK/MCK，或基于 RA8T2 的用户自定义板。
- 外部 Flash：W25Q128JV/W25Q128JW、AT25SF128A 等 128 Mbit QSPI NOR Flash 可参考本工程适配。

## 4. 硬件要求

- RA8T2 目标板。
- IS25LP128、W25Q128 或兼容的 QSPI Flash，连接到 OSPI0 / CS0。
- 调试器及 SWD/JTAG 连接线。
- USB 线缆及必要跳线。
- 逻辑分析仪或示波器（可选），用于观察 CS#、SCLK、IO0～IO3 波形。

## 5. 硬件连接

如果目标板已焊接 IS25LP128 等外部 Flash，通常无需额外飞线，只需确认 Flash 片选接在 CS0，并且四线数据线连接正确。如果使用自定义板，请按下表连接。

| QSPI 信号 | RA8T2 引脚配置 |
| --- | --- |
| CS# | P107 / OSPI0_OM_0_CS0 |
| SCLK | P808 / OSPI0_OM_0_SCLK |
| IO0 | P100 / OSPI0_OM_0_SIO0 |
| IO1 | P803 / OSPI0_OM_0_SIO1 |
| IO2 | P103 / OSPI0_OM_0_SIO2 |
| IO3 | P101 / OSPI0_OM_0_SIO3 |

建议将 OSPI 相关引脚的驱动能力设置为 **HH**，以降低高速通信出错的概率。

抓取波形时，建议至少连接 CS#、SCLK、IO0、IO1 和 GND。如需确认 1S-4S-4S 访问，需同时连接 IO2 和 IO3。

## 6. 配置原理与说明

RA8T2 FSP 使用 OSPI_B 驱动访问外部 Flash。RA8T2 的 OSPI 接口兼容 xSPI 标准，QSPI 可以理解为其中的 1S-4S-4S 模式。

程序的基本流程如下：

1. 以 1S-1S-1S 协议完成 Flash 复位和 ID 读取。
2. 执行擦除、写入及读回校验。
3. 切换至 1S-4S-4S 协议后，再次通过内存映射地址读取并校验数据。

外部 Flash 映射到 `0x80000000` 起始的地址空间，程序可以通过内存地址直接读取 Flash 数据。

### 6.1 OSPI_B 配置

| 项目 | 配置 |
| --- | --- |
| FSP 驱动 | OSPI Flash (`r_ospi_b`) |
| 驱动实例 | `g_ospi0` |
| Unit / Channel | OSPI_B0 / CS0 |
| 初始协议 | SPI（1S-1S-1S） |
| High-speed Mode | QSPI（1S-4S-4S） |
| 地址宽度 | 3 字节 |
| Sector 大小 | `0x1000` 字节 |
| 4S Memory Read Dummy Cycles | 6 |
| Memory-mapping Prefetch | Enable |
| Memory-mapping Combination | 64 Byte |
| 外部 Flash 映射地址 | `0x80000000` |
| 测试地址 | `0x80000000`、`0x80040000` |

### 6.2 主要命令

| 功能 | 命令 | 说明 |
| --- | --- | --- |
| Enable Reset | `0x66` | 复位前置命令 |
| Reset Device | `0x99` | 复位外部 Flash |
| Read ID（1S 模式） | `0x90` | 读取 Manufacturer ID / Device ID |
| Read ID（4S 模式） | `0x94` | 四线模式下读取 ID |
| Read JEDEC ID | `0x9F` | IS25LP128 返回 `0x9D6018` |
| Write Enable | `0x06` | 擦除/写入前执行 |
| Sector Erase | `0x20` | 擦除 `0x1000` 字节 Sector |
| Page Program | `0x02` | 页编程 |
| Quad I/O Read | `0xEB` | 1S-4S-4S 模式下的读命令 |
| QE 位设置（AT25SF128A） | `0x50` / `0x31` | 仅在 AT25SF128A 分支中使用 |

程序通过 Read ID 命令识别 AT25SF128A、W25Q128JVPIQ、W25Q128JW 等外部 Flash。若 ID 无法识别，或读写校验失败，程序将进入 `fsp_assert()`。

### 6.3 初始化与测试流程

1. 调用 `R_OSPI_B_Open()`，打开 OSPI_B 驱动。
2. 发送 `0x66` 和 `0x99`，复位外部 Flash。
3. 默认发送 `0x90`，读取并判断外部 Flash ID。`0x9F` 仅用于当前未启用的 `QSPI_S28HS512T` 条件分支。
4. 执行 Write Enable 和 Sector Erase，擦除 `0x80000000` 所在的 Sector（`0x1000` 字节）。
5. 将 `test_data[256]` 按 0～255 填充，以每次 64 字节的方式写入 Flash。
6. 在 1S-1S-1S 模式下，读取 `0x80000000` 和 `0x80040000` 地址处的数据，与 `test_data` 比较。

   > **注意：** 程序还会写入 `0x80040000`，但写入前未擦除该地址所在的 Sector，因此运行前应确保该区域已处于擦除状态。

7. 根据器件型号设置 QE 位。当前代码仅在识别为 AT25SF128A 时使用 `0x50` / `0x31` 设置 QE 位。
8. 调用 `R_OSPI_B_SpiProtocolSet()` 切换至 1S-4S-4S 协议，再次通过内存映射读取并校验数据。
9. 全部校验通过后，程序执行到代码末尾的 `__BKPT(0)`。

## 7. 运行方法

### 7.1 导入、编译与下载工程

1. 解压 `ra8t2_cpk_qspi_fsp590rc0_0804.zip`。
2. 在 e2 studio 中导入 `ra8t2_cpk_qspi_fsp590rc_0804` 工程。
3. 打开 `configuration.xml`，检查 OSPI_B0 / CS0 配置、引脚分配，以及引脚驱动能力是否为 HH。
4. 连接目标板和调试器，编译工程并下载到目标板。

> **注意：** 工程包含外部 Flash 擦除和写入操作，运行前请确认测试区域没有需要保留的数据。

### 7.2 调试过程与结果分析

全速运行程序，并根据停止位置判断结果：

- **停在 `hal_entry()` 末尾标注为“happy path”的 `__BKPT(0)`：**  
  表示两个地址在 1S-1S-1S 和 1S-4S-4S 模式下均校验通过。

- **停在 1S 模式 ID 判断后的 `__BKPT(0)`：**  
  表示 `0x90` 返回的器件 ID 不在当前代码支持列表中。

- **停在 4S 模式 ID 判断后的 `__BKPT(0)`：**  
  表示切换协议后，`0x94` 返回的 ID 不符合当前代码判断。

- **停在 `fsp_assert()`：**  
  表示 FSP API 返回错误、等待 WIP 超时或数据比较失败。