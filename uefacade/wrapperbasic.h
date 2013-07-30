/*
* The UrbanEye PND solution is a collection of packages based on native C++ classes
* that implement the fundamental algorithms & operations required to satisfy one kind
* of mobile or portable devices with competitive qualities being a leading one.
*
* Copyright (C) 2008 Xiujie Sheng
*
* This library isn't a free software, so you cann't take the sourcecode without author's permit.
* Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
* if you illegally got it. 
*
* NOTE: 
* AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
* LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
*
* For more information, contact:
*
* China University of Geoseciences(Wuhan)
* Graduate School
*
* Xiujie Sheng
* smartpoint@sohu.com
*/
#ifndef _UEFACADE_WRAPPERBASIC_H
#define _UEFACADE_WRAPPERBASIC_H

/// UeFacade namespace 
namespace UeFacade
{
	/*! \enum PNDSchema
	* \brief Maybe it only need to change the schema definition 
	* not to change all we could get the different product 
	* implementation against different world market demands
	*/
	enum PNDSchema
	{
		PS_Lower = 0,
		PS_Middle,
		PS_Higher,
		PS_Max
	};

	/*! 
	* \enum PackageDefinition
	* \brief 各模块的标识
	*/
	enum PackageDefinition
	{
		PD_Unknown = -1,
		PD_Base,
		PD_Index,
		PD_Network,
		PD_Gps,
		PD_Plan,
		PD_Query,
		PD_Map,
		PD_Gui,
		PD_Facade,
		PD_Max
	};

	/*!
	* \enum FacadeErrorCode
	*/
	enum FacadeErrorCode
	{
		/// Zero means success
		FEC_Success                     = 0,

		/// Error categories
		FEC_SchemaError       = 0x01000000,

		/// Schema errors
		FEC_InvalidIndex        = 0x01000001,
		FEC_InvalidNetwork      = 0x01000002,
		FEC_InvalidGps          = 0x01000003,
		FEC_InvalidRoute        = 0x01000004,
		FEC_InvalidQuery        = 0x01000005,
		FEC_InvalidMap          = 0x01000006,
		FEC_InvalidRes          = 0x01000007,
		FEC_InvalidGui          = 0x01000008,
		FEC_InvalidVersion        = 0x01000009,
	};
}

#endif