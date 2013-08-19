#include "fontmanager.h"
#include "uebase/pathconfig.h"
#include "uebase/uemaps.h"
#include "uebase/pathbasic.h"
#include "uebase/stringbasic.h"
using namespace UeBase;

using namespace UeMap;

CFontManager::CFontManager()
{

}


CFontManager::~CFontManager()
{

}

void CFontManager::LoadFont(const MapsFont &fontProp, int faceIdx, int renType)
{
  /*TCHAR dataPath[CPathBasic::MAXPATHLENGTH] = {0, };
  CPathBasic::Get().GetModulePath(dataPath, CPathBasic::MAXPATHLENGTH);*/
  tstring fontFile = CPathConfig::GetCommonPath(CPathConfig::CPK_FontsPath);
  /*CPathBasic::Get().GetPathSeperator(fontFile);
  fontFile += _T("fonts");
  CPathBasic::Get().GetPathSeperator(fontFile);*/
  wchar_t uniFamily[512];
  int len = CStringBasic::Get().Ascii2Wcs(const_cast<char *>(fontProp.m_family), uniFamily, 512);
  fontFile += uniFamily;

  if (m_preFontFile == fontFile)
  {
    if (m_preFont == renType)
    {
      return;
    }
    char name[512];
    int len = CStringBasic::Get().Wcs2Ascii(const_cast<wchar_t *>(fontFile.c_str()), name, 512);
    if(renType == agg::glyph_ren_agg_gray8)
    {
      m_grayFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_grayFont.flip_y(true);
    }
    else if(renType == agg::glyph_ren_outline)
    {
      m_lineFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_lineFont.flip_y(true);
    }
    return;
  }

  if (CPathBasic::Get().IsFileExist(fontFile))
  {
    m_preFontFile = fontFile;
    m_preFont = renType;
    char name[512];
    int len = CStringBasic::Get().Wcs2Ascii(const_cast<wchar_t *>(fontFile.c_str()), name, 512);
    if(renType == agg::glyph_ren_agg_gray8)
    {
      m_grayFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_grayFont.flip_y(true);
    }
    else if(renType == agg::glyph_ren_outline)
    {
      m_lineFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_lineFont.flip_y(true);
    }
  }
  else
  {
    assert(false);
  }

}


void CFontManager::SetGrayFontWidthAndHeight(unsigned char width, unsigned char height)
{
  m_grayFont.width(width);
  m_grayFont.height(height);
}

double CFontManager::GetGrayFontWidth()
{
  return m_grayFont.width();
}

double CFontManager::GetGrayFontHeight()
{
  return m_grayFont.height();
}


void CFontManager::SetLineFontWidthAndHeight(unsigned char width, unsigned char height)
{
  m_lineFont.height(width);
  m_lineFont.width(height);
}

double CFontManager::GetLineFontHeight()
{
  return m_lineFont.height();
}

double CFontManager::GetLineFontWidth()
{
  return m_lineFont.width();
}

font_engine_type& CFontManager::GetGrayFont()
{
  return m_grayFont;
}

font_engine_type& CFontManager::GetLineFont()
{
  return m_lineFont;
}