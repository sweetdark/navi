// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in irrlicht.h and/or irrXML.h

#include "irrXML.h"
#include "irrString.h"
#include "irrArray.h"
#include "fast_atof.h"
#include "CXMLReaderImpl.h"

namespace irr
{
namespace io
{

// FACTORY FUNCTIONS:


//! Creates an instance of an UFT-8 or ASCII character xml parser. 
IrrXMLReader* createIrrXMLReader(const char* filename)
{
	return new CXMLReaderImpl<char, IXMLBase>(new CFileReadCallBack(filename)); 
}


//! Creates an instance of an UFT-8 or ASCII character xml parser. 
IrrXMLReader* createIrrXMLReader(FILE* file)
{
	return new CXMLReaderImpl<char, IXMLBase>(new CFileReadCallBack(file)); 
}


//! Creates an instance of an UFT-8 or ASCII character xml parser. 
IrrXMLReader* createIrrXMLReader(IFileReadCallBack* callback)
{
	return new CXMLReaderImpl<char, IXMLBase>(callback, false); 
}


//! Creates an instance of an UTF-16 xml parser. 
IrrXMLReaderUTF16* createIrrXMLReaderUTF16(const char* filename)
{
	return new CXMLReaderImpl<char16, IXMLBase>(new CFileReadCallBack(filename)); 
}


//! Creates an instance of an UTF-16 xml parser. 
IrrXMLReaderUTF16* createIrrXMLReaderUTF16(FILE* file)
{
	return new CXMLReaderImpl<char16, IXMLBase>(new CFileReadCallBack(file)); 
}


//! Creates an instance of an UTF-16 xml parser. 
IrrXMLReaderUTF16* createIrrXMLReaderUTF16(IFileReadCallBack* callback)
{
	return new CXMLReaderImpl<char16, IXMLBase>(callback, false); 
}


//! Creates an instance of an UTF-32 xml parser. 
IrrXMLReaderUTF32* createIrrXMLReaderUTF32(const char* filename)
{
	return new CXMLReaderImpl<char32, IXMLBase>(new CFileReadCallBack(filename)); 
}


//! Creates an instance of an UTF-32 xml parser. 
IrrXMLReaderUTF32* createIrrXMLReaderUTF32(FILE* file)
{
	return new CXMLReaderImpl<char32, IXMLBase>(new CFileReadCallBack(file)); 
}


//! Creates an instance of an UTF-32 xml parser. 
IrrXMLReaderUTF32* createIrrXMLReaderUTF32(IFileReadCallBack* callback)
{
	return new CXMLReaderImpl<char32, IXMLBase>(callback, false); 
}


} // end namespace io
} // end namespace irr
