/* -------------------------------------------------------------------- */
/* Manual for the sample program                                        */
/* -------------------------------------------------------------------- */
The LIN2.1 slave driver for RL78/F2x.
                                                    Version 0.10


/* -------------------------------------------------------------------- */
/* History                                                              */
/* -------------------------------------------------------------------- */
2013/03/29 A.Kondo      Ver 0.01 1st creation.
2013/04/08 A.Kondo      Ver 0.02 A few modifications.
2013/08/22 A.Kondo      Ver 0.03 Deviations of RL78/F14 UM(Rev1.00) were reflected.
2015/02/04 S.Sugawara   Ver 0.04 Deviations of RL78/F14 UM(Rev2.00) were reflected.
2015/06/10 S.Sugawara   Ver 0.05 Correction of errors.
2016/02/02 A.Kondo      Ver 0.06 The restriction for fMX was added.
2016/06/14 S.Sugawara   Ver 0.07 CS+ CC-RL support.
2021/08/31 K.Ogata      Ver 0.08 RL78/F23,F24 support,LIN2.1 support(RL78/F23,F24 only),IAR compiler support,
2024/03/22 M.hirano     Ver 0.09 RL78/F23,F24 support.
2025/08/06 A.Kondo      Ver 0.10 Modified for LIN2.1 Conformance Test.


/* -------------------------------------------------------------------- */
/* Outline                                                              */
/* -------------------------------------------------------------------- */
This document explains about LIN2.1 slave driver.


/* -------------------------------------------------------------------- */
/* Folder composition                                                   */
/* -------------------------------------------------------------------- */
Folder composition is shown below.

Root folder
   |
   +-conf                               : The folder for configuration
   |   |
   |   +-conflin.c                      : The configuration file for the LIN2.1 slave driver
   |   |
   |   +-conflin.h                      : The configuration header file for the LIN2.1 slave driver
   |
   +-liblin2                            : The folder including files for generating a LIN2.1 library
   |   |
   |   +-liblin21s_CCRL.mtpj            : Project file for CS+ CC-RL
   |   |
   |   +-liblin21s_IAR.eww              : Project file for IAR
   |   |
   |   +-(Other folder)
   |
   +-libsrc                             : The folder for the driver program
   |   |
   |   +-conf                           : The folder for driver configuration
   |   |   |
   |   |   +-confslin_opt.h             : The driver configuration header file
   |   |   | 
   |   |   +-r_rlin3_config.h           : The driver configuration header file (Smart configurator output)
   |   |   |
   |   |   +-(Other header files)
   |   |
   |   +-dev                            : Device dependence folder
   |   |   |
   |   |   +-(Other header files)
   |   |
   |   +-(Other files)
   |
   +-readme.txt                         : This file


/* -------------------------------------------------------------------- */
/* Usage                                                                */
/* -------------------------------------------------------------------- */

[How to compile library]

  1. Modify as necessary the driver configuration [libsrc/conf/confslin_opt.h].
  2. Read The following project file at CS+ or IAR compiler.
        CS+ CC-RL   : liblin2/liblin21s_CCRL.mtpj
        IAR compiler: liblin2/liblin21s_IAR.eww
  3. Add compile options as needed.
        CS+ CC-RL   : Select the "Compile Options" tag and add compile options to the "Define Macros" if necessary.
        IAR compiler: From the "C/C++ Compiler Category" menu, add compile options to "Symbol Definition" if necessary.
      (See [A list of compile options], which will be described later)
  4. Generates an I/O header file.(for CS+ CC-RL)
     Select "Build Tool" in the project tree.
     Select "I/O Header File Generation Options" tag,
     Set "Update I/O header file on build" of "I/O Header File" as "Yes(Checking the device file)".
     When the device file has been updated at build, the I/O header file("iodefine.h") is updated.
  5. Compile and link.
     (for CS+ CC-RL)
     Select [Build]->[Build Project].
     (IAR compiler)
     Select [Project]->[Rebuild All].
  7. If you want to link with application program, copy library file in the application folder.
     The library file name is as follows.
       CS+ CC-RL   : liblin21s_CCRL.lib
       IAR compiler: liblin21s_IAR.a

[A list of compile options]

  1. Series of a micro computer
    __LIN_RL78_F23_F24__    : RL78/F23,F24 series

  2. Channel
    __LIN_CH0_P1__          : RLIN3 use of channel 0(Transmission:P13 , Reception:P14)
    __LIN_CH0_P4__          : RLIN3 use of channel 0(Transmission:P42 , Reception:P43)
    __LIN_CH1_P1__          : RLIN3 use of channel 1(Transmission:P10 , Reception:P11)
    __LIN_CH1_P10__         : RLIN3 use of channel 1(Transmission:P106, Reception:P107)
    __LIN_CH1_P12__         : RLIN3 use of channel 1(Transmission:P120, Reception:P125)

  3. Use hardware auto baudrate detection
    __LIN_HW_AUTO_BR__      : Use auto automatic baud rate

  4. Assembler version of the memory copy routine
    __LIN_MEMCOPY_ASM__

  5. There are 32 extensions defined in the frame signal
    __LIN_SIGNAL_32__

 - 1,2 Select Always.
   Select it even when compiled application.
 - Select 3,4,5 necessary.
   (If you do not select a 3, Describe the baud rate to be used to conflin.c.)
   (4 Small, medium model is not available outside.)
   (5 must match the selection in the presence or absence of both the library and the application.)
 - If you are using a memory copy routine assembler version, you need to pass the 2 and 4 to the assembler.


/* -------------------------------------------------------------------- */
/* Limitations                                                          */
/* -------------------------------------------------------------------- */
 - If driver uses fMX as LIN communication clock, don't use timeout error detection
   and set frequency [fCLK >= fMX x 1.2].
   Compile error will occur when fMX and timeout error detection are selected at
   same time.


/* -------------------------------------------------------------------- */
/* Supplement                                                           */
/* -------------------------------------------------------------------- */
 - After setting the contents of the register LCHSEL channel selection RLIN3, with this driver, 
   but it does not return to its previous value setting.
   Therefore, if you use a different channel in your software, register access is done in the interrupt disabled,
   Set the value to LCHSEL before access.


/* -------------------------------------------------------------------- */
/* Notice                                                               */
/* -------------------------------------------------------------------- */
 - This software is a software for reference purposes only, it is not guaranteed by the Company this behavior.
   If you use this software, 
   thank you to your application so that the product is used on a thorough evaluation on a set of customers.


/* End of files */
