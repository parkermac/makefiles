      SUBROUTINE ana_nudgcoef (ng, tile, model)
!
!! svn $Id: ana_nudgcoef.h 727 2014-03-28 21:46:14Z arango $
!!================================================= Hernan G. Arango ===
!! Copyright (c) 2002-2014 The ROMS/TOMS Group                         !
!!   Licensed under a MIT/X style license                              !
!!   See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This routine sets spatially varying nudging coefficients time-      !
!  scales (1/s). They are used for nuding to climatology in the        !
!  governing equations.                                                !
!                                                                      !
!  It is HIGHLY recommended to write these nudging coefficients into   !
!  input NetCDF NUDNAME instead of using analytical expressions        !
!  below.  It is very easy to introduce parallel bugs.  Also, Users    !
!  can plot their spatial distribution and fine tune their values      !
!  during the pre-proccessing stage for a particular application.      !
!                                                                      !
!  REMARK:  Nudging of free-surface in the vertically integrated       !
!  ======   continuity equation is NOT allowed because it VIOLATES     !
!  mass/volume conservation. If such nudging effects are required,     !
!  it needs to be specified on the momentum equations for (u,v)        !
!  and/or (ubar,vbar). If done on (u,v) only, its effects enter        !
!  the 2D momentum equations via the residual vertically integrated    !
!  forcing term.                                                       !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_ncparam
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile, model
!
!  Local variable declarations.
!
#include "tile.h"
!
      CALL ana_nudgcoef_tile (ng, tile, model,                          &
     &                        LBi, UBi, LBj, UBj,                       &
     &                        IminS, ImaxS, JminS, JmaxS)
!
! Set analytical header file name used.
!
#ifdef DISTRIBUTE
      IF (Lanafile) THEN
#else
      IF (Lanafile.and.(tile.eq.0)) THEN
#endif
        ANANAME(16)=__FILE__
      END IF

      RETURN
      END SUBROUTINE ana_nudgcoef
!
!***********************************************************************
      SUBROUTINE ana_nudgcoef_tile (ng, tile, model,                    &
     &                              LBi, UBi, LBj, UBj,                 &
     &                              IminS, ImaxS, JminS, JmaxS)
!***********************************************************************
!
      USE mod_param
      USE mod_parallel
      USE mod_clima
      USE mod_grid
      USE mod_ncparam
      USE mod_scalars
#ifdef DISTRIBUTE
!
      USE distribute_mod, ONLY : mp_collect
      USE mp_exchange_mod, ONLY : mp_exchange2d
# ifdef SOLVE3D
      USE mp_exchange_mod, ONLY : mp_exchange3d
      USE mp_exchange_mod, ONLY : mp_exchange4d
# endif
#endif
!
      implicit none
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile, model
      integer, intent(in) :: LBi, UBi, LBj, UBj
      integer, intent(in) :: IminS, ImaxS, JminS, JmaxS
!
!  Local variable declarations.
!
      integer :: Iwrk, i, itrc, j, k

      real(r8) :: cff1, cff2

      real(r8), parameter :: IniVal = 0.0_r8

      real(r8), dimension(IminS:ImaxS,JminS:JmaxS) :: wrk

#include "set_bounds.h"
!
!-----------------------------------------------------------------------
!  Set up nudging towards data time-scale coefficients (1/s).
!-----------------------------------------------------------------------
!
!  Initialize.
!
      DO j=JstrT,JendT
        DO i=IstrT,IendT
          wrk(i,j)=0.0_r8
        END DO
      END DO

#if defined NATL
!
!  Set tracer nudging coefficients in the southern and northern edges
!  from a five days time scale at the boundary point to decrease
!  linearly to 60 days seven grids points away from the boundary.
!
      cff1=1.0_r8/(5.0_r8*86400.0_r8)
      cff2=1.0_r8/(60.0_r8*86400.0_r8)
      DO j=JstrT,MIN(8,JendT)
        DO i=IstrT,IendT
          wrk(i,j)=cff2+REAL(8-j,r8)*(cff1-cff2)/7.0_r8
        END DO
      END DO
      DO j=MAX(JstrT,Mm(ng)-7),JendT
        DO i=IstrT,IendT
          wrk(i,j)=cff1+REAL(Mm(ng)-j,r8)*(cff2-cff1)/7.0_r8
        END DO
      END DO

      IF (ANY(LnudgeTCLM(:,ng))) THEN
        DO itrc=1,NTCLM(ng)
          DO k=1,N(ng)
            DO j=JstrT,JendT
              DO i=IstrT,IendT
                CLIMA(ng)%Tnudgcof(i,j,k,itrc)=wrk(i,j)
              END DO
            END DO
          END DO
        END DO
      END IF

#elif defined SCB
!
!  Set tracer nudging coefficients in the southern and northern edges
!  from a five days time scale at the boundary point to decrease
!  linearly to 60 days six grids points away from the boundary.
!
      cff1=1.0_r8/(5.0_r8*86400.0_r8)
      cff2=1.0_r8/(60.0_r8*86400.0_r8)
      DO j=JstrT,MIN(6,JendT)
        DO i=IstrT,IendT
          wrk(i,j)=cff2+REAL(6-j,r8)*(cff1-cff2)/6.0_r8
        END DO
      END DO
      DO j=MAX(JstrT,Mm(ng)+1-6),JendT
        DO i=IstrT,IendT
          wrk(i,j)=cff1+REAL(Mm(ng)+1-j,r8)*(cff2-cff1)/6.0_r8
        END DO
      END DO
      DO i=IstrT,MIN(6,IendT)
        DO j=MAX(JstrT,i),MIN(Mm(ng)+1-i,JendT)
          wrk(i,j)=cff2+REAL(6-i,r8)*(cff1-cff2)/6.0_r8
        END DO
      END DO

      IF (LnudgeM2CLM(ng)) THEN
        DO j=JstrT,JendT
          DO i=IstrT,IendT
            CLIMA(ng)%M2nudgcof(i,j)=wrk(i,j)
          END DO
        END DO
      END IF

      IF (LnudgeM3CLM(ng)) THEN
        DO k=1,N(ng)
          DO j=JstrT,JendT
            DO i=IstrT,IendT
              CLIMA(ng)%M3nudgcof(i,j,k)=wrk(i,j)
            END DO
          END DO
        END DO
      END IF

      IF (ANY(LnudgeTCLM(:,ng))) THEN
        DO itrc=1,NTCLM(ng)
          DO k=1,N(ng)
            DO j=JstrT,JendT
              DO i=IstrT,IendT
                CLIMA(ng)%Tnudgcof(i,j,k,itrc)=wrk(i,j)
              END DO
            END DO
          END DO
        END DO
      END IF
!!
!!  Specify your spatially varying nudging coefficients here.
!!
!!#elif defined ???

#elif defined AE1
! COMPLETELY RECODED 9/7/2012 Parker MacCready
! and again on 4/3/2015 for consistency with new ROMS version
!
! Set tracer nudging coefficients on chosen edges from a
! time set by cff1 at the boundaries to that set by cff2 six
! gridpoints in.  Eventually this should be recoded to have the
! buffer size set by a variable instead of hardwired.
!
! This is coded so that you can turn on and off a given edge
! just by changing the TRUE/FALSE expression in the IF statement
! surrounding it.
!
! It appears that standard procedure may be to set the nudging at
! the outer edge (cff1) to MATCH that set by Tnudg
! and note that Tnudg is set to be 1/(TNUDG*86400) in the code
! ROMS/Utility/read_phypar.F
! This is also what is applied to OUTGOING tracers at the open boundary
! while the potentially much faster nudging OBCFAC*Tnudg is applied to
! INCOMING.  But note that these boundary values are a separate
! operation from the "nudging to climatology"

! set inverse time scales (1/seconds)
  cff1=1.0_r8/(3.0_r8*86400.0_r8)
  cff2=1.0_r8/(60.0_r8*86400.0_r8)

! SOUTHERN EDGE
  IF (.TRUE.) THEN
    DO j=JstrT,MIN(6,JendT)
      DO i=IstrT,IendT
      wrk(i,j)=MAX(wrk(i,j),cff1+REAL(j,r8)*(cff2-cff1)/6.0_r8)
      END DO
    END DO
  END IF

! NORTHERN EDGE
  IF (.TRUE.) THEN	
    DO j=MAX(JstrT,Mm(ng)+1-6),JendT
      DO i=IstrT,IendT
      wrk(i,j)=MAX(wrk(i,j),cff1+REAL(Mm(ng)+1-j,r8)*(cff2-cff1)/6.0_r8)
      END DO
    END DO
  END IF

! WESTERN EDGE
  IF (.TRUE.) THEN
    DO i=IstrT,MIN(6,IendT)
      DO j=JstrT,JendT
      wrk(i,j)=MAX(wrk(i,j),cff1+REAL(i,r8)*(cff2-cff1)/6.0_r8)
      END DO
    END DO
  END IF

! EASTERN EDGE	
  IF (.FALSE.) THEN
    DO i=MAX(IstrT,Lm(ng)+1-6),IendT
      DO j=JstrT,JendT
      wrk(i,j)=MAX(wrk(i,j),cff1+REAL(Lm(ng)+1-i,r8)*(cff2-cff1)/6.0_r8)
      END DO
    END DO
  END IF

! DEBUGGING (plot results with wrk_plot.m)
! Only helpful when NOT running with multiple tiles
  IF (.FALSE.) THEN
    OPEN(17,FILE='wrk.dat',STATUS='REPLACE',ACTION='WRITE')
    DO i=IstrT,IendT
      DO j=JstrT,JendT
        WRITE(17,FMT='(I4,2X,I4,2X,F20.15)') i, j ,wrk(i,j)
      END DO
    END DO
    CLOSE(17)
  END IF

      IF (LnudgeM2CLM(ng)) THEN
        DO j=JstrT,JendT
          DO i=IstrT,IendT
            CLIMA(ng)%M2nudgcof(i,j)=wrk(i,j)
          END DO
        END DO
      END IF

      IF (LnudgeM3CLM(ng)) THEN
        DO k=1,N(ng)
          DO j=JstrT,JendT
            DO i=IstrT,IendT
              CLIMA(ng)%M3nudgcof(i,j,k)=wrk(i,j)
            END DO
          END DO
        END DO
      END IF

      IF (ANY(LnudgeTCLM(:,ng))) THEN
        DO itrc=1,NTCLM(ng)
          DO k=1,N(ng)
            DO j=JstrT,JendT
              DO i=IstrT,IendT
                CLIMA(ng)%Tnudgcof(i,j,k,itrc)=wrk(i,j)
              END DO
            END DO
          END DO
        END DO
      END IF
!!
!!  Specify your spatially varying nudging coefficients here.
!!
!!#elif defined ???


#else
!
!  Default nudging coefficients.  Set nudging coefficients uniformly to
!  the values specified in the standard input file.
!
      IF (LnudgeM2CLM(ng)) THEN
        DO j=JstrT,JendT
          DO i=IstrT,IendT
            CLIMA(ng)%M2nudgcof(i,j)=M2nudg(ng)
          END DO
        END DO
      END IF

# ifdef SOLVE3D
!
      IF (LnudgeM3CLM(ng)) THEN
        DO k=1,N(ng)
          DO j=JstrT,JendT
            DO i=IstrT,IendT
              CLIMA(ng)%M3nudgcof(i,j,k)=M3nudg(ng)
            END DO
          END DO
        END DO
      END IF
!
      IF (ANY(LnudgeTCLM(:,ng))) THEN
        DO itrc=1,NTCLM(ng)
          DO k=1,N(ng)
            DO j=JstrT,JendT
              DO i=IstrT,IendT
                CLIMA(ng)%Tnudgcof(i,j,k,itrc)=Tnudg(itrc,ng)
              END DO
            END DO
          END DO
        END DO
      END IF
# endif
#endif
#ifdef DISTRIBUTE
!
!-----------------------------------------------------------------------
!  Exchage nudging coefficients information.
!-----------------------------------------------------------------------
!
      IF (LnudgeM2CLM(ng)) THEN
        CALL mp_exchange2d (ng, tile, model, 1,                         &
     &                      LBi, UBi, LBj, UBj,                         &
     &                      NghostPoints, .FALSE., .FALSE.,             &
     &                      CLIMA(ng)%M2nudgcof)
      END IF

# ifdef SOLVE3D
!
      IF (LnudgeM3CLM(ng)) THEN
        CALL mp_exchange3d (ng, tile, model, 1,                         &
     &                      LBi, UBi, LBj, UBj, 1, N(ng),               &
     &                      NghostPoints, .FALSE., .FALSE.,             &
     &                      CLIMA(ng)%M3nudgcof)
      END IF
!
      IF (ANY(LnudgeTCLM(:,ng))) THEN
        CALL mp_exchange4d (ng, tile, model, 1,                         &
     &                      LBi, UBi, LBj, UBj, 1, N(ng), 1, NTCLM(ng), &
     &                      NghostPoints, .FALSE., .FALSE.,             &
     &                      CLIMA(ng)%Tnudgcof)
      END IF
# endif
#endif

      RETURN
      END SUBROUTINE ana_nudgcoef_tile
