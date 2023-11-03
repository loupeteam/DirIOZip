(*
 * File: Types.typ
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of DirIOZip, licensed under the MIT License.
 *)

TYPE
	DataObjectToFolder_typ : 	STRUCT 
		ConvertToFile : DataObjToFile;
		Unzip : zipExtract;
		DeleteFile : FileDelete; (*Open a file and read its contents*)
		_Execute : BOOL;
		_DeleteTemp : BOOL;
		_Active : BOOL;
		_Finishing : BOOL;
		TempArchiveName : STRING[80];
	END_STRUCT;
	FolderToDataObject_typ : 	STRUCT 
		Zip : zipArchive;
		ConvertToDataObject : FileToDataObj;
		DeleteFile : FileDelete; (*Open a file and read its contents*)
		_Execute : BOOL;
		_DeleteTemp : BOOL;
		_Active : BOOL;
		TempArchiveName : STRING[80];
	END_STRUCT;
END_TYPE
