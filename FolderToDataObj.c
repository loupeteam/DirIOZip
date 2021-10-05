
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
void FolderToDataObj(struct FolderToDataObj* inst)
{

	switch (inst->internal.Zip.status)
	{		
		case ERR_FUB_ENABLE_FALSE:	
			if( inst->Execute 
				&& !inst->internal._Execute 
				&& !inst->internal._DeleteTemp
				&& !inst->internal._Active
			){
				inst->internal._Execute=		1;
				
				if( ValidDirectoryName((UDINT)inst->FileName) ){
					inst->internal._DeleteTemp=		1;				
					inst->internal._Active=			1;
					inst->internal.Zip.enable=		1;
					inst->Status=	ERR_FUB_BUSY;
				}
				else{
					inst->Status = fiERR_INVALID_DIRECTORY;
				}
				inst->internal.Zip.pArchiveDevice=	(UDINT)&inst->TempFileDevice;
				inst->internal.Zip.pSrcDevice=		(UDINT)&inst->FileDevice;
				inst->internal.Zip.pSrcFile=		(UDINT)&inst->FileName;

				strcpy(inst->internal.TempArchiveName,inst->FileName);
				strcat(inst->internal.TempArchiveName,".tar.gz");
				inst->internal.Zip.pArchiveFile=	(UDINT)&inst->internal.TempArchiveName;

				inst->internal.DeleteFile.pDevice=	(UDINT)&inst->TempFileDevice;
				inst->internal.DeleteFile.pName=	(UDINT)&inst->internal.TempArchiveName;
				
				strcpy(inst->internal.ConvertToDataObject.DataObjectName,	inst->DataObjectName);
				strcpy(inst->internal.ConvertToDataObject.FileDevice,		inst->TempFileDevice);
				strcpy(inst->internal.ConvertToDataObject.FileName,			inst->internal.TempArchiveName);
				

			}	
			else if(!inst->Execute){
				inst->Status=	ERR_FUB_ENABLE_FALSE;			
			}
			break;

		case ERR_FUB_BUSY:		
			//Use this status
			inst->Status=	inst->internal.Zip.status;
			break;
		case ERR_OK:
			inst->internal.Zip.enable=					0;
			inst->internal.ConvertToDataObject.Execute=	1;			
			break;
		default:
			//Any other status should grab the status and disable the function block
			inst->Status=	inst->internal.Zip.status;
			inst->internal.Zip.enable=		0;
			inst->internal._Active=			0;			
			break;
	}

	switch (inst->internal.ConvertToDataObject.Status)
	{		
		case ERR_FUB_BUSY:		
			//Use this status
			inst->Status=	inst->internal.ConvertToDataObject.Status;
			break;
		case ERR_OK:			
			inst->internal._Active=		0;
			if(inst->internal._DeleteTemp){

			}
			else{
				inst->internal.ConvertToDataObject.Execute=		0;		
				inst->Status=	ERR_OK;	
			}	
			break;
		case ERR_FUB_ENABLE_FALSE:

			break;	
		default:
			//Any other status should grab the status and disable both function blocks
			inst->Status=	inst->internal.ConvertToDataObject.Status;
			inst->internal.ConvertToDataObject.Execute=		0;
			inst->internal._Active=			0;			
			break;
	}
	
	if(inst->internal._DeleteTemp && !inst->internal._Active){
		inst->internal.DeleteFile.enable=	1;		
	}
	
	zipArchive(&inst->internal.Zip);
	FileToDataObj(&inst->internal.ConvertToDataObject);

	FileDelete(&inst->internal.DeleteFile);	
	if(inst->internal.DeleteFile.status != ERR_FUB_ENABLE_FALSE
	&& inst->internal.DeleteFile.status != ERR_FUB_BUSY
	){
		inst->internal._DeleteTemp=	0;		
		inst->internal.DeleteFile.enable= 0;
	}

	
	if( !inst->Execute){
		inst->internal._Execute=					0;
	}

	
	
}
