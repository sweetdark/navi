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
// Refer to UeQuery package
#ifndef _UEQUERY_QUICKWORDINDEX_H
#include "quickwordindex.h"
#endif
// Refer to UeBase package
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
#ifndef _UEQUERY_CHWPELLINGCTRL_H
#include "chwpellingctrl.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
using namespace UeBase;
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	char CQuickWordIndex::QuickExtractedWord::m_extractedBuffer[m_maxQueryLength+1];
	char CQuickWordIndex::QuickExtractedWord::m_intersectedBuffer[m_maxQueryLength+1];
	//
	CQuickWordIndex::CQuickWordIndex(void):m_cities(0),m_characters(0)
	{
		::memset(m_charTo,0xfe,(m_maxWords+1)*sizeof(short));
		::memset(m_charFrom,0xff,(m_maxWords+1)*sizeof(short));
	}
	unsigned int CQuickWordIndex::Open(void)
	{
		if (!m_indexHandle || !m_fileBasic.IsValidHandle(m_indexHandle))
		{
			// Check whether it stay there
			tstring indexFile(CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath));
			indexFile += getIndexFileName();
			//
			if(!m_pathBasic.IsFileExist(indexFile))
			{
				return SQL_NotExistIndex;
			}
			//
			if (!(m_indexHandle=m_fileBasic.OpenFile(indexFile, CFileBasic::UE_FILE_READ))
				|| !m_fileBasic.IsValidHandle(m_indexHandle))
			{
				return SQL_NotExistIndex;
			}
			// Get header information
			void *ptr = 0;
			int count = 1;
#ifndef USE_NEW_DATA
			//获取分词数目
			ptr = &m_bodyCount;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(m_bodyCount),count);
			//获取城市数目
			ptr = &m_idxCount;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(m_idxCount),count);
			//获取省份数目
			unsigned provinces(0);
			ptr = &provinces;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(provinces),count);
			//现在不在需要省份数目，如果要定位到某一个省份时，参照AcronymIndex的做法
			m_fileBasic.SeekFile(m_indexHandle,provinces*sizeof(QuickProvince),CFileBasic::UE_SEEK_CUR);
#else
			//新数据为保持一致性，而添加的头信息
			m_fileBasic.SeekFile(m_indexHandle,16+2,CFileBasic::UE_SEEK_BEGIN);
			//获取区域数目
			ptr = &m_idxCount;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(m_idxCount),count);
			//获取分词数目
			ptr = &m_bodyCount;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(m_bodyCount),count);
#endif
			//分配内存空间
			if (m_cities!=0)
			{
				::free(m_cities);
			}
#ifndef USE_NEW_DATA
			if (0==(m_cities=static_cast<unsigned short *>(::malloc(m_idxCount*sizeof(*m_cities)))))
#else
			if (0==(m_cities=static_cast<unsigned *>(::malloc(m_idxCount*sizeof(*m_cities)))))
#endif
			{
				return SQL_InitIndexFailure;
			}
			//读取数据
			ptr = m_cities;
			count = m_idxCount;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(*m_cities),count);
			//分配内存空间
			if (m_characters!=0)
			{
				::free(m_characters);
			}
			if (0==(m_characters=static_cast<QuickCharacter *>(::malloc(m_bodyCount*sizeof(*m_characters)))))
			{
				::free(m_cities);
				m_cities = 0;
				return SQL_InitIndexFailure;
			}
			//获取分词数据
			ptr = m_characters;
			count = m_bodyCount;
			m_fileBasic.ReadFile(m_indexHandle,&ptr,sizeof(*m_characters),count);

			// Set up word map between encoding of one character and offsets of word library
			SetWordMapping();
		}
		return SQL_Success;
	}
	unsigned CQuickWordIndex::Close(void)
	{
		if (m_cities!=0)
		{
			::free(m_cities);
			m_cities = 0;
		}
		if (m_characters!=0)
		{
			::free(m_characters);
			m_characters = 0;
		}
		if (m_indexHandle!=0)
		{
			m_fileBasic.CloseFile(m_indexHandle);
			m_indexHandle = 0;
		}
		return SQL_Success;
	}
	unsigned int CQuickWordIndex::Predicate(const SQLSentence &sqlSentence)
	{
		// Check SQL sentence, m_uniName represents input query string
		if(sqlSentence.m_acronyms[0] == '\0')
		{
			return SQL_InvalidSentence;
		}
		// Varify current SQL sentence
		// Not opened yet
		if(!m_characters)
		{
			Open();
		}
		assert(m_characters);
		//
		m_sql = sqlSentence;
		// Extract word or vocables 
		ExtractWords();

		// Exception whether one character in current query string can be found in word library part of current index file
		char *cursor = QuickExtractedWord::m_extractedBuffer;
		::memset(cursor, 0x00, m_maxQueryLength+1);
		int i = 0;
		int count = m_extractedWords.size();
		for(; i < count; i++)
		{
			int j = 0;
			QuickExtractedWord *curWord = m_extractedWords[i];
			for(; j < curWord->m_occurances; j++)
			{
				::memcpy(cursor + curWord->m_position[j], m_characters[curWord->m_offset].m_bytes, ::strlen(m_characters[curWord->m_offset].m_bytes));
			}
		}
		if(::strcmp(::_strlwr(cursor), ::_strlwr(m_sql.m_acronyms)))
		{
			return SQL_InvalidSentence;
		}
		// Get administrative indicators for Next(...)
		return GetIdxes();
	}
	unsigned CQuickWordIndex::Next(CMemVector &idxes, unsigned maxNumber)
	{
		// Whether to meet with the tail
		if(m_curIdx == -1 || m_curIdx > m_endIdx)
		{
			return SQL_MeetTail;
		}
		//assert((m_curIdx >= 0) && (m_curIdx < m_header.m_cities));

		// Get tids information for each word just extracted
		// Note:
		// TID means offsets of body entries of main body file and if we decrease memory fragement, the definition of cells and rows should be
		// member variables
		int words = m_extractedWords.size();
		assert(words);
		if(words)
		{
			// Record intersection result between different extracted words and whose TIDs information represented by QuickResultRow
			QuickResultCell **cells = (QuickResultCell **)::malloc(sizeof(QuickResultCell *) * words);
			assert(cells);

			// Loop all extracted words or vacables and read its TID part from current index file
			int i = 0;
			int j = 0;
			for(; i < words; i++)
			{
				// Initialization
				cells[i] = 0;

				// TODO:
				// Here should consider buffer mechanism in order to decrease I/O obviously
				if(!GetTIDs(*m_extractedWords[i], m_curIdx))
				{
					for(;j<i;j++)
					{
						::free(m_extractedWords[i]->m_dataBuffer);
						m_extractedWords[i]->m_dataBuffer = 0;
					}
					::free(cells);
					m_curIdx++;
					return SQL_Success;
				}
			}

			// Note:
			// If we face the large size of below vector, we can use vector of indirect pointer to speed the performance of std::sort algorithm
			// ...
			// Compare those extracted words by its total number corresponding to body entries
			std::sort(m_extractedWords.begin(), m_extractedWords.end(), CompareExtractedWord);
			int minCount = m_extractedWords[0]->m_tids;
			assert(minCount < 0xFFFF);

			//if(minCount >= 0xFFFF)
			//暂时打开
			if (0)
			{
				//数目太大了，不与处理
				// As far, we can't support this since we divide TID group in different blocks and the total size is 0xFFFF
				// The size of each block including those TIDS is 250
				::free(cells);
				for(i = 0 ; i < words ; i++)
				{
					if(m_extractedWords[i]->m_dataBuffer)
					{
						free(m_extractedWords[i]->m_dataBuffer);
						m_extractedWords[i]->m_dataBuffer = 0;
					}
				}
				m_curIdx++;
				return SQL_Success;
			}

			// Implicit single list for TIDs corresponding to one extracted word
			// Note:
			// Here initial result cells means the minimum number of TIDs corresponding to one extracted word or vacable
			// ...
			QuickResultRow *rows = (QuickResultRow *)::malloc(sizeof(QuickResultRow) * minCount);
			assert(rows);
			::memset(rows,0x00,sizeof(QuickResultRow) * minCount);
			cells[0] = (QuickResultCell *)::malloc(sizeof(QuickResultCell) * minCount);
			::memset(cells[0], 0x00, sizeof(QuickResultCell) * minCount);
			GetInitialResultCells(*m_extractedWords[0], cells[0], rows, minCount);
			assert(minCount);

			// Intersection among other extracted words or vacables
			unsigned short headTidOrder = 0;
			if(minCount)
			{
				i = 1;
				for(; i < words; i++)
				{
					cells[i] = (QuickResultCell *)::malloc(sizeof(QuickResultCell) * minCount);
					::memset(cells[i], 0x00, sizeof(QuickResultCell) * minCount);

					// Step by step to make intersection among those extracted words
					GetIntersectedResultCells(&headTidOrder, *m_extractedWords[i], cells[i], rows);
					if(headTidOrder == 0xFFFF)
					{
						break;
					}
				}
			}

			// Get tids
			::memset(QuickExtractedWord::m_intersectedBuffer, 0x00, m_maxQueryLength+1);
			unsigned short tidOrder = headTidOrder;
			while(tidOrder != 0xFFFF)
			{
				// Exception
				// TODO:
				// Clumsy code
				if(idxes.GetCount() > unsigned int(maxNumber))
				{
					break;
				}

				//
				memset(QuickExtractedWord::m_intersectedBuffer, '%', m_maxQueryLength);
				i = 0;
				for(; i < words; i++)
				{
					GetLastResultCells(*m_extractedWords[i], cells[i][tidOrder]);
				}

				if(QuickExtractedWord::IsMatched())
				{
					int tid = 0;
					::memcpy(&tid, rows[tidOrder].m_tid, 3);
					idxes.Add(&tid);
				}
				tidOrder = rows[tidOrder].m_next;
			}

			// Free 
			i = 0;
			for(; i < words; i++)
			{
				if(cells[i])
				{
					::free(cells[i]);
				}

				if(m_extractedWords[i]->m_dataBuffer)
				{
					::free(m_extractedWords[i]->m_dataBuffer);
					m_extractedWords[i]->m_dataBuffer = 0;
				}
			}
			::free(cells);
			::free(rows);
		}

		// Move to next, namely next city part
		m_curIdx++;
		return SQL_Success;
	}
	void CQuickWordIndex::SetWordMapping(void)
	{
		assert(m_characters);

		unsigned short curWord = 0;
		unsigned short prevWord = 0;
		int i = 0;
		for(; i <m_bodyCount; i++)
		{
			::memcpy(&curWord, m_characters[i].m_bytes, 2);
			if(prevWord != curWord)
			{
				// Get the last offset of previous word
				if(prevWord)
				{
					m_charTo[prevWord] = i-1;
				}

				// Move to next character
				m_charFrom[curWord] = i;
				prevWord = curWord;
			}
		}

		if(prevWord)
		{
			m_charTo[prevWord] = i-1;
		}
	}
	/*!
	*
	**/
	void CQuickWordIndex::ExtractWords()
	{
		// Verify the quality of current query string
		// Note:
		// Below is deprecated function since there is no any input way to input invalid character
		//DoVerifyQueryString();
		CChWSpellingCtrl::GetChWSpellCtrl().clearStringSign(m_sql.m_acronyms);

		// Declare and initialization by default
		return DoModeExtraction();
	}

	/*!
	* Deprecated function since there is no possibilities of input of invalid character
	**/
	void CQuickWordIndex::DoVerifyQueryString()
	{
		// Find whether there is an invalid character not contained by CHN or ASCII character set, even including
		// Note:
		// Below not to consider whether there is only the first part of one CHN character in the tail of this query string
		int cur = 0;
		int prev = 0;
		char *cursor = m_sql.m_acronyms;
		while (cursor[cur] !=0)
		{
			if(cursor[cur] == '\r' || 
				cursor[cur] == '\n' || 
				cursor[cur] == ' ')
			{
				cur++;
				continue;
			}

			// Given this is a CHN character set
			unsigned char first = cursor[cur];	
			unsigned char last = cursor[cur + 1];
			cur++;

			// If it is a CHN character
			if(first >= 0x80)
			{	
				// Meet with the tail
				if(last == 0)
				{
					break;
				}

				// 
				cur++;
				//if(cur == 0xA3)
				if (first==0xA3)
				{	
					// Reserver Ascii code in CHN character set
					if(last >= 0xC1 && last <= 0xDA)
					{
						cursor[prev++] = last - 0xC1 + 'A';
						continue;
					}

					if(last >= 0xE1 && last <= 0xFA)
					{
						cursor[prev++] = last - 0xE1 + 'a';
						continue;
					}

					if(last >= 0xB0 && last <= 0xB9)
					{
						cursor[prev++] = last - 0xB0 + '0';
						continue;
					}
				}

				// Kick out special CHN characters
				if (first >= 0xA1 && first <= 0xA9)
				{
					continue;
				}

				cursor[prev++] = first;
				cursor[prev++] = last;
			}
			else
			{	
				// Only for ASCII characters
				if((first >= 'a' && first <= 'z') || 
					(first >= 'A' && first <= 'Z') || 
					(first >= '0' && first <= '9') )
				{
					cursor[prev++] = first;
				}
			}
		}
		cursor[prev] = 0;
	}

	/*!
	*
	**/
	void CQuickWordIndex::DoModeExtraction()
	{

		// Indicator whether to extract vacable not only single word including ASCII code already
		int flag = 0;
		// Word buffer in order to whether extract vacable not only single word
		// Note:
		// As far, we only support vacable in two words, so below is 2.
		// ...
		char words[2][m_bytesPerVacable+1];
		// Offset of the last word in one extracted vacable
		int lastOffset = 0;
		// Flag of the last word in one extracted vacable
		int lastFlag = 0;
		// Prepare word or vacables extracted from current query string
		int i = 0;
		int count = m_extractedWords.size();
		for(; i < count; i++)
		{
			if(m_extractedWords[i])
			{
				if(m_extractedWords[i]->m_dataBuffer)
				{
					::free(m_extractedWords[i]->m_dataBuffer);
					m_extractedWords[i]->m_dataBuffer = 0;
				}
				::free(m_extractedWords[i]);
				m_extractedWords[i] = 0;
			}
		}
		m_extractedWords.clear();

		// According to mode which define we can extract the maximum of one vacable from current query string
		// and find all single word or vacables
		int cur = 0;
		int prev = 0;
		const char *cursor = m_sql.m_acronyms;
		while(1)
		{
			// Meet the tail
			if(cursor[cur] == 0)
			{
				// Whether to find one vacable
				if(flag > 0)
				{
					// Note:
					// Here use m_misses to indicate whether to find vacable which is limited to specified length
					// And even if it meets with the tail it still need to back previous position for new vacable
					// For mode == 1, directly skip the whole vacable, for mode == 1 only skip one word 
					if(m_sql.m_misses == 1)
					{
						cur = prev + ::strlen(words[0]);
					}
					else
					{
						// Directly skip the whole vacable
						AddExtractedWord(lastOffset, prev);
						cur = prev + ::strlen(words[lastFlag]);
					}

					// Prepare for next word or vacable
					prev = cur;
					flag = 0;
					continue;
				}
				else 
				{
					// Since it is only to extract single word, so it ends up
					break;
				}
			}



			// Whether to record one word as part of one vacable
			if(flag > 0)
			{
				::strcpy(words[flag], words[flag-1]);
			}
			else 
			{
				// Initialization for the first time
				words[flag][0]=0;
			}

			// Extract one single word or vacable
			int k = ::strlen(words[flag]);
			words[flag][k++] = cursor[cur++];
			if(cursor[cur] != 0)
			{
				// Get one CHN character
				if(cursor[cur-1] < 0)
				{
					words[flag][k++] = cursor[cur++];
				}
				else if(k==1)
				{
					// Current word is an ASCII character for case conversion
					if(words[flag][0] >= 'A' && words[flag][0] <= 'Z')
					{
						words[flag][0]=words[flag][0] - 'A' + 'a';
					}
				}
			}
			else if(cursor[cur-1] > 0)	// Prevous word is an ASCII character
			{
				if(words[flag][0] >= 'A' && words[flag][0] <= 'Z')
				{
					words[flag][0] = words[flag][0] - 'A' + 'a';
				}
			}
			else
			{
				// Exception character in current query string when facing 
				// ... ...
				continue;
			}

			// To find current word in word library in current index file
			words[flag][k] = 0;
			unsigned short curWord = 0;
			::memcpy(&curWord, &words[flag][0], 2);
			int offset = GetWordOffset(words[flag], m_charFrom[curWord], m_charTo[curWord]);
			//assert(offset >= 0);

			// Means an ASCII code
			if(words[flag][0] > 0)
			{
				if(offset != -1)
				{
					AddExtractedWord(offset, prev);
				}

				// To find next ASCII or CHN character
				prev = cur;
				flag = 0;
				continue;
			}

			// Mean a the first CHN word in one vacable
			if(flag == 0)
			{
				// Not find this word yet
				if(offset == -1)
				{
					continue;
				}

				lastOffset = offset;
				lastFlag =0;
			}

			if(offset != -1)	
			{
				if(m_sql.m_misses == 1)
				{
					AddExtractedWord(offset, prev);
				}
				else 
				{
					// Record the last CHN word in current extracted vacable
					lastOffset = offset;	
					lastFlag = flag;
				}
			}
			flag++;

			// As far, one vacable only contains two CHN character
			if((k >= m_bytesPerVacable) || (cursor[cur-2] < 0 && cursor[cur] > 0))
			{
				// Only to find one vacable and directly skip this vacable
				if(m_sql.m_misses == 0)
				{
					AddExtractedWord(lastOffset, prev);
					cur = prev + ::strlen(words[lastFlag]);
				}
				else
				{
					cur = prev + ::strlen(words[0]);
				}

				prev = cur;
				flag = 0;
			}
		}
	}

	/*!
	*
	**/
	int CQuickWordIndex::GetWordOffset(char *oneWord, int from, int to)
	{
		if (from>m_bodyCount)
			return -1;
		//if (from==to)
		//	return from;
		//
		int mid(0);
		while(from <= to)
		{
			mid = (from + to)/2;
			int cmp(::strcmp(m_characters[mid].m_bytes,oneWord));
			if (cmp == 0)
			{
				return mid;
			}
			else if(cmp < 0)
			{
				from = mid + 1;
			}
			else 
			{
				to = mid-1;
			}
		}

		//assert(false);
		return -1;
	}

	/*!
	*
	**/
	void CQuickWordIndex::AddExtractedWord(int offset, int pos)
	{
		// Whether already got
		int i = 0;
		int count = m_extractedWords.size();
		for(; i < count; i++)
		{
			if(m_extractedWords[i]->m_offset == offset)
			{
				break;
			}
		}

		// If not found in current vector
		if(i == count)
		{
			QuickExtractedWord *oneWord = (QuickExtractedWord *)::malloc(sizeof(QuickExtractedWord));
			memset(oneWord,0x00,sizeof(QuickExtractedWord));
			oneWord->m_offset = -1;
			assert(oneWord);

			m_extractedWords.push_back(oneWord);
		}

		// Update
		m_extractedWords[i]->m_position[m_extractedWords[i]->m_occurances] = pos;
		m_extractedWords[i]->m_occurances++;
		m_extractedWords[i]->m_offset = offset;
	}

	/*!
	*
	**/
	unsigned CQuickWordIndex::GetIdxes(void)
	{
		register int startIdx(0),endIdx(m_idxCount-1),midIdx(0),cmp(0);
		//
#ifndef USE_NEW_DATA
		unsigned codeMask(0xff00);
		if (m_sql.m_addrOne&0x00ff)
		{
			codeMask = 0xffff;
		}
#else
		unsigned codeMask(0xff0000);
		if (m_sql.m_addrOne&0x0000ff)
		{
			codeMask = 0xffffff;
		}
		else if (m_sql.m_addrOne&0x00ff00)
		{
			codeMask = 0xffff00;
		}
#endif
		//
		while (startIdx<=endIdx)
		{
			midIdx = (startIdx+endIdx)>>1;
			if (!(cmp=((*(m_cities+midIdx))&codeMask)-m_sql.m_addrOne))
			{
				if (m_sql.m_addrOne&0x0000ff)
				{
					m_startIdx = m_endIdx = midIdx;
					return First();
				}
				else
				{
					m_endIdx = GetIdxes(codeMask,midIdx,endIdx,false);
					m_startIdx = GetIdxes(codeMask,startIdx,midIdx,true);
					return First();
				}
			}
			else if (cmp>0)
			{
				endIdx = midIdx-1;
			}
			else
			{
				startIdx = midIdx+1;
			}
		}
		return SQL_ZeroRecord;
	}
	int CQuickWordIndex::GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart)
	{
		register int mid(0),rt(-1),cmp(0);
		register int increase(isGetStart?-1:1);
		register int *pPtr(isGetStart?&endIdx:&startIdx);
		//
		while (startIdx<=endIdx)
		{
			mid = (startIdx+endIdx)>>1;
			if (!(cmp=((*(m_cities+mid))&codeMask)-m_sql.m_addrOne))
			{
				rt = mid;
				*pPtr = mid+increase;
			}
			else if (cmp>0)
			{
				endIdx = mid-1;
			}
			else
			{
				startIdx = mid+1;
			}
		}
		return rt;
	}
	/*!
	*
	**/
	bool CQuickWordIndex::GetTIDs(QuickExtractedWord &curWord, int cityOffset)
	{
		// Read file offsets of TIDS belonging to the same word in different cities
		// One character -> Offsets of its host cities
		assert(m_indexHandle);
		int offset = m_characters[curWord.m_offset].m_offset;
		if(offset == 0)
			return false;
		m_fileBasic.SeekFile(m_indexHandle, offset+sizeof(int)*cityOffset, CFileBasic::UE_SEEK_BEGIN);

		int count = 1;
		void *ptr = &offset;
		m_fileBasic.ReadFile(m_indexHandle, &ptr, sizeof(int), count);
		if(offset == 0)
			return false;
		// Read TIDs part for current word in specified city in group and block definitions
		m_fileBasic.SeekFile(m_indexHandle, offset, CFileBasic::UE_SEEK_BEGIN);

		// Number of Tids of current extracted word
		count = 3;
		ptr = &curWord.m_tids;
		m_fileBasic.ReadFile(m_indexHandle, &ptr, sizeof(unsigned char), count);

		// The whole size of TID part in group and block definition
		ptr = &curWord.m_dataSize;
		m_fileBasic.ReadFile(m_indexHandle, &ptr, sizeof(unsigned char), count);

		// Get group data
		//assert(curWord.m_dataBuffer);
		curWord.m_dataBuffer = (char *)::malloc(curWord.m_dataSize);
		count = curWord.m_dataSize;
		ptr = &(curWord.m_dataBuffer[0]);
		m_fileBasic.ReadFile(m_indexHandle, &ptr, sizeof(char), count);
		return true;
	}

	/*!
	* Note:
	* The offsets of body entries are organized into GROUPs which has the maximum number is 60000, and each GROUP has blocks whose number
	* is 250
	**/
	void CQuickWordIndex::GetInitialResultCells(const QuickExtractedWord &curWord, QuickResultCell *cells, QuickResultRow *rows, int &minCount)
	{
		// Exception
		assert(m_indexHandle);
		assert(curWord.m_dataBuffer);

		// Read group basic information which is leaded by one word and its corresponding city
		char *cursor = curWord.m_dataBuffer;
		// Order indicating each group
		unsigned char nextGrpOrder = 0;
		// Number of blocks belonging to current group
		unsigned char nextFirstBlockOrder = 0;

		// Get the header description of the first group
		::memcpy(&nextGrpOrder, cursor++, sizeof(char));
		::memcpy(&nextFirstBlockOrder, cursor++, sizeof(char));
		unsigned char curGrpOrder = nextGrpOrder;
		unsigned char curBlockOrder = nextFirstBlockOrder;

		// In fact, each node of QuickResultRow is a implicit single list, which contains the tids for the same extracted word in the same city
		int prevTidOrder = 0;
		int curTidOrder = 0;

		unsigned char pos = 0;
		unsigned char posTemp = 0;
		unsigned char posNum = 0;
		unsigned char posNumTemp = 0;

		unsigned int tid;
		// Loop the whole buffer and extract information in group
		while(curGrpOrder != 0xFF)
		{
			// Get header information of each group
			int grpStartPos = (curGrpOrder - 1) * m_maxGrpTids;
			::memcpy(&nextGrpOrder, cursor++, sizeof(char));
			::memcpy(&nextFirstBlockOrder, cursor++, sizeof(char));

			// Loop the whole buffer and extract information in blocks
			unsigned char nextBlockOrder = 0;
			while(curBlockOrder != 0xFF)
			{
				// Get header information of each block
				int blockStartPos = (curBlockOrder - 1) * m_maxBlockTids;
				unsigned char blocks = 0;
				::memcpy(&nextBlockOrder, cursor++, sizeof(char));
				::memcpy(&blocks, cursor++, sizeof(char));

				// 
				unsigned char i = 0;
				unsigned char diff = 0;
				for(; i < blocks; i++)
				{
					memcpy(&diff, cursor++, sizeof(char));
					tid = grpStartPos + blockStartPos + diff;

					// Get position information in byte, the first three bits represent the number of position, the last five bits represent real position
					// Namely, XXXYYYYY, XXX means number, YYYYY means position
					// However, if occurances of current extracted word in current query string exceeds XXX, namely 7, using first bits of the next byte
					// ...
					pos = 0;
					posTemp = 0;
					posNum = 0;
					posNumTemp = 0;
					while(true)
					{
						::memcpy(&pos, cursor+posTemp, sizeof(char));
						posNum = pos >> 5;
						if(posNum == 0)
						{
							break;
						}

						posNumTemp += posNum;
						posTemp++;	// Move to next byte which may still carry number info in the first three bytes
						if(posNum < 7)
						{
							break;
						}
					}
					posNum = posNumTemp;

					// Fill row node information represented by QuickResultRow
					//::memset(rows + curTidOrder, 0x00, sizeof(QuickResultRow));
					memcpy((void *)rows[curTidOrder].m_tid, &tid,3);
					rows[prevTidOrder].m_next = curTidOrder;
					rows[curTidOrder].m_next = 0xFFFF;
					prevTidOrder = curTidOrder;

					// Fill cell information represented by QuickResultCell
					cells[curTidOrder].m_occurances = posNum;
					//::memset(cells[curTidOrder].m_offset, 0x00, 3);
					unsigned int posOffset = cursor - curWord.m_dataBuffer;
					memcpy(cells[curTidOrder].m_offset, &posOffset, 3);
					cursor += posNum;
					curTidOrder++;
				}
				curBlockOrder = nextBlockOrder;
			}
			curGrpOrder = nextGrpOrder;
			curBlockOrder = nextFirstBlockOrder;
		}

		minCount = curTidOrder;
	}

	/*!
	*
	**/
	void CQuickWordIndex::GetIntersectedResultCells(unsigned short *headTidOrder, const QuickExtractedWord &curWord, QuickResultCell *cells, QuickResultRow *rows)
	{
		// Exception
		assert(m_indexHandle);
		assert(curWord.m_dataBuffer);

		// Read group basic information which is leaded by one word and its corresponding city
		char *cursor = curWord.m_dataBuffer;
		// Order indicating each group
		unsigned char nextGrpOrder = 0;
		// Number of blocks belonging to current group
		unsigned char nextFirstBlockOrder = 0;

		// Get the header description of the first group
		::memcpy(&nextGrpOrder, cursor++, sizeof(char));
		::memcpy(&nextFirstBlockOrder, cursor++, sizeof(char));
		unsigned char curGrpOrder = nextGrpOrder;
		unsigned char curBlockOrder = nextFirstBlockOrder;

		//
		unsigned short curRec = *headTidOrder;
		unsigned short lastRec = *headTidOrder;

		int grpStartPos = 0;
		unsigned char nextBlockOrder = 0;
		int blockStartPos = 0;
		unsigned char blocks = 0;
		bool isLastGrp = false;
		unsigned int startPos = 0;
		unsigned int grpSize = 0;
		// Loop the whole buffer and extract information in group
		while(curGrpOrder != 0xFF)
		{
			// Get header information of each group
			grpStartPos = (curGrpOrder - 1) * m_maxGrpTids;
			::memcpy(&nextGrpOrder, cursor++, sizeof(char));
			::memcpy(&nextFirstBlockOrder, cursor++, sizeof(char));

			// Loop the whole buffer and extract information in blocks

			while(curBlockOrder != 0xFF)
			{
				// Get header information of each block
				blockStartPos = (curBlockOrder - 1) * m_maxBlockTids;

				::memcpy(&nextBlockOrder, cursor++, sizeof(char));
				::memcpy(&blocks, cursor++, sizeof(char));

				// Whether is the last group
				isLastGrp = (nextBlockOrder == 0xFF) && (nextGrpOrder == 0xFF);

				// Do intersect
				startPos = grpStartPos + blockStartPos;
				grpSize = 0;
				DoIntersect(curWord, headTidOrder, rows, cells, cursor, startPos, blocks, isLastGrp, curRec, lastRec, grpSize);

				//
				cursor += grpSize;
				if(curRec == 0xFFFF)
				{
					break;
				}
				curBlockOrder = nextBlockOrder;
			}

			if(curRec == 0xFFFF)
			{
				break;
			}
			curGrpOrder = nextGrpOrder;
			curBlockOrder = nextFirstBlockOrder;
		}
	}

	/*!
	*
	**/
	void CQuickWordIndex::GetLastResultCells(const QuickExtractedWord &curWord, const QuickResultCell &curCell)
	{
		//
		char *cursor = curWord.m_dataBuffer;
		unsigned int posOffset = 0;
		memcpy(&posOffset, curCell.m_offset, 3);
		cursor += posOffset;

		// According to occruances, the maximum bytes for storing the occurance number
		// ...
		unsigned char bytes = curCell.m_occurances / 7+1;
		unsigned char i = 0;
		unsigned char pos = 0;
		for(; i < curCell.m_occurances; i++)
		{
			::memcpy(&pos,(unsigned char*)cursor,1);
			if(i < bytes)
			{
				pos = pos & 0x1F;
			}
			::memcpy(QuickExtractedWord::m_intersectedBuffer+pos, m_characters[curWord.m_offset].m_bytes, ::strlen(m_characters[curWord.m_offset].m_bytes));
			cursor++;
		}
	}

	/*!
	*
	**/
	void CQuickWordIndex::DoIntersect(const QuickExtractedWord &curWord, unsigned short *head, QuickResultRow *rows, QuickResultCell *cells, char *cursor, unsigned int startPos, unsigned char blocks, bool isLastGrp, unsigned short &curRec, unsigned short &lastRec, unsigned int &grpSize)
	{
		//
		unsigned int cmpTid = 0;
		unsigned short curTidOrder = curRec;
		unsigned short prevTidOrder = lastRec;
		unsigned short headTidOrder = *head;

		unsigned int blockOrder = 0;


		unsigned char pos = 0;
		unsigned char posTemp = 0;
		unsigned char posNum = 0;
		unsigned char posNumTemp = 0;
		//
		while(curTidOrder != 0xFFFF) 
		{
			// Get TID from the rows got by GetInitialResultCells(...) function
			cmpTid = 0;
			::memcpy(&cmpTid, rows[curTidOrder].m_tid, 3);

			// Loop current block information
			while(blockOrder < blocks)
			{
				// Get differenciated tid
				unsigned char diff = 0;
				::memcpy(&diff, (unsigned char *)cursor, sizeof(unsigned char));
				unsigned int curTid = startPos + diff;

				// Move to next block
				if(curTid < cmpTid)
				{
					cursor++;
					grpSize++;

					// Get position number
					pos = 0;
					posTemp = 0;
					posNum = 0;
					posNumTemp = 0;
					while(true)
					{
						::memcpy(&pos, cursor + posTemp, sizeof(char));
						posNum = pos >> 5;
						if(posNum == 0)
						{
							break;
						}

						posNumTemp += posNum;
						posTemp++;
						if(posNum < 7)
						{
							break;
						}
					}
					posNum = posNumTemp;
					cursor += posNum;
					grpSize += posNum;
					blockOrder++;
				}
				else if(curTid == cmpTid)
				{
					pos = 0;
					posTemp = 0;
					posNum = 0;
					posNumTemp = 0;
					while(true)
					{
						::memcpy(&pos ,cursor + 1 + posTemp, sizeof(char));
						posNum = pos >> 5;
						if(posNum == 0)
						{
							break;
						}

						posNumTemp += posNum;
						posTemp++;
						if(posNum < 7)
						{
							break;
						}
					}
					posNum = posNumTemp;

					// Fill Result cell information since both tids are intersected
					cells[curTidOrder].m_occurances = posNum;
					unsigned int posOffset = cursor + 1 - curWord.m_dataBuffer;
					::memcpy(cells[curTidOrder].m_offset, &posOffset, 3);

					//
					prevTidOrder = curTidOrder;
					curTidOrder = rows[curTidOrder].m_next;

					cursor++;
					grpSize++;
					cursor += posNum;
					grpSize += posNum;
					blockOrder++;
					break;
				}
				else
				{
					if(curTidOrder == headTidOrder)
					{
						headTidOrder = rows[headTidOrder].m_next;
						prevTidOrder = curTidOrder = headTidOrder;
					}
					else
					{
						curTidOrder = rows[curTidOrder].m_next;
						rows[prevTidOrder].m_next = curTidOrder;
					}
					break;
				}
			}

			// Whether meet with the last block of the last group
			if(blockOrder == blocks)
			{
				if(isLastGrp)
				{
					if(curTidOrder != 0xFFFF)
					{
						if(curTidOrder == headTidOrder)
						{
							headTidOrder = 0xFFFF;
						}
						else
						{
							rows[prevTidOrder].m_next = 0xFFFF;
						}
					}
				}
				break;
			}
		}

		curRec = curTidOrder;
		lastRec = prevTidOrder;
		*head = headTidOrder;

		return;
	}

	/*!
	*
	**/
	bool CQuickWordIndex::CompareExtractedWord(const QuickExtractedWord *first, const QuickExtractedWord *second)
	{
		return first->m_tids < second->m_tids;
	}
}