/* -------------------------------------------------------------------- */
/* Including Device File                                                */
/* -------------------------------------------------------------------- */
#ifdef __CCRL__
#include "./iodefine.h"
#endif

/* -------------------------------------------------------------------- */
/* Pragma Definitions                                                   */
/* -------------------------------------------------------------------- */


/* -------------------------------------------------------------------- */
/* Including Header Files                                               */
/* -------------------------------------------------------------------- */
#include "./conflin_0.h"


/* -------------------------------------------------------------------- */
/* Type Definitions                                                     */
/* -------------------------------------------------------------------- */


/* -------------------------------------------------------------------- */
/* Macro Definitions                                                     */
/* -------------------------------------------------------------------- */
#ifdef __CCRL__
#define Drv_vogNop()            { __nop(); }
#define Drv_vogEi()             { __EI(); }
#define Drv_vogDi()             { __DI(); }
#endif

#define Drv_vogTogglePort( p )  { (p) = ~(p); }

#define _FRAME4_SIGNAL_NUM (32U)

/* -------------------------------------------------------------------- */
/* Prototype Definitions                                                */
/* -------------------------------------------------------------------- */
void main( void );


/* -------------------------------------------------------------------- */
/* Constant Definitions                                                 */
/* -------------------------------------------------------------------- */


/* -------------------------------------------------------------------- */
/* Variable Definitions                                                 */
/* -------------------------------------------------------------------- */


/* -------------------------------------------------------------------- */
/* Function Definitions                                                 */
/* -------------------------------------------------------------------- */


/* -------------------------------------------------------------------- */
/* Description     : Sample Program                                     */
/* Arguments       : None                                               */
/* Return value    : None                                               */
/* Remark          :                                                    */
/* -------------------------------------------------------------------- */
void main( void )
{
    l_bool  blt_Frm1Data             = 0U;
    volatile l_u8    u8t_Frm2Data    = 0U;
    l_u16   u16t_Frm3Data            = 0U;
    volatile l_bool  blt_Frm4Array[ _FRAME4_SIGNAL_NUM ] = {
        0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
        0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
        0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
        0U, 0U
    };
    l_u8    u8t_Cnt;

    const u1 u1t_Frm4SigName[ _FRAME4_SIGNAL_NUM ] = {
        SIG4,  SIG5,  SIG6,  SIG7,  SIG8,  SIG9,  SIG10, SIG11, SIG12, SIG13,
        SIG14, SIG15, SIG16, SIG17, SIG18, SIG19, SIG20, SIG21, SIG22, SIG23,
        SIG24, SIG25, SIG26, SIG27, SIG28, SIG29, SIG30, SIG31, SIG32, SIG33,
        SIG34, SIG35,
    };

    /*----------------------------------*/
    /* LIN tranceiver(Philips TJA1020T) */
    P1_bit.no3   = 1U;
    POM1_bit.no3 = 0U;
    PM1_bit.no3  = 0U;

    P5_bit.no3   = 1U;
    PM5_bit.no3  = 0U;


    /*----------------------------------*/
    /* Toggle port                      */
    P10_bit.no1  = 0U;      /* for Signal #1 (Master <-- Slave) in Frame A (SIG16) */
    PM10_bit.no1 = 0U;
    PMC10_bit.no1 = 0U;

    P10_bit.no2  = 0U;      /* for Signal #2 (Master --> Slave) in Frame B (SIG16) */
    PM10_bit.no2 = 0U;
    PMC10_bit.no2 = 0U;

    P10_bit.no3  = 0U;      /* for Signal #3 (Master <-- Slave) in Frame C (SIG16) */
    PM10_bit.no3 = 0U;
    PMC10_bit.no3 = 0U;

    P10_bit.no4  = 0U;      /* for Signal #4 (Master --> Slave) in Frame D (SIG32) */
    PM10_bit.no4 = 0U;
    PMC10_bit.no4 = 0U;


    Drv_vogEi();

    if( l_sys_init() == (u1)0U )
    {
        if( l_ifc_init( LIN_CHANNEL0 ) == (u1)0U )
        {

            while( 1U == 1U )
            {
                /* Wait for my send completion */
                if( l_flg_tst( SIG1 ) == 0U )
                {
                    /* Write new data */
                    l_bool_wr( SIG1, blt_Frm1Data );
                    blt_Frm1Data = 1U - blt_Frm1Data;
                    Drv_vogTogglePort( P10_bit.no1 );
                }

                /* Wait for response from Master */
                if( l_flg_tst( SIG2 ) != 0U )
                {
                    /* Read new data */
                    u8t_Frm2Data = l_bool_rd( SIG2 );
                    l_flg_clr( SIG2 );
                    Drv_vogTogglePort( P10_bit.no2 );
                }

                /* Wait for my send completion */
                if( l_flg_tst( SIG3 ) == 0U )
                {
                    /* Write new data */
                    l_u16_wr( SIG3, u16t_Frm3Data++ );
                    Drv_vogTogglePort( P10_bit.no3 );
                }

                /* Check [SIG4 to SIG35] */
                /* Wait for response from Master */
                for( u8t_Cnt = 0U ; u8t_Cnt < _FRAME4_SIGNAL_NUM ; u8t_Cnt++ )
                {
                    if( l_flg_tst( u1t_Frm4SigName[ u8t_Cnt ] ) == 0U )
                    {
                        /* If a signal has not been received, break and go to next interval. */
                        break;
                    }
                }

                if( u8t_Cnt == _FRAME4_SIGNAL_NUM )
                {
                    /* Read new 32 data */
                    for( u8t_Cnt = 0U ; u8t_Cnt < _FRAME4_SIGNAL_NUM ; u8t_Cnt++ )
                    {
                        blt_Frm4Array[ u8t_Cnt ] = l_bool_rd( u1t_Frm4SigName[ u8t_Cnt ] );
                        l_flg_clr( u1t_Frm4SigName[ u8t_Cnt ] );
                    }
                    Drv_vogTogglePort( P10_bit.no4 );
                }
            } /* end of while */
        }
    }

    return;
}

/* end of file */
