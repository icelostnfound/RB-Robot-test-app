#ifndef _SAMPLE_8254_8_H
#define _SAMPLE_8254_8_H

#ifdef __cplusplus
extern "C" {
#endif

//Adlink head file////////
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"
//////////////////////////

#define YES (1)
#define NO  (0)
#define ON  (1)
#define OFF (0)
#define __TO_RADIAN( _x_ ) ( (_x_) * 3.14159265 / 180.0 )
#define __MAX_DO_CH (24)
#define __MAX_DI_CH (24)

void get_version_info( I32 Board_ID, I32 *Array );
I32 load_parameters_from_file_sample( const char *FilePath );
I32 check_motion_done_example(I32 Axis_ID, I32 *Stop_Code);
void home_move_example( I32 Axis_ID );
void velocity_move_example( I32 Axis_ID );
void jog_move_continuous_mode_example( I32 Axis_ID );
void p2p_example( I32 Axis_ID );
void deceleration_stop_example( I32 Axis_ID );
void emg_stop_example( I32 Axis_ID );
void interpolation_2D_line_example( I32 *Axis_ID_Array );
void interpolation_2D_arc_example( I32 *Axis_ID_Array );
void interpolation_3D_arc_example( I32 *Axis_ID_Array );
void interpolation_3D_helical_example( I32 *Axis_ID_Array );
void contour_2D_example( I32 *Axis_ID_Array );
void point_table_2D_example( I32 Board_ID, I32 *Axis_ID_Array );
void gear_example( I32 *Axis_ID_Array );
I32 interrupt_example( I32 Board_ID, I32 Axis_num );
void DIO_sample( I32 Board_ID );
void AIO_sample( I32 Board_ID );





#ifdef __cplusplus
}
#endif

#endif

