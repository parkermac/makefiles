/*
** svn $Id: upwelling.h 709 2014-01-23 20:09:38Z arango $
*******************************************************************************
** Copyright (c) 2002-2014 The ROMS/TOMS Group                               **
**   Licensed under a MIT/X style license                                    **
**   See License_ROMS.txt                                                    **
*******************************************************************************
**
** Options for my run.
**
** Application flag:   [specified in the filename (in lowercase)]
** Input script:       [depends on forecast day].in
*/

/* OPTIONS associated with momentum equations:                               */
#define UV_ADV      /* use to turn ON or OFF advection terms                 */
#define UV_COR      /* use to turn ON or OFF Coriolis term                   */
#define UV_QDRAG    /* use to turn ON or OFF quadratic bottom friction       */

/* OPTIONS associated with tracers equations:                                */
#define TS_MPDATA   /* use if recursive MPDATA 3D advection                  */
#define SALINITY    /* use if having salinity                                */

/* Pressure gradient algorithm OPTIONS:                                      */
#define DJ_GRADPS   /* use if splines density Jacobian (Shchepetkin, 2000)   */

/* Model configuration OPTIONS:                                              */
#define SOLVE3D     /* use if solving 3D primitive equations                 */
#define MASKING     /* use if land/sea masking                               */
#define DIAGNOSTICS_UV /* use if writing out momentum diagnostics            */
#define DIAGNOSTICS_TS /* use if writing out tracer diagnostics              */
#define AVERAGES    /* use to save averages                                  */
#define SPHERICAL   /* use if analytical spherical grid                      */

/* OPTIONS for analytical fields configuration:                              */
#define ANA_NUDGCOEF
#define ANA_BSFLUX  /* use if analytical bottom salinity flux                */
#define ANA_BTFLUX  /* use if analytical bottom temperature flux             */
#define ANA_SSFLUX  /* use if analytical surface salinity flux               */
#define ANA_STFLUX  /* use if analytical surface net heat flux               */
#define ANA_SMFLUX  /* use if analytical surface momentum flux               */

/* OPTIONS for horizontal mixing of tracers:                                 */
#define MIX_GEO_TS  /* use if mixing on geopotential (constant Z) surfaces   */

/* OPTIONS for vertical turbulent mixing scheme of momentum and tracers      **
** (activate only one closure):                                              */
#define GLS_MIXING  /* use if Generic Length-Scale mixing                    */

/* OPTIONS for the Generic Length-Scale closure (Warner et al., 2005):       */
#ifdef GLS_MIXING
# define CANUTO_A   /*  use if Canuto A-stability function formulation        */
# define N2S2_HORAVG /* use if horizontal smoothing of buoyancy/shear         */
# define RI_SPLINES /* use if splines reconstruction for vertical sheer       */
#endif

/* Lateral boundary conditions OPTIONS:                                      */
#define RADIATION_2D /* use if tangential phase speed in radiation conditions*/

/* OPTIONS for tidal forcing at open boundaries:                             */
#define SSH_TIDES   /* use if imposing tidal elevation                       */
#define UV_TIDES    /* use if imposing tidal currents                        */
#define RAMP_TIDES  /* use if ramping (over one day) tidal forcing           */
#define ADD_FSOBC   /* use to add tidal elevation to processed OBC data      */
#define ADD_M2OBC   /* use to add tidal currents  to processed OBC data      */

/* Climatological fields are now hadled in the .in file                      */

/* Nearshore and shallow water model OPTIONS:                                */
#undef WET_DRY      /* use to activate wetting and drying                    */

/* NetCDF input/output OPTIONS:                                              */
#define PERFECT_RESTART /* use to include perfect restart variables          */
