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
// Refer to UeFacade
#include "uegarbage.h"
using namespace UeFacade;

///////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
CUeGarbage::CUeGarbage()
{
}

/**
*
**/
CUeGarbage::~CUeGarbage()
{
	// Note:
	// Here no need to release instances in this map
	m_consumers.clear();
}

////////////////////////////////////////////////////////////////////////////////
//
/**
* 1) It would be triggered by one collegue indicatored by type if it face the little memory, generally it will
* meet with this occasion by catching one exception
* 2) Where can be got more memory? From itself and with send WM_HIBERNATE to let other applications to get
* 3) With compacting memory fragmentation to get more memory. From CE mobile 5.0 or higher version, HeapCompact maybe play
* great role
**/
bool CUeGarbage::NotifyCollegues(short type, CMemoryConsumer *curConsumer)
{
	if(type == CMemoryConsumer::CT_Unknown)
	{
		consumer_itr first = m_consumers.begin();
		consumer_itr end = m_consumers.end();
		//
		for(; first != end; first++)
		{
			//
			ConsumerVector &curVector = first->second;
			int count = static_cast<int>(curVector.size());
			int i = 0;
			for(; i < count; i++)
			{
				if(curVector[i])
				{
					curVector[i]->ForCollegues();
				}
			}
		}
	}
	else
	{
		// Try to let other collegues release memory currently not being be used
		consumer_itr cur = m_consumers.find(type);
		consumer_itr first = m_consumers.begin();
		consumer_itr end = m_consumers.end();

		//
		for(; first != end; first++)
		{
			//
			ConsumerVector &curVector = first->second;
			int count = static_cast<int>(curVector.size());
			int i = 0;

			//
			if((*first).first == (*cur).first)
			{
				if(!curConsumer)
				{
					continue;
				}

				for(; i < count; i++)
				{
					if(curVector[i] && curVector[i] != curConsumer)
					{
						curVector[i]->ForCollegues();
					}
				}
			}
			else
			{
				for(; i < count; i++)
				{
					if(curVector[i])
					{
						curVector[i]->ForCollegues();
					}
				}
			}
		}
	}

#ifdef _WIN32_WCE
	::SendMessage(NULL, WM_HIBERNATE, NULL, NULL);
#endif
	return true;
}

/**
*
**/
bool CUeGarbage::RegisterConsumer(short type, CMemoryConsumer *oneCollegue)
{
	// Whether already existing
	if(m_consumers.find(type) == m_consumers.end())
	{
		//
		std::pair<consumer_itr, bool> rt;
		ConsumerVector oneVector;
		oneVector.push_back(oneCollegue);
		rt = m_consumers.insert(ConsumerMap::value_type(type, oneVector));
	    
		return rt.second;
	}
	else
	{
		//
		ConsumerVector &curVector = m_consumers[type];
		int count = static_cast<int>(curVector.size());
		int i = 0;
		for(; i < count; i++)
		{
			if(curVector[i] == oneCollegue)
			{
				return false;
			}
		}

		curVector.push_back(oneCollegue);
		return true;
	}

	return false;
}
