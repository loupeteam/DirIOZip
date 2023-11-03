/*
 * File: DataObjToFolder.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of DirIOZip, licensed under the MIT License.
 */
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "DirIOZip.h"
	#include <string.h>

#ifdef __cplusplus
	};
#endif
void DataObjToFolder(struct DataObjToFolder* inst)
{

	switch (inst->internal.ConvertToFile.Status)
	{		
		case ERR_FUB_ENABLE_FALSE:	
			if( inst->Execute 
				&& !inst->internal._Execute 
				&& !inst->internal._DeleteTemp
				&& !inst->internal._Active
			){
				inst->internal._Execute=		1;
				inst->internal._DeleteTemp=		1;
				inst->internal._Active=			1;

				strcpy(inst->internal.ConvertToFile.DataObjectName,	inst->DataObjectName);
				strcpy(inst->internal.ConvertToFile.FileDevice,		inst->TempFileDevice);
				//				strcpy(inst->internal.ConvertToFile.FileName,);
				inst->internal.ConvertToFile.Execute=	1;
				
				inst->internal.Unzip.pArchiveDevice=	(UDINT)&inst->TempFileDevice;
				inst->internal.Unzip.pOutDevice=		(UDINT)&inst->FileDevice;
				inst->internal.Unzip.pArchiveFile=		(UDINT)&inst->internal.TempArchiveName;
				inst->internal.Unzip.pOutFolder=		(UDINT)&inst->DestPath;					

				inst->internal.DeleteFile.pDevice=	(UDINT)&inst->TempFileDevice;
				inst->internal.DeleteFile.pName=	(UDINT)&inst->internal.TempArchiveName;
				
				
				inst->Status=	ERR_FUB_BUSY;
			}	
			else if(!inst->Execute){
				inst->Status=	ERR_FUB_ENABLE_FALSE;			
			}
			break;

		case ERR_FUB_BUSY:		
			//Use this status
			inst->Status=	inst->internal.ConvertToFile.Status;
			break;
		case ERR_OK:
			inst->internal.ConvertToFile.Execute=		0;			

			if(strstr(inst->internal.ConvertToFile.ExportFileName,".tar.gz")){
				strcpy(inst->internal.TempArchiveName, inst->internal.ConvertToFile.ExportFileName);			
				inst->internal.Unzip.enable=	1;		
			}
			else{
				inst->Status=					ERR_OK;
				inst->internal._DeleteTemp=		0;				
				inst->internal._Active=			0;
			}	
			
			break;
		case fiERR_EXIST:
			//Any other status should grab the status and disable the function block
			inst->Status=	inst->internal.ConvertToFile.Status;
			inst->internal.ConvertToFile.Execute=		0;
			inst->internal._Active=			0;			
			inst->internal._DeleteTemp=		0;			
				
			break;
		default:
			//Any other status should grab the status and disable the function block
			inst->Status=	inst->internal.ConvertToFile.Status;
			inst->internal.ConvertToFile.Execute=		0;
			inst->internal._Active=			0;			
			break;
	}

	switch (inst->internal.Unzip.status)
	{		
		case ERR_FUB_BUSY:		
			//Use this status
			inst->Status=	inst->internal.Unzip.status;
			break;
		case ERR_OK:			
			inst->internal._Active=		0;
			inst->internal.Unzip.enable=0;		
			inst->internal._Finishing=	1;	
			break;
		case ERR_FUB_ENABLE_FALSE:
			
			break;	
		default:
			//Any other status should grab the status and disable both function blocks
			inst->Status=	inst->internal.Unzip.status;
			inst->internal.Unzip.enable=		0;
			inst->internal._Active=				0;			
			break;
	}
	
	if(inst->internal._DeleteTemp && !inst->internal._Active){
		inst->internal.DeleteFile.enable=	1;		
	}
	
	zipExtract(&inst->internal.Unzip);
	DataObjToFile(&inst->internal.ConvertToFile);

	FileDelete(&inst->internal.DeleteFile);	
	if(inst->internal.DeleteFile.status != ERR_FUB_ENABLE_FALSE
		&& inst->internal.DeleteFile.status != ERR_FUB_BUSY
	){
		if(inst->internal._Finishing){
			inst->Status= ERR_OK;
		}
		inst->internal._DeleteTemp=	0;		
		inst->internal.DeleteFile.enable= 0;
		inst->internal._Finishing=	0;
	}
	
	if( !inst->Execute){
		inst->internal._Execute=					0;
	}
	
}
