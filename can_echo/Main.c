#include <bur/plctypes.h>
#include <stdio.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#define CAN_WRITE_ID	0x013
#define CAN_READ_ID 0x123

void _INIT ProgramInit(void)
{
	canReadWrite.CANopen.enable = 1;
	canReadWrite.CANopen.device = (UDINT)"IF7 /BD=250000 /EX=0";
	canReadWrite.CANopen.cob_anz = 200;
	canReadWrite.CANopen.error_adr = canReadWrite.canError;
	CANopen (&canReadWrite.CANopen);
	
	canReadWrite.step = STEP_INIT;
}

void _CYCLIC ProgramCyclic(void)
{
	CANinfo (&canReadWrite.CANinfo);
	
	switch (canReadWrite.step)
	{
		case STEP_INIT:
			if (canReadWrite.CANopen.status == 0)
			{
				canReadWrite.CANinfo.enable = 1;
				canReadWrite.CANinfo.info_adr = (UDINT)&canReadWrite.canInfo;
				canReadWrite.CANinfo.us_ident = canReadWrite.CANopen.us_ident;
				canReadWrite.step = STEP_PREPARE_READ;
			}
			
			break;
		
		case STEP_PREPARE_READ:
						
			canReadWrite.CANread.enable = 1;
			canReadWrite.CANread.us_ident = canReadWrite.CANopen.us_ident;
			canReadWrite.CANread.can_id = CAN_READ_ID;
			canReadWrite.CANread.data_adr = (UDINT)canReadWrite.readData;
			
			canReadWrite.step = STEP_READ;
	
			break;
		
		case STEP_WRITE:
			
			canReadWrite.CANwrite.enable = 1;
			canReadWrite.CANwrite.us_ident = canReadWrite.CANopen.us_ident;
			canReadWrite.CANwrite.can_id = CAN_WRITE_ID;
			canReadWrite.CANwrite.data_adr = (UDINT)canReadWrite.writeData;
			canReadWrite.CANwrite.data_lng = sizeof (canReadWrite.writeData);
			
			CANwrite (&canReadWrite.CANwrite);
			if (canReadWrite.CANwrite.status == 0)
			{
				canReadWrite.step = STEP_READ;
			}
			else if (canReadWrite.CANwrite.status != 1)
			{
				canReadWrite.step = STEP_ERROR;
			}
			
			break;
		
		case STEP_READ:
			CANread (&canReadWrite.CANread);
			if (canReadWrite.CANread.status == 0)
			{
				memcpy ( canReadWrite.writeData, canReadWrite.readData, sizeof (canReadWrite.readData) );
				canReadWrite.step = STEP_WRITE;
			}
			else if (canReadWrite.CANread.status != canERR_NO_DATA)
			{
				canReadWrite.step = STEP_ERROR;
			}
			
			break;
		
		case STEP_ERROR:
			break;	
		

		
	}
	

}

void _EXIT ProgramExit(void)
{

}

