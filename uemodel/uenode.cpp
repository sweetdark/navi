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
// Refer to UeModel package
#include "uenode.h"
using namespace UeModel;

// Refer to DBG useful macroes
#include "uebase\dbgmacro.h"

// Initialization
const short CUeNode::m_clinkCount[UNT_Total] = {1,2,3,4,0,	1,2,3,4,0,	1,2,3,4,0, 1,2,3,4,0};
const bool  CUeNode::m_adjFlag[UNT_Total] = {1,1,1,1,1,	0,0,0,0,0,	0,0,0,0,0, 0,0,0,0,0};
const bool  CUeNode::m_upFlag[UNT_Total] = {0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0};
const bool  CUeNode::m_mainFlag[UNT_Total] = {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1};

const short CUeSimpleNode::m_nodeBasicSize[UNT_Total] =
{
  sizeof(UeSimpleNodeOne),
  sizeof(UeSimpleNodeTwo),
  sizeof(UeSimpleNodeThree),
  sizeof(UeSimpleNodeFour),
  sizeof(UeSimpleNodeFive),

  sizeof(UeSimpleNodeOne),
  sizeof(UeSimpleNodeTwo),
  sizeof(UeSimpleNodeThree),
  sizeof(UeSimpleNodeFour),
  sizeof(UeSimpleNodeFive),

  sizeof(UeSimpleNodeOne),
  sizeof(UeSimpleNodeTwo),
  sizeof(UeSimpleNodeThree),
  sizeof(UeSimpleNodeFour),
  sizeof(UeSimpleNodeFive),

  sizeof(UeSimpleNodeOne),
  sizeof(UeSimpleNodeTwo),
  sizeof(UeSimpleNodeThree),
  sizeof(UeSimpleNodeFour),
  sizeof(UeSimpleNodeFive),
};

const short CUeSimpleNode::m_nodeTotalSize[UNT_Total] = 
{
  sizeof(UeSimpleNodeOne) + sizeof(UeAdjacentNode),
  sizeof(UeSimpleNodeTwo) + sizeof(UeAdjacentNode),
  sizeof(UeSimpleNodeThree) + sizeof(UeAdjacentNode),
  sizeof(UeSimpleNodeFour) + sizeof(UeAdjacentNode),
  sizeof(UeSimpleNodeFive) + sizeof(UeAdjacentNode),

  sizeof(UeSimpleNodeOne) + sizeof(UeUpNode),
  sizeof(UeSimpleNodeTwo) + sizeof(UeUpNode),
  sizeof(UeSimpleNodeThree) + sizeof(UeUpNode),
  sizeof(UeSimpleNodeFour) + sizeof(UeUpNode),
  sizeof(UeSimpleNodeFive) + sizeof(UeUpNode),

  sizeof(UeSimpleNodeOne),
  sizeof(UeSimpleNodeTwo),
  sizeof(UeSimpleNodeThree),
  sizeof(UeSimpleNodeFour),
  sizeof(UeSimpleNodeFive),

  sizeof(UeSimpleNodeOne) + sizeof(UeMainNode),
  sizeof(UeSimpleNodeTwo) + sizeof(UeMainNode),
  sizeof(UeSimpleNodeThree) + sizeof(UeMainNode),
  sizeof(UeSimpleNodeFour) + sizeof(UeMainNode),
  sizeof(UeSimpleNodeFive) + sizeof(UeMainNode),
};

const short CUeCompositedNode::m_nodeBasicSize[UNT_Total] =
{
  sizeof(UeCompositedNodeOne),
  sizeof(UeCompositedNodeTwo),
  sizeof(UeCompositedNodeThree),
  sizeof(UeCompositedNodeFour),
  sizeof(UeCompositedNodeFive),

  sizeof(UeCompositedNodeOne),
  sizeof(UeCompositedNodeTwo),
  sizeof(UeCompositedNodeThree),
  sizeof(UeCompositedNodeFour),
  sizeof(UeCompositedNodeFive),

  sizeof(UeCompositedNodeOne),
  sizeof(UeCompositedNodeTwo),
  sizeof(UeCompositedNodeThree),
  sizeof(UeCompositedNodeFour),
  sizeof(UeCompositedNodeFive),

  sizeof(UeCompositedNodeOne),
  sizeof(UeCompositedNodeTwo),
  sizeof(UeCompositedNodeThree),
  sizeof(UeCompositedNodeFour),
  sizeof(UeCompositedNodeFive),
};

const short CUeCompositedNode::m_nodeTotalSize[UNT_Total] = 
{
  sizeof(UeCompositedNodeOne) + sizeof(UeAdjacentNode),
  sizeof(UeCompositedNodeTwo) + sizeof(UeAdjacentNode),
  sizeof(UeCompositedNodeThree) + sizeof(UeAdjacentNode),
  sizeof(UeCompositedNodeFour) + sizeof(UeAdjacentNode),
  sizeof(UeCompositedNodeFive) + sizeof(UeAdjacentNode),

  sizeof(UeCompositedNodeOne) + sizeof(UeUpNode),
  sizeof(UeCompositedNodeTwo) + sizeof(UeUpNode),
  sizeof(UeCompositedNodeThree) + sizeof(UeUpNode),
  sizeof(UeCompositedNodeFour) + sizeof(UeUpNode),
  sizeof(UeCompositedNodeFive) + sizeof(UeUpNode),

  sizeof(UeCompositedNodeOne),
  sizeof(UeCompositedNodeTwo),
  sizeof(UeCompositedNodeThree),
  sizeof(UeCompositedNodeFour),
  sizeof(UeCompositedNodeFive),

  sizeof(UeCompositedNodeOne) + sizeof(UeMainNode),
  sizeof(UeCompositedNodeTwo) + sizeof(UeMainNode),
  sizeof(UeCompositedNodeThree) + sizeof(UeMainNode),
  sizeof(UeCompositedNodeFour) + sizeof(UeMainNode),
  sizeof(UeCompositedNodeFive) + sizeof(UeMainNode),
};

//
//
//
/**
*
*/
CUeNode::CUeNode(short type, void *nodeData) : m_parcelType(type), m_nodeData(nodeData)
{
}

/**
*
*/
CUeNode::~CUeNode()
{
  // Note: Don't forget this memory is conducted by parcel not this node
  assert(m_nodeData);
  m_nodeData = 0;
}

/**
*
*/
void CUeNode::Delete()
{
  //delete this;
}
