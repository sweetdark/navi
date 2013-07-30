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
#ifndef _UEQUERY_QUICKWORDINDEX_H
#define _UEQUERY_QUICKWORDINDEX_H
//////////////////////////////////////////////////////////////////////////
// Refer to its parent
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif
//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class UEQUERY_CLASS CQuickWordIndex : public CUeIndex
	{
		friend class CQueryDataVerify;
	public:
		CQuickWordIndex(void);
		//打开文件
		virtual unsigned Open(void);
		virtual unsigned Close(void);
		virtual unsigned Predicate(const SQLSentence &sqlSentence);
		virtual unsigned Next(CMemVector &idxes, unsigned maxNumber = 10000);
		//
		virtual bool ForCollegues(void) {return false;};
	protected:
		/*!
		* Using current administrative code to get iterative idx
		**/
		virtual unsigned GetIdxes(void);
		virtual void InitIndexData(void) {};
		virtual tstring getIndexFileName(void) const
		{
			return _T("quickwords.mj");
			//return _T("poinameterm.mj");
		}
	private:
		// Bytes for one character in this index 
		const static short m_bytesPerVacable = 4;
		// Maximum character number in word library
		const static unsigned short m_maxWords = 0xFFFF;
		// Maximum length of query string supported by this index
		const static short m_maxQueryLength = 20;
		// Maximum TIDs for one word specified by one city
		const static int m_maxGrpTids = 60000;
		// Maximum TIDs in each block divied from above maximum TIDs
		const static int m_maxBlockTids = 250;
		//
#pragma pack(1)
		/*!
		* Unif of word lib in this index file
		* TODO:
		* Consider the large size of word library part in current index file, its side effect is slower performance of I/O
		**/
		struct QuickCharacter
		{
			QuickCharacter(void):m_offset(0)
			{
				::memset(m_bytes, 0x00, m_bytesPerVacable+1);
			}
			// How many bytes for one character
			char m_bytes[m_bytesPerVacable+1];
			// Offset to cities，文件偏移到m_offset处按市级区域编码
			//从小到大对应各个市级区域中，该m_bytes单字或词在整个文件
			//中的偏移，通过这个值可以定位文件，而获取该单字或词在某一
			//区域的详细情况
			unsigned int m_offset;
		};
		/*!
		* Administritive part of this index file for provinces
		**/
		struct QuickProvince
		{
			QuickProvince(void) : m_code(0), m_cities(0), m_offset(0)
			{
			}
			// Province code
			unsigned short m_code;
			// Cites belonging current province
			unsigned short m_cities;
			// Relatvie offset corresponding to CITY part of current index file
			unsigned short m_offset;
		};
#pragma pack()
		/*!
		* Word extracted from current query string
		**/
		struct QuickExtractedWord
		{
			QuickExtractedWord(void):m_offset(-1),m_occurances(0),
				m_tids(0),m_dataSize(0),m_dataBuffer(0)
			{
				::memset(m_position, 0x00, sizeof(unsigned short) * m_maxQueryLength);
			}
			static bool IsMatched(void)
			{
				assert(::strlen(m_intersectedBuffer) && ::strlen(m_extractedBuffer));
				return ::strstr(m_intersectedBuffer, m_extractedBuffer) != 0;
			}
			// Offset corresponding to word library part of current index file
			unsigned int m_offset;
			// Occurances of this word in current query string
			unsigned char m_occurances;
			// Positions of one occurance in current query string
			unsigned char m_position[m_maxQueryLength];
			// Number of offsets of main POI body file corresponding to current word just extracted
			unsigned int m_tids;
			// Data size for storing TIDs corresponding to current word
			unsigned int m_dataSize;
			// Data buffer for storing TIDs corresponding to current word
			char *m_dataBuffer;
			// Buffer respectively for storing extracted words and those intersected words
			static char m_extractedBuffer[m_maxQueryLength+1];
			static char m_intersectedBuffer[m_maxQueryLength+1];
		};
		/*!
		* The intersection row represented by below structure and column represented by extracted words defer the decisions
		* about whether to get matched TID, namely, body offset
		*MIN_TID
		**/
		struct QuickResultRow
		{
			QuickResultRow(void):m_next(-1)
			{
				::memset(m_tid, 0x00, 3);
			}
			// TID corresponding to one extracted word or vacable
			unsigned char m_tid[3];
			// Maybe skip some tids not intersected by different extracted word or vacable
			unsigned short m_next;
		};
		/*!
		* The intersection row represented by below structure and column represented by extracted words defer the decisions
		* about whether to get matched TID, namely, body offset
		* TID_UNIT
		**/
		struct QuickResultCell
		{
			QuickResultCell(void):m_occurances(0)
			{
				::memset(m_offset, 0x00, 3);
			}
			// Occurances of current word specified by one tid
			//单字或词出现的次数
			unsigned char m_occurances;
			// Offset of different positions of current word specified by one tid
			//单字或词出现在词中的位置，在数据(QuickExtractedWord::m_dataBuffer)中的偏移量
			unsigned char m_offset[3];
		};
		/*!
		* Extract words from current input query string
		**/
		void ExtractWords(void);
		/*!
		* Set up mapping between one character in word library and its offset corresponding
		**/
		void SetWordMapping(void);
		/*!
		* Given default algorithm to find words and valcables
		* Note:
		* It should be simplified more and more
		**/
		void DoModeExtraction(void);
		/*!
		* Verify the quality of query string in order to later correct finding actions
		* /brief 去除关键字中的中文符号，及将中文A~Z,a~Z,0~9转换为ASCII码
		**/
		void DoVerifyQueryString(void);
		/*!
		*
		**/
		void AddExtractedWord(int offset, int pos);
		/*!
		* Get offset of current word or vacable in word library of this index file
		*
		* \para: oneWord means one single word or vacable extracted from current query string
		* \para: from posistion corresponding to one word
		* \para: to position corresponding to one word
		* \return: offset to this word or vacable
		**/
		int GetWordOffset(char *oneWord, int from, int to);
		/*!
		* Read group buffer
		**/
		bool GetTIDs(QuickExtractedWord &curWord, int cityOffset);
		//
		int GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart);
		/*!
		*
		**/
		void GetLastResultCells(const QuickExtractedWord &curWord, const QuickResultCell &curCell);
		/*!
		* Get minimum TID row
		**/
		void GetInitialResultCells(const QuickExtractedWord &curWord, QuickResultCell *cells, QuickResultRow *rows, int &minCount);
		/*!
		* 
		**/
		void GetIntersectedResultCells(unsigned short *rowHead, const QuickExtractedWord &curWord, QuickResultCell *cells, QuickResultRow *rows);
		/*!
		*
		**/
		void DoIntersect(const QuickExtractedWord &curWord, unsigned short *rowHead, QuickResultRow *rows, QuickResultCell *cells, char *cursor, unsigned int startPos, unsigned char blocks, bool isLastGrp, unsigned short &curRec, unsigned short &lastRec, unsigned int &grpSize);
		/*!
		*
		**/
		static bool CompareExtractedWord(const QuickExtractedWord *first, const QuickExtractedWord *seond);
		//
#ifndef USE_NEW_DATA
		// City description
		unsigned short *m_cities;
#else
		// City description
		unsigned *m_cities;
#endif
		// Word lib descritpion and their offset mapping between one character and library part of this index file
		QuickCharacter *m_characters;

		// Note: Here currently is a limitation when the size of word library is enlarged and including more vocubals
		unsigned short m_charFrom[m_maxWords + 1];
		unsigned short m_charTo[m_maxWords + 1];

		typedef std::vector<QuickExtractedWord*> ExtractedWordVector;
		// Extracted words given current query string
		ExtractedWordVector m_extractedWords;
	};
}
#endif