#include <windows.h>
#include <math.h>
#include "Motion.h"


void get_version_info( I32 Board_ID, I32 *Array )
{
  // Get DLL Version
  Array[0] = APS_version();

  // Get Driver version
  APS_get_device_info( Board_ID, 0x10, &Array[1] );
  
  // Get DSP kernel version
  APS_get_device_info( Board_ID, 0x40, &Array[2] ); 
  
  // Get FPGA version
  APS_get_device_info( Board_ID, 0x21, &Array[3] );

  // Get PCB version
  APS_get_device_info( Board_ID, 0x30, &Array[4] );
}

I32 load_parameters_from_file_sample( const char *FilePath )
{
	I32 ret = 0;
	ret = APS_load_param_from_file( FilePath );
	return ret;
}

I32 check_motion_done_example(I32 Axis_ID, I32 *Stop_Code)
{
	I32 axis_id     = Axis_ID;
    I32 msts        = 0;
    
    msts = APS_motion_status(axis_id); // Get motion status
    msts = (msts >> 5) & 1;            // Get motion done bit
          
    // Get stop code.
    APS_get_stop_code(axis_id, Stop_Code);

    if (msts == 1)
    {
		// Check move success or not
        msts = APS_motion_status(axis_id); // Get motion status
        msts = (msts >> 16) & 1;           // Get abnormal stop bit
                
        if (msts == 1)
        { // Error handling ...
			APS_get_stop_code(axis_id, Stop_Code);
			return -1; //error
        }
        else
        {   // Motion success.
            return 1;
		}
	}
	
	return 0; //Now are in motion
}

void home_move_example( I32 Axis_ID )
{
  //This example shows how home move operation
  I32 axis_id = Axis_ID;
  I32 return_code;

  // 1. Select home mode and config home parameters 
  APS_set_axis_param( axis_id, PRA_HOME_MODE,      0 ); // Set home mode
  APS_set_axis_param( axis_id, PRA_HOME_DIR,       1 ); // Set home direction
  APS_set_axis_param( axis_id, PRA_HOME_CURVE,     0 ); // Set acceleration paten (T-curve)
  APS_set_axis_param( axis_id, PRA_HOME_ACC, 10000 ); // Set homing acceleration rate
  APS_set_axis_param( axis_id, PRA_HOME_VM,   6250 ); // Set homing maximum velocity.
  APS_set_axis_param( axis_id, PRA_HOME_VO,    50000 ); // Set homing VO speed
  APS_set_axis_param( axis_id, PRA_HOME_EZA,       0 ); // Set EZ signal alignment (yes or no)
  APS_set_axis_param( axis_id, PRA_HOME_SHIFT,     0 ); // Set home position shfit distance. 
  APS_set_axis_param( axis_id, PRA_HOME_POS,       0 ); // Set final home position.


  //Check servo on or not
  APS_set_servo_on( axis_id, ON ); 
  Sleep( 500 ); // Wait stable.
  
  // 2. Start home move
  return_code = APS_home_move( axis_id ); //Start homing 
  if( return_code != ERR_NoError )
  { /* Error handling */ }

}

void velocity_move_example( I32 Axis_ID )
{
  I32 axis_id = Axis_ID;
  F64 speed_1 =  500.0;
 
  APS_set_axis_param_f( axis_id, PRA_STP_DEC, 10000.0 );
  APS_set_axis_param_f( axis_id, PRA_CURVE,      0.5 ); //Set acceleration rate
  APS_set_axis_param_f( axis_id, PRA_ACC,    10000.0 ); //Set acceleration rate
  APS_set_axis_param_f( axis_id, PRA_DEC,    10000.0 ); //Set deceleration rate
 
  //Check servo on or not
  if( !( ( APS_motion_io_status( axis_id ) >> MIO_SVON ) & 1 ) )  
  {
	 APS_set_servo_on( axis_id, ON ); 
	 Sleep( 500 ); // Wait stable.
  }

  APS_vel( axis_id, 0, speed_1, 0 );  // Start a velocity move
  
}

void jog_move_continuous_mode_example( I32 Axis_ID )
{
	APS_set_axis_param( Axis_ID, PRA_JG_MODE, 0 );  // Set jog mode
	APS_set_axis_param( Axis_ID, PRA_JG_DIR,  0 );  // Set jog direction

	APS_set_axis_param_f( Axis_ID, PRA_JG_CURVE, 0.0  );
	APS_set_axis_param_f( Axis_ID, PRA_JG_ACC, 1000.0 );
	APS_set_axis_param_f( Axis_ID, PRA_JG_DEC, 1000.0 );
	APS_set_axis_param_f( Axis_ID, PRA_JG_VM,  100.0  );

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( Axis_ID, 1 ); 
		Sleep( 500 ); // Wait stable.
	}

	// Create a rising edge.
	APS_jog_start( Axis_ID, OFF );  //Jog Off
	APS_jog_start( Axis_ID, ON );  //Jog ON

	// Sleep( 200 );

	// APS_jog_start( I32 Axis_ID, I32 On ); // Jog off
}

void p2p_example( I32 Axis_ID )
{
	I32 ret = 0;
	
	// Config speed profile parameters.
	ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
	ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
	ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
	ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
	{
		ret = APS_set_servo_on( Axis_ID, ON ); 
		Sleep( 100 ); // Wait stable.
	}

	// Start a relative p to p move
	ret = APS_ptp( Axis_ID, OPT_RELATIVE, 1000.0 , 0 );

	/*
	// Method 2, ptp move with maximum velocity.
	APS_ptp_v( 
		  Axis_ID  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, 500.0   // F64 Position
		, 1000.0  // F64 Vm
		, 0       // ASYNCALL *Wait
	);
	*/

	/*
	// Method 3, ptp move with all speed parameters.
	APS_ptp_all( 
		  Axis_ID  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, 500.0   // F64 Position
		, 0.0     // F64 Vs
		, 1000.0  // F64 Vm
		, 0.0     // F64 Ve
		, 10000.0 // F64 Acc
		, 10000.0 // F64 Dec
		, 0.5 // F64 SFac
		, 0   // ASYNCALL *Wait
	);
	*/
}

void deceleration_stop_example( I32 Axis_ID )
{
	APS_set_axis_param_f( Axis_ID, PRA_STP_DEC, 10000.0 );
	APS_stop_move( Axis_ID );
}

void emg_stop_example( I32 Axis_ID )
{
	// Stop immediately
	APS_emg_stop( Axis_ID );
}

void interpolation_2D_line_example( I32 *Axis_ID_Array )
{
	F64 PositionArray[2] = {1000.0 , 2000.0 };
	F64 TransPara = 0;
	
	// config speed profile
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_SF,  0.5     );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_ACC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_DEC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_VM,  1000.0  );

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID_Array[0] ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( Axis_ID_Array[0], 1 ); 
		Sleep( 100 ); // Wait stable.
	}

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID_Array[1] ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( Axis_ID_Array[1], 1 ); 
		Sleep( 100 ); // Wait stable.
	}

	// Start a 2 dimension linear interpolation
	APS_line( 
		  2 // I32 Dimension
		, Axis_ID_Array // I32 *Axis_ID_Array
		, OPT_RELATIVE  // I32 Option
		, PositionArray // F64 *PositionArray
		, &TransPara    // F64 *TransPara
		, 0 // ASYNCALL *Wait
	);
}

void interpolation_2D_arc_example( I32 *Axis_ID_Array )
{
	F64 CenterArray[2] = {1000.0 , 0.0 };
	F64 Angle = __TO_RADIAN( 90.0 ); 
	F64 TransPara = 0;
	
	// config speed profile
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_SF,  0.5     );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_ACC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_DEC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_VM,  1000.0  );

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID_Array[0] ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( Axis_ID_Array[0], 1 ); 
		Sleep( 100 ); // Wait stable.
	}

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID_Array[1] ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( Axis_ID_Array[1], 1 ); 
		Sleep( 100 ); // Wait stable.
	}

	// Start a 2 dimension linear interpolation
	APS_arc2_ca( 
		  Axis_ID_Array // I32 *Axis_ID_Array
		, OPT_RELATIVE  // I32 Option
		, CenterArray   // F64 *CenterArray
		, Angle         // F64 Angle
		, &TransPara    // F64 *TransPara
		, 0 // ASYNCALL *Wait 
	);
}

void interpolation_3D_arc_example( I32 *Axis_ID_Array )
{

	F64 CenterArray[3] = { 1000.0 , 1000.0, 0 };
	F64 EndArray[3]    = { 1000.0 , 1000.0, 1000.0 * sqrt( 2 ) };
	I16 Dir = 0;
	F64 TransPara = 0;
	int i;

	// config speed profile
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_SF,  0.5     );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_ACC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_DEC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_VM,  2000.0  );

	//Check servo on or not
	for( i = 0; i < 3; i++ )
	{
		if( !( ( APS_motion_io_status( Axis_ID_Array[i] ) >> MIO_SVON ) & 1 ) )  
			APS_set_servo_on( Axis_ID_Array[i], 1 ); 
	}
	Sleep( 100 ); // Wait stable.

	APS_arc3_ce( 
		  Axis_ID_Array // I32 *Axis_ID_Array
		, OPT_RELATIVE  // I32 Option
		, CenterArray   // F64 *CenterArray
		, EndArray      // F64 *EndArray
		, Dir           // I16 Dir
		, &TransPara    //F64 *TransPara
		, 0 // ASYNCALL *Wait 
	);
}

void interpolation_3D_helical_example( I32 *Axis_ID_Array )
{

	F64 CenterArray[3] = { 1000.0 , 1000.0, 0 };
	F64 NormalArray[3] = { 0, 0, 1 };
	F64 Angle  = __TO_RADIAN( 720.0 );  
	F64 DeltaH = 500.0;
	F64 FinalR = 200.0;
	F64 TransPara = 0;
	int i;

	// config speed profile
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_SF,  0.5     );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_ACC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_DEC, 10000.0 );
	APS_set_axis_param_f( Axis_ID_Array[0], PRA_VM,  2000.0  );

	//Check servo on or not
	for( i = 0; i < 3; i++ )
	{
		if( !( ( APS_motion_io_status( Axis_ID_Array[i] ) >> MIO_SVON ) & 1 ) )  
			APS_set_servo_on( Axis_ID_Array[i], 1 ); 
	}
	Sleep( 100 ); // Wait stable.

	APS_spiral_ca( 
		  Axis_ID_Array // I32 *Axis_ID_Array
		, OPT_RELATIVE  // I32 Option
		, CenterArray   // F64 *CenterArray
		, NormalArray   // F64 *NormalArray
		, Angle         // F64 Angle
		, DeltaH        // F64 DeltaH
		, FinalR        // F64 FinalR
		, &TransPara    // F64 *TransPara
		, 0 // ASYNCALL *Wait 
	);
}

void contour_2D_example( I32 *Axis_ID_Array )
{
	int i;
	F64 TransPara = 0;
	F64 PositionArray[4] = { 1000,    0,    // segment 1
		                        0, 1000 };  // segment 3
	F64 CenterArray[4]   = { 1000,  500,    // segment 2 (center)
	                            0,  500 };  // segment 4 (center)
	F64 EndArray[4]      = { 1000, 1000,    // segment 2 (end)
	                            0,    0 };  // segment 4 (end)
	

	//Check servo on or not
	for( i = 0; i < 2; i++ )
	{
		if( !( ( APS_motion_io_status( Axis_ID_Array[i] ) >> MIO_SVON ) & 1 ) )  
			APS_set_servo_on( Axis_ID_Array[i], 1 ); 
	}
	Sleep( 500 ); // Wait stable.

	// Execute 4 interpolation move useing bufferd mode. Note option using "ITP_OPT_BUFFERED"
	
	APS_line_all( 2, Axis_ID_Array, OPT_ABSOLUTE | ITP_OPT_BUFFERED, PositionArray, &TransPara
		,     0.0 // F64 Vs
		,  1000.0 // F64 Vm
		,  1000.0 // F64 Ve
		, 10000.0 // F64 Acc
		, 10000.0 // F64 Dec
		,     0.0 // F64 SFac
		,       0 // ASYNCALL *Wait 
	);

	APS_arc2_ce_all( Axis_ID_Array, OPT_ABSOLUTE | ITP_OPT_BUFFERED, CenterArray, EndArray, 0, &TransPara
	    ,  1000.0 // F64 Vs
		,  1000.0 // F64 Vm
		,  1000.0 // F64 Ve
		, 10000.0 // F64 Acc
		, 10000.0 // F64 Dec
		,     0.0 // F64 SFac
		,       0 // ASYNCALL *Wait 
	);

	APS_line_all( 2, Axis_ID_Array, OPT_ABSOLUTE | ITP_OPT_BUFFERED, PositionArray + 2, &TransPara
	    ,  1000.0 // F64 Vs
		,  1000.0 // F64 Vm
		,  1000.0 // F64 Ve
		, 10000.0 // F64 Acc
		, 10000.0 // F64 Dec
		,     0.0 // F64 SFac
		,       0 // ASYNCALL *Wait 
	);

	APS_arc2_ce_all( Axis_ID_Array, OPT_ABSOLUTE | ITP_OPT_BUFFERED, CenterArray + 2, EndArray + 2, 0, &TransPara
		,  1000.0 // F64 Vs
		,  1000.0 // F64 Vm
		,     0.0 // F64 Ve
		, 10000.0 // F64 Acc
		, 10000.0 // F64 Dec
		,     0.0 // F64 SFac
		,       0 // ASYNCALL *Wait 
	);
}

void point_table_2D_example( I32 Board_ID, I32 *Axis_ID_Array )
{
	I32 boardId = Board_ID;
	I32 ptbId = 0; //Point table id 0
	I32 dimension = 2; //2D point table
	PTSTS Status;
	PTLINE Line;
	PTA2CA Arc2;
	I32 doChannel = 0; //Do channel 0
	I32 doOn = 0;
	I32 doOff = 1;
	I32 i = 0;

	//Check servo on or not
	for( i = 0; i < dimension; i++ )
	{
		if( !( ( APS_motion_io_status( Axis_ID_Array[i] ) >> MIO_SVON ) & 1 ) )  
			APS_set_servo_on( Axis_ID_Array[i], 1 ); 
	}
	Sleep( 500 ); // Wait stable.

	//Enable point table
	APS_pt_disable( boardId, ptbId );
	APS_pt_enable( boardId, ptbId, dimension, Axis_ID_Array );

	//Configuration
	APS_pt_set_absolute( boardId, ptbId ); //Set to absolute mode
	APS_pt_set_trans_buffered( boardId, ptbId ); //Set to buffer mode
	APS_pt_set_acc( boardId, ptbId, 10000 ); //Set acc
	APS_pt_set_dec( boardId, ptbId, 10000 ); //Set dec

	//Get status
	//BitSts;	//b0: Is PTB work? [1:working, 0:Stopped]
				//b1: Is point buffer full? [1:full, 0:not full]
				//b2: Is point buffer empty? [1:empty, 0:not empty]
				//b3~b5: reserved
	APS_get_pt_status( boardId, ptbId, &Status );
	if( !(Status.BitSts & 0x02) ) //Buffer is not Full
	{
		//Set 1st move profile
		APS_pt_set_vm( boardId, ptbId, 10000 ); //Set vm to 10000
		APS_pt_set_ve( boardId, ptbId, 5000 ); //Set ve to 5000
		//Set do command to sync with 1st point
		APS_pt_ext_set_do_ch( boardId, ptbId, doChannel, doOn ); //Set Do channel 0 to on
		//Set pt arc data
		Arc2.Center[0] = 0;
		Arc2.Center[1] = 5000;
		Arc2.Angle = __TO_RADIAN( -180 ); //180 degree
		Arc2.Index[0] = 0;
		Arc2.Index[1] = 1;
		//Push 1st point to buffer
		APS_pt_arc2_ca( boardId, ptbId, &Arc2 , &Status );
	}

	//Get status
	APS_get_pt_status( boardId, ptbId, &Status );
	if( !(Status.BitSts & 0x02) ) //Buffer is not Full
	{
		//Set 2nd move profile
		APS_pt_set_vm( boardId, ptbId, 12000 ); //Set vm to 12000
		APS_pt_set_ve( boardId, ptbId, 6000 ); //Set ve to 6000
		//Set do command to sync with 2nd point
		APS_pt_ext_set_do_ch( boardId, ptbId, doChannel, doOff ); //Set Do channel 0 to on
		//Set pt line data
		Line.Dim = 2;
		Line.Pos[0] = 10000.0;
		Line.Pos[1] = 10000.0;
		//Push 2nd point to buffer
		APS_pt_line( boardId, ptbId, &Line, &Status );
	}

	//Get status
	APS_get_pt_status( boardId, ptbId, &Status );
	if( !(Status.BitSts & 0x02) ) //Buffer is not Full
	{
		//Set 3rd move profile
		APS_pt_set_vm( boardId, ptbId, 10000 ); //Set vm to 10000
		APS_pt_set_ve( boardId, ptbId, 5000 ); //Set ve to 5000
		//Set do command to sync with 3rd point
		APS_pt_ext_set_do_ch( boardId, ptbId, doChannel, doOn ); //Set Do channel 0 to off
		//Set pt arc data
		Arc2.Center[0] = 10000;
		Arc2.Center[1] = 5000;
		Arc2.Angle = __TO_RADIAN( 180 ); //180 degree
		Arc2.Index[0] = 0;
		Arc2.Index[1] = 1;
		//Push 3rd point to buffer
		APS_pt_arc2_ca( boardId, ptbId, &Arc2 , &Status );
	}

	//Get status
	APS_get_pt_status( boardId, ptbId, &Status );
	if( !(Status.BitSts & 0x02) ) //Buffer is not Full
	{
		//Set 4th move profile
		APS_pt_set_vm( boardId, ptbId, 12000 ); //Set vm to 12000
		APS_pt_set_ve( boardId, ptbId, 500 ); //Set ve to 500
		//Set do command to sync with 4th point
		APS_pt_ext_set_do_ch( boardId, ptbId, doChannel, doOff ); //Set Do channel 0 to on
		//Set pt line data
		Line.Dim = 2;
		Line.Pos[0] = 0.0;
		Line.Pos[1] = 0.0;
		//Push 4th point to buffer
		APS_pt_line( boardId, ptbId, &Line, &Status );
	}

	APS_pt_start( boardId, ptbId );

}

void gear_example( I32 *Axis_ID_Array )
{
	I32 masterAxis = Axis_ID_Array[0];
	I32 slaveAxis = Axis_ID_Array[1];
	I32 i = 0;
	I32 status = 0;

	//Check servo on or not
	for( i = 0; i < 2; i++ )
	{
		if( !( ( APS_motion_io_status( Axis_ID_Array[i] ) >> MIO_SVON ) & 1 ) )  
			APS_set_servo_on( Axis_ID_Array[i], 1 ); 
	}
	Sleep( 100 ); // Wait stable.

	APS_get_gear_status( slaveAxis, &status );
	//status == 0, Disable mode
	//status == 1, Gear mode
	//status == 2. Gantry mode
	if( status == 0 )
	{
		APS_set_axis_param( slaveAxis, PRA_GEAR_MASTER, masterAxis ); //Select master Axis command to be slaveAxis's master
		
		APS_set_axis_param_f( slaveAxis, PRA_GEAR_ENGAGE_RATE, 10000.0 ); //Gear engage rate = 1000.0 //Gear mode only
		APS_set_axis_param_f( slaveAxis, PRA_GEAR_RATIO, 2.0 ); //Gear ratio //Gear mode only

		//E-gear gantry mode protection level 1, | fbk_master – fbk_slave | >= 10000, sd-stop motion //Gantry mode only
		APS_set_axis_param_f( slaveAxis, PRA_GANTRY_PROTECT_1, 10000.0 ); 
		//E-gear gantry mode protection level 2, | fbk_master – fbk_slave | >= 10000, both servo-off //Gantry mode only
		APS_set_axis_param_f( slaveAxis, PRA_GANTRY_PROTECT_2, 20000.0 ); 

		//Set to Standard mode (0: Disable, 1: Standard mode, 2: Gantry mode.)
		APS_start_gear( slaveAxis, 1 ); 
	}

	// Start a relative p to p move
	APS_ptp( masterAxis, OPT_RELATIVE, 1000.0 , 0 );
	Sleep(1200); //wait motion done

	APS_start_gear( slaveAxis, 0 ); //Disable gear/gantry
}

I32 interrupt_example( I32 Board_ID, I32 Axis_num )
{
  // This example shows how interrupt functions work.
  I32 board_id = Board_ID;
  I32 int_no;      // Interrupt number
  I32 return_code; // function return code
  I32 item   = Axis_num;    // Axis #? interrupt
  I32 factor = 12 ; // factor number = 12 IMDN interrupt

  // Step 1: set interrupt factor ( factor = IMDN )
  // 設定要等待的中斷事件
  int_no = APS_set_int_factor( board_id, item, factor, 1 );  

  // Step 2: set interrupt main switch 
  // 設定中斷總開關
  APS_int_enable( board_id, 1 ); // Enable the interrupt main switch

  // Step 3: wait interrupt.
  // 等待中斷觸發
  return_code = APS_wait_single_int( int_no, -1 ); //Wait interrupt
	
  if( return_code == ERR_NoError )
  { //Interrupt occurred	
    //Step 4: 重置中斷為為觸發狀態
	APS_reset_int( int_no );
  }

  // Step 5: Disable interrupt at the end of program.
  // 關閉中斷事件和中斷總開關
  APS_set_int_factor( board_id, item, factor, 0 ); 
  APS_int_enable( board_id, 0 ); 

  return return_code;
}

void DIO_sample( I32 Board_ID )
{
	
	I32 digital_output_value = 0;
	I32 digital_input_value  = 0;
	I32 do_ch[__MAX_DO_CH];
	I32 di_ch[__MAX_DI_CH];
	int i;

	//****** Read digital output channels *****************************
	APS_read_d_output( Board_ID
		, 0                     // I32 DO_Group
		, &digital_output_value // I32 *DO_Data
	);

	for( i = 0; i < __MAX_DO_CH; i++ )
		do_ch[i] = ( ( digital_output_value >> i ) & 1 );

	//************ Write digital output channels examples *************
	do_ch[0] = 1;  // set 0 or 1
	do_ch[2] = 1;  // set 0 or 1
	do_ch[4] = 1;  // set 0 or 1

	digital_output_value = 0;
	for( i = 0; i < __MAX_DO_CH; i++ )
		digital_output_value |= ( do_ch[i] << i );

	
	APS_write_d_output( Board_ID
		, 0                     // I32 DO_Group
		, digital_output_value  // I32 DO_Data
	);
	//*******************************************************************

	//**** Read digital input channels **********************************
	APS_read_d_input( Board_ID
		, 0                    // I32 DI_Group
		, &digital_input_value // I32 *DI_Data
	);

	for( i = 0; i < __MAX_DI_CH; i++ )
		di_ch[i] = ( ( digital_input_value >> i ) & 1 );
	//********************************************************************
}

void AIO_sample( I32 Board_ID )
{
	I32 Channel_No = 0;
	F64 analog_input_volt;
	F64 analog_output_volt;
	
	APS_read_a_input_value(   Board_ID, Channel_No, &analog_input_volt );

	analog_output_volt = 1.5; // 1.5 volt
	APS_write_a_output_value( Board_ID, Channel_No, analog_output_volt );	
	
	APS_read_a_output_value(  Board_ID, Channel_No, &analog_output_volt );
}



