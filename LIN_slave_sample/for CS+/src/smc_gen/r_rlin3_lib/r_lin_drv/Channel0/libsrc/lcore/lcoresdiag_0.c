/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2025 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoresdiag.c                                              */
/* Version:   0.05                                                      */
/* Date:      2025.08.04                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2015.02.02  Corresponded to RISRA-C                         */
/*   V0.03: 2021.05.31  LIN2.1, RL78/F2x support                        */
/*   V0.04: 2024.08.30  Modified for RL78/F23,F24.                      */
/*   V0.05: 2025.08.04  Applied Wildcard for ReadByID's SupID and FunID */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* ---------------------------------------------------------------------*/
/* The addition of LIN channel number in function name and variable name*/
/* ---------------------------------------------------------------------*/
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)        /* Use LIN channel 0 */
#define LCoreSDiag_vosAnalysisPdu   LCoreSDiag_vosAnalysisPdu_0
#define LCoreSDiag_vosSetReadById   LCoreSDiag_vosSetReadById_0
#define LCoreSDiag_u1sAssignIdRange LCoreSDiag_u1sAssignIdRange_0
#define LCoreSDiag_u1sDataBuf       LCoreSDiag_u1sDataBuf_0
#define LCoreSDiag_u1sSignalFlag    LCoreSDiag_u1sSignalFlag_0
#define LCoreSDiag_u1sNad           LCoreSDiag_u1sNad_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)       /* Use LIN channel 1 */
#define LCoreSDiag_vosAnalysisPdu   LCoreSDiag_vosAnalysisPdu_1
#define LCoreSDiag_vosSetReadById   LCoreSDiag_vosSetReadById_1
#define LCoreSDiag_u1sAssignIdRange LCoreSDiag_u1sAssignIdRange_1
#define LCoreSDiag_u1sDataBuf       LCoreSDiag_u1sDataBuf_1
#define LCoreSDiag_u1sSignalFlag    LCoreSDiag_u1sSignalFlag_1
#define LCoreSDiag_u1sNad           LCoreSDiag_u1sNad_1
#endif /* __LIN_CH1__ */


/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "../drv/drvsuart.h"
#include "./lcoresdiag.h"
#include "./lcoresresp.h"   /* Just to use response direction */
#include "./lcoressleep.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#define LCORESDIAG_u1sNONUPDATED            (0x00U)
#define LCORESDIAG_u1sUPDATED               (0x01U)

#define LCORESDIAG_u1sASSIGNFRAMEID         (0xB1U)     /* SID definition */
#define LCORESDIAG_u1sREADBYIDREQ           (0xB2U)
#define LCORESDIAG_u1sASSIGNFRAMEIDRANGE    (0xB7U)
#define LCORESDIAG_u1sNADWILDCARD           (0x7FU)
#define LCORESDIAG_u2sSUPIDWILDCARD         (0x7FFFU)   /* Wildcard for SupplierID */
#define LCORESDIAG_u2sFUNIDWILDCARD         (0xFFFFU)   /* Wildcard for FunctionID */

#define LCORESDIAG_u1sMASTERREQUESTID       (0x3CU)     /* Master request ID(Including parity) */
#define LCORESDIAG_u1sSLAVERESPONSEID       (0x7DU)     /* Slave response ID(Including parity) */

/* Assign frame ID range */
#define LCORESDIAG_u1sASSIGNIDUNASSIGN      (0x00U)     /* unassign value    */
#define LCORESDIAG_u1sASSIGNIDDONOTCARE     (0xFFU)     /* do not care value */

/* -------------------------------------------------------------------- */
/* Function prototype declaration                                       */
/* -------------------------------------------------------------------- */
static void LCoreSDiag_vosAnalysisPdu( void );              /* Analysis process PDU          */
static void LCoreSDiag_vosSetReadById( u1 );                /* Reading process by Identifier */
static u1   LCoreSDiag_u1sAssignIdRange( u1, const u1* );   /* Allocation process PID        */

/* -------------------------------------------------------------------- */
/* Constant declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Variable declaration                                                 */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes = @ "LSDATA"
#endif
static u1 LCoreSDiag_u1sDataBuf[8];                 /* Buffer for sending diagnostic response */
static u1 LCoreSDiag_u1sSignalFlag;                 /* Diagnosis update flag                  */
static u1 LCoreSDiag_u1sNad;                        /* NAD                                    */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes =
#endif

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes = @ "LSCODE"
#endif
/* -------------------------------------------------------------------- */
/* Description : Initialize this diagnostic driver                      */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
void
LCoreSDiag_vogInitAll( void )
{
    u1 i;

    LCoreSDiag_u1sSignalFlag = (u1)LCORESDIAG_u1sNONUPDATED;

    for(i = (u1)0; i < (u1)8; i++)
    {
        LCoreSDiag_u1sDataBuf[i] = (u1)0x00U;
    }

    LCoreSDiag_u1sNad = ConfSLin_stgNODE.u1InitNad;

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start sending and receiving response.                  */
/* Parameters  : u1tPid = PID                                           */
/* Return value: LCORESRESP_u1gIGNORE  = Not need to do diag action     */
/*               LCORESRESP_u1gRECEIVE = Start reception process        */
/*               LCORESRESP_u1gSEND    = Start transmission process     */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSDiag_u1gStartResponse( u1 u1tPid )
{
    u1 u1tRespDir;
     
    
    u1tRespDir = (u1)LCORESRESP_u1gIGNORE;
    
    if( u1tPid == (u1)LCORESDIAG_u1sMASTERREQUESTID ) {
        
        /* If the request is from master, receive unconditionally */
        LDrvSUart_vogRecvResponse( 8, (u1)CONFSLIN_u1gCLASSICSUM );
        u1tRespDir = (u1)LCORESRESP_u1gRECEIVE;
    }
    else if( u1tPid == (u1)LCORESDIAG_u1sSLAVERESPONSEID ) {
        
        /* If the request is from slave, send only if the diagnostic information has been updated */
        if( LCoreSDiag_u1sSignalFlag == (u1)LCORESDIAG_u1sUPDATED ) {
            
            LDrvSUart_vogSetBuf( &LCoreSDiag_u1sDataBuf[0], 8 );
            LDrvSUart_vogSendResponse( 8, (u1)CONFSLIN_u1gCLASSICSUM );
            u1tRespDir = (u1)LCORESRESP_u1gSEND;
        }
    }
    else {
        
        /* No processing */
    }


    return u1tRespDir;
}


/* -------------------------------------------------------------------- */
/* Description : Process of when response is successfully received      */
/* Parameters  : u1tPid = PID                                           */
/* Return value: TRUE  = There is process as a diagnostic module        */
/*               FALSE = There is no process as a diagnostic module     */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSDiag_u1gFinishRecResp( u1 u1tPid )
{
    u1 u1tResult;
    

    if( u1tPid == (u1)LCORESDIAG_u1sMASTERREQUESTID ) {
        
        /* Copy 8 bytes from the data buffer of RLIN3 */
        /* (In order to reduce processing time, copy only if PID matches) */
        LDrvSUart_vogGetBuf( &LCoreSDiag_u1sDataBuf[0], (u1)8 );
        if( LCoreSDiag_u1sDataBuf[0] == (u1)0x00U ) {
            
            /* Master request frame(Request SLEEP) */
            LCoreSSleep_vogSleep();
        }
        else {
            
            /* Master request frame(Request except SLEEP) */
            LCoreSDiag_vosAnalysisPdu();    /* Diag processing */
        }
        
        /* Returns TRUE if there is process as a diagnostic module */
        u1tResult = (u1)TRUE;
    }
    else {

        /* Returns FALSE if there is no process as a diagnostic module */
        u1tResult = (u1)FALSE;
    }
    
    
    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Description : Process of when response is response successfully sent */
/* Parameters  : u1tPid = PID                                           */
/* Return value: TRUE  = There is process as a diagnostic module        */
/*               FALSE = There is no process as a diagnostic module     */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSDiag_u1gFinishSendResp( u1 u1tPid )
{
    u1 u1tResult;
    
    
    if( u1tPid == (u1)LCORESDIAG_u1sSLAVERESPONSEID ) {
        
        /* Clear the diagnosis update flag because slave response transmission is completed, */
        LCoreSDiag_u1sSignalFlag = (u1)LCORESDIAG_u1sNONUPDATED;
        
        u1tResult = (u1)TRUE;
    }
    else {
        u1tResult = (u1)FALSE;
    }
    
    
    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : PDU frame analysis, the diagnostic process             */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : The receive data must be stored in                     */
/*               LCoreSDiag_u1sDataBuf in advance. If you want to       */
/*               enable this function for other diagnostic frame,       */
/*               please fix this function.                              */
/* -------------------------------------------------------------------- */
static void
LCoreSDiag_vosAnalysisPdu( void )
{
    u1 u1tRet;
    u2 u2tRxSupId;
    u2 u2tRxFunId;

    /* Check whether message is compatible with PDU supported by this driver */
    if( (LCoreSDiag_u1sDataBuf[0] == LCoreSDiag_u1sNad) ||      /* Byte0 : NAD or Wild card              */
        (LCoreSDiag_u1sDataBuf[0] == (u1)LCORESDIAG_u1sNADWILDCARD) ) {

        if ( LCoreSDiag_u1sDataBuf[1] == (u1)0x06U ) {          /* Byte1 : PCI is only compatible with 6 */

            if( LCoreSDiag_u1sDataBuf[2] == (u1)LCORESDIAG_u1sREADBYIDREQ ) {
                                                                /* Byte2 : SID                           */

                u2tRxSupId = ( (u2)LCoreSDiag_u1sDataBuf[4] | (u2)((u2)LCoreSDiag_u1sDataBuf[5] << 8) );
                u2tRxFunId = ( (u2)LCoreSDiag_u1sDataBuf[6] | (u2)((u2)LCoreSDiag_u1sDataBuf[7] << 8) );
                                                                /* Byte4-5 : SupplierID                  */
                                                                /* Byte6-7 : FunctionID                  */

                if ( ( (u2tRxSupId == ConfSLin_stgNODE.u2SupId) || (u2tRxSupId == LCORESDIAG_u2sSUPIDWILDCARD) )
                  && ( (u2tRxFunId == ConfSLin_stgNODE.u2FunId) || (u2tRxFunId == LCORESDIAG_u2sFUNIDWILDCARD) ) ) {

                    /************************/
                    /* Read By ID           */

                    LCoreSDiag_vosSetReadById( LCoreSDiag_u1sDataBuf[3] );  /* Read identifier(Byte3)      */
                    LCoreSDiag_u1sSignalFlag = (u1)LCORESDIAG_u1sUPDATED;   /* Update the diagnostic flags */
                }
            }
            else if( LCoreSDiag_u1sDataBuf[2] == (u1)LCORESDIAG_u1sASSIGNFRAMEIDRANGE )
            {
                /************************/
                /* Assign Frame ID Range*/

                u1tRet = LCoreSDiag_u1sAssignIdRange( LCoreSDiag_u1sDataBuf[3], (const u1*)&LCoreSDiag_u1sDataBuf[4] );
                                                                            /* Command assignment ID */

                if( u1tRet == (u1)OK ) {

                    LCoreSDiag_u1sSignalFlag = (u1)LCORESDIAG_u1sUPDATED;   /* Diagnostic flag update */

                }
                else {
                    /* Because there is no response for Assign frame identifier, */
                    /* process for the allocation failure is not necessary.      */
                    ;
                }
            }
            else
            {
                /* If there is no the case in the above conditions, exit doing nothing */
            }
        }
    }
    else {

        LCoreSDiag_u1sSignalFlag = (u1)LCORESDIAG_u1sNONUPDATED;
    }

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Read by identifier                                     */
/* Parameters  : u1tId = identifier                                     */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
static void
LCoreSDiag_vosSetReadById( u1 u1tId )
{
    if( u1tId == (u1)0x00U )                                              /* Read LIN product identification */
    {
        LCoreSDiag_u1sDataBuf[1] = (u1)0x06U;                             /* Set the PCI               */
        LCoreSDiag_u1sDataBuf[2] = (u1)0xf2U;                             /* Set the RSID              */
        LCoreSDiag_u1sDataBuf[3] = (u1)ConfSLin_stgNODE.u2SupId;          /* Set the supplier ID (LSB) */
        LCoreSDiag_u1sDataBuf[4] = (u1)(ConfSLin_stgNODE.u2SupId >> 8);   /* Set the supplier ID (MSB) */
        LCoreSDiag_u1sDataBuf[5] = (u1)ConfSLin_stgNODE.u2FunId;          /* Set the function ID (LSB) */
        LCoreSDiag_u1sDataBuf[6] = (u1)(ConfSLin_stgNODE.u2FunId >> 8);   /* Set the function ID (MSB) */
        LCoreSDiag_u1sDataBuf[7] = ConfSLin_stgNODE.u1Variant;            /* Set the variant ID        */
    }
    else                                                                  /* Set the negative response in the other conditions */
    {
        LCoreSDiag_u1sDataBuf[1] = (u1)0x03U;                             /* Set the PCI  */
        LCoreSDiag_u1sDataBuf[2] = (u1)0x7fU;                             /* Set the RSID */
        LCoreSDiag_u1sDataBuf[3] = (u1)0xb2U;                             /* Set D1       */
        LCoreSDiag_u1sDataBuf[4] = (u1)0x12U;                             /* Set D2       */
        LCoreSDiag_u1sDataBuf[5] = (u1)0xffU;                             /* Set D3       */
        LCoreSDiag_u1sDataBuf[6] = (u1)0xffU;                             /* Set D4       */
        LCoreSDiag_u1sDataBuf[7] = (u1)0xffU;                             /* Set D5       */
    }

    LCoreSDiag_u1sDataBuf[0] = LCoreSDiag_u1sNad;                         /* Set the NAD */

    return;
}

/* -------------------------------------------------------------------- */
/* Description : Assign PID                                             */
/* Parameters  : u1tStartIndex = start index                            */
/*               *pu1tNewPids  = pointer to the new PIDs                */
/* Return value: Process result                                         */
/*                OK    = OK                                            */
/*                ERROR = Error                                         */
/* Misc.       : If the specified index value does not exist,           */
/*               the PID is not assigned.                               */
/*               Set also for 00H(unassign).                            */
/*               (PID0 is 80H due to the addition of parity and         */
/*                is effectively ignored.)                              */
/* -------------------------------------------------------------------- */
static u1
LCoreSDiag_u1sAssignIdRange( u1 u1tStartIndex, const u1* pu1tNewPids )
{
    u1 u1tResult;
    u1 u1tIndex;


    u1tResult = (u1)OK;

    u1tIndex  = (u1)4;


    do {

        u1tIndex--;

        /* Value comparison,(Number of messages + Number of event trigger messages), (Start index + u1tIndex) */
        if ( (ConfSLin_u1gMESNUM + ConfSLin_u1gEVENTNUM) <= (u1tStartIndex + u1tIndex) ) {

            if ( pu1tNewPids[u1tIndex] != (u1)LCORESDIAG_u1sASSIGNIDDONOTCARE ) {
                /* Nonexistent index      */
                u1tResult = (u1)ERROR;

                break;
            }

        }
        else if ( ConfSLin_u1gMESNUM <= (u1tStartIndex + u1tIndex) ) {

            /* Target index is the index of the event trigger */

            if ( pu1tNewPids[u1tIndex] != (u1)LCORESDIAG_u1sASSIGNIDDONOTCARE ) {
                /* The assignment of 00H is set as it is, but the PID of PID0 becomes 80H due to the addition of parity, so it is effectively invalid. */

                ConfSLin_u1gEventPid[(u1tStartIndex + u1tIndex) - ConfSLin_u1gMESNUM] = pu1tNewPids[u1tIndex];
            }

        }
        else {

            /* Target index is the index of the message   */

            if ( pu1tNewPids[u1tIndex] != (u1)LCORESDIAG_u1sASSIGNIDDONOTCARE ) {
                /* The assignment of 00H is set as it is, but the PID of PID0 becomes 80H due to the addition of parity, so it is effectively invalid. */

                ConfSLin_u1gMesPid[(u1tStartIndex + u1tIndex)]   = pu1tNewPids[u1tIndex];
            }

        }


    } while( u1tIndex != (u1)0 );

    /* Slave response (always a positive response) */
    LCoreSDiag_u1sDataBuf[0] = LCoreSDiag_u1sNad;   /* Set NAD  */
    LCoreSDiag_u1sDataBuf[1] = (u1)0x01U;           /* Set PCI  */
    LCoreSDiag_u1sDataBuf[2] = (u1)0xF7U;           /* Set RSID */
    LCoreSDiag_u1sDataBuf[3] = (u1)0xFFU;           /* Set D1   */
    LCoreSDiag_u1sDataBuf[4] = (u1)0xFFU;           /* Set D2   */
    LCoreSDiag_u1sDataBuf[5] = (u1)0xFFU;           /* Set D3   */
    LCoreSDiag_u1sDataBuf[6] = (u1)0xFFU;           /* Set D4   */
    LCoreSDiag_u1sDataBuf[7] = (u1)0xFFU;           /* Set D5   */

    return u1tResult;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
