#ifndef __Q_CODE_H__
#define __Q_CODE_H__

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* 经纬度 -- Q码
	*		@param	dX					[in]		经度
	*		@param	dY					[in]		纬度
	*		@param	pCode				[out]		编码缓冲区
	*		@param	nCode				[out]		缓冲区大写
	*		@return	成功返回true，失败返回false
	*/
	bool QCode_MapToCode( double dX, double dY, unsigned short* pCode, int nCode );

	/**
	* Q码 -- 经纬度
	*		@param	pCode				[in]		编码缓冲区
	*		@param	nCode				[in]		缓冲区大写
	*		@param	dX					[out]		经度
	*		@param	dY					[out]		纬度
	*		@return	成功返回true，失败返回false
	*/
	bool QCode_CodeToMap( unsigned short* pCode, int nCode, double& dX, double& dY );

#ifdef __cplusplus
}
#endif


#endif	// __Q_CODE_H__