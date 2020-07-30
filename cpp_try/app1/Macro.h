#ifndef __Macro_h__
#define __Macro_h__

#include <string>

typedef unsigned char Byte;
typedef char SByte;
typedef short Int16;
typedef unsigned short UInt16;
typedef int Int32;
typedef unsigned int UInt32;
typedef __int64 Int64;
typedef unsigned __int64 UInt64;

#define SAFE_DELETE(p) if (p!=NULL){delete(p); p=NULL;}
#define SAFE_DELETE_ARRAY(p) if (p!=NULL){delete[](p); p=NULL;}
#define SAFE_DELETE_VECTOR(v) for (auto itr = v.begin(); itr != v.end(); ++itr){ SAFE_DELETE(*itr); } v.clear();
#define SAFE_DELETE_MAP(p) for (auto itr = p.begin(); itr != p.end(); ++itr){ SAFE_DELETE(itr->second); } p.clear();

#endif
