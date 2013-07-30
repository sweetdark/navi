#ifndef _UEGUI_ASSISTANT_H
#define _UEGUI_ASSISTANT_H
class CAssistant {
public:
  //判断4S和景点数据的区域是否属于目标区域
  //4S和景点的区域编码是十进制的需要进行转换
  static bool IsBelongTo(int src, int dest)
  {
    //编码转换规则。使用24位的二进制来表示。后八位是地区级别
    unsigned int first = src / 10000;
    unsigned int two = (src % 10000) / 100;
    unsigned int last = src  % 100;
    first = first << 16;
    two = two << 8;
    unsigned int result = first | two | last;
    //(dest & 0x0000ff) == 0 表示dest是市级别以上的。那只要比较是级别以上的编码
    if ( result == dest ||
      ((dest & 0x0000ff) == 0 && (result & 0xffff00) == (dest & 0xffff00) )
      || ((dest == 720896) && (result & 0xff0000) == (dest & 0xff0000)) //北京市
      || ((dest == 786432) && (result & 0xff0000) == (dest & 0xff0000)) //天津市
      || ((dest == 2031616) && (result & 0xff0000) == (dest & 0xff0000)) //上海市
      || ((dest == 5308416) && (result & 0xff0000) == (dest & 0xff0000))  //香港
      || ((dest == 5373952) && (result & 0xff0000) == (dest & 0xff0000)))  //澳门
    {
      return true;
    }
    return false;
  }
};
#endif