(*
 * File: DirIOZip.fun
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of DirIOZip, licensed under the MIT License.
 *)


FUNCTION_BLOCK FolderToDataObj (* *) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		FileDevice : STRING[80];
		TempFileDevice : STRING[80];
		FileName : STRING[80];
		DataObjectName : STRING[10];
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : FolderToDataObject_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK DataObjToFolder (*Convert a file to a dataobject*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Execute : BOOL;
		FileDevice : STRING[80];
		FileName : STRING[80];
		DataObjectName : STRING[10];
		DestPath : STRING[80];
		TempFileDevice : STRING[80];
	END_VAR
	VAR_OUTPUT
		Status : UINT;
	END_VAR
	VAR
		internal : DataObjectToFolder_typ;
	END_VAR
END_FUNCTION_BLOCK
