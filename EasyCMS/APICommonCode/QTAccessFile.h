/*
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2008 Apple Inc.  All Rights Reserved.
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 *
 */
 /*
	 Copyleft (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	 Github: https://github.com/EasyDarwin
	 WEChat: EasyDarwin
	 Website: http://www.easydarwin.org
 */
 /*
	 File:       QTAccessFile.h

	 Contains:   This object contains an interface for finding and parsing qtaccess files.


 */
#ifndef _QT_ACCESS_FILE_H_
#define _QT_ACCESS_FILE_H_

#include <stdlib.h>
#include "SafeStdLib.h"
#include "QTSS.h"
#include "StrPtrLen.h"
#include "OSHeaders.h"
#include "StringParser.h"
#include "OSMutex.h"

class QTAccessFile
{
public:
	static UInt8 sWhitespaceAndGreaterThanMask[];
	static void Initialize();

	// GetGroupsArrayCopy allocates outGroupCharPtrArray. Caller must "delete [] outGroupCharPtrArray" when done.
	static char*  GetAccessFile_Copy(const char* movieRootDir, const char* dirPath);

	//over ride these in a sub class
	virtual bool HaveUser(char *userName, void* extraDataPtr);
	virtual bool HaveGroups(char** groupArray, UInt32 numGroups, void* extraDataPtr);
	virtual bool HaveRealm(char* userName, StrPtrLen* ioRealmNameStr, void* extraData);
	virtual bool TestUser(StrPtrLen* accessUser, char* userName, void* extraDataPtr);
	virtual bool TestGroup(StrPtrLen* accessGroup, char* userName, char** groupArray, UInt32 numGroups, void* extraDataPtr);
	virtual bool TestExtraData(StrPtrLen* wordPtr, StringParser* lineParserPtr, void* extraDataPtr);
	virtual void   GetRealm(StrPtrLen* accessRealm, StrPtrLen* ioRealmNameStr, char* userName, void* extraDataPtr);
	virtual bool ValidUser(char* userName, void* extraDataPtr) { return false; };

	static void SetAccessFileName(const char* inQTAccessFileName); //makes a copy and stores it
	static char* GetAccessFileName() { return sQTAccessFileName; }; // a reference. Don't delete!

	virtual ~QTAccessFile() {};

private:
	static char* sQTAccessFileName; // managed by the QTAccess module
	static bool sAllocatedName;
	static OSMutex* sAccessFileMutex;
	static char* sAccessValidUser;
	static char* sAccessAnyUser;

};

class DSAccessFile : public QTAccessFile
{
public:
	virtual   ~DSAccessFile() {}
	virtual bool HaveGroups(char** groupArray, UInt32 numGroups, void* extraDataPtr) { return true; }
	virtual bool TestGroup(StrPtrLen* accessGroup, char* userName, char** groupArray, UInt32 numGroups, void* extraDataPtr)
	{
		StrPtrLenDel deleter(accessGroup->GetAsCString());
		return this->CheckGroupMembership(userName, deleter.Ptr);
	}
	virtual bool ValidUser(char* userName, void* extraDataPtr);
	bool CheckGroupMembership(const char* inUsername, const char* inGroupName);

};


#endif //_QT_ACCESS_FILE_H_

