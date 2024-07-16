
TYPE
	CanReadWrite_typ : 	STRUCT 
		step : CanReadWrite_Steps := STEP_INIT;
		canDevice : STRING[80];
		writeData : ARRAY[0..1]OF UDINT;
		readData : ARRAY[0..1]OF UDINT;
		CANopen : CANopen;
		CANread : CANread;
		CANwrite : CANwrite;
		CANinfo : CANinfo;
		CANTime : CANTime;
		successCount : UDINT;
		errorCount : UINT;
		canError : UINT;
		canInfo : INFO_typ;
	END_STRUCT;
	CanReadWrite_Steps : 
		(
		STEP_ERROR := 255,
		STEP_INIT := 1,
		STEP_PREPARE_READ := 2,
		STEP_WRITE := 3,
		STEP_READ := 4
		);
END_TYPE
