/*
** svn $Id: upwelling.h 795 2016-05-11 01:42:43Z arango $
*******************************************************************************
** Copyright (c) 2002-2016 The ROMS/TOMS Group                               **
**   Licensed under a MIT/X style license                                    **
**   See License_ROMS.txt                                                    **
*******************************************************************************
**
** Options for a LiveOcean run.
**
** Application flag:   [specified in the filename in lowercase]
** Input script:       [depends on forecast day].in
*/

#define UV_ADV
#define UV_COR
#define UV_QDRAG

#define TS_MPDATA
#define TS_DIF2
#define SALINITY
#define NONLIN_EOS
#define SOLAR_SOURCE /* use if solar radiation source term         WHY?      */
#define DJ_GRADPS
#define MIX_GEO_TS
#define SOLVE3D
#define MASKING
#define SPHERICAL

#define BULK_FLUXES
#define LONGWAVE_OUT

#undef ANA_NUDGCOEF
#define ANA_BTFLUX
#define ANA_BSFLUX
#define ANA_SSFLUX
#define ANA_STFLUX

#define GLS_MIXING
#if defined GLS_MIXING
# define CANUTO_A
# define N2S2_HORAVG
# define RI_SPLINES
#endif

#define RADIATION_2D

#undef RAMP_TIDES

#define SSH_TIDES
#define UV_TIDES
#define ADD_FSOBC
#define ADD_M2OBC

#if defined BIO_FENNEL  || defined ECOSIM || \
    defined NPZD_POWELL || defined NEMURO
# define ANA_BIOLOGY
# define ANA_SPFLUX
# define ANA_BPFLUX
# define ANA_SRFLUX
#endif

#undef BIOLOGY
#undef NPZD2O_BANAS


#ifdef NPZD2O_BANAS
#  undef DIAGNOSTICS
#  undef DIAGNOSTICS_BIO
#  undef DIAGNOSTICS_TS
#  define BIO_SEDIMENT
#  define OXYGEN
#  define CARBON
#  define pCO2_RZ
#  define ANA_SPFLUX
#  define ANA_BPFLUX
#  define SDEPEND_LIGHTATT
#  define OPT_UPTAKE
#  define NO_GROW_SALISH
#  undef  NPZDO_BANAS_DEBUG
#  undef COAG_PHYTOS
#endif

#undef BIO_FENNEL
#undef ECOSIM
#undef NEMURO
#undef NPZD_FRANKS
#undef NPZD_IRON
#undef NPZD_POWELL

#undef PERFECT_RESTART

#ifdef PERFECT_RESTART
# undef  AVERAGES
# undef  DIAGNOSTICS_BIO
# undef  DIAGNOSTICS_TS
# undef  DIAGNOSTICS_UV
# define OUT_DOUBLE
#endif
