#ifndef __XLB_PACK_RAW_INTERFACE_H__
#define __XLB_PACK_RAW_INTERFACE_H__

#include "pack_interface.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Pack_Raw_Interface : public Pack_Interface
{
public:
    Pack_Raw_Interface();
    virtual ~Pack_Raw_Interface();

public:
    virtual void add_pack_file(const std::string &file);

    virtual Pack_File* pfopen(const char* filename, const char* access) ;
    virtual int pfclose(Pack_File* file);

    virtual int	pfseek(Pack_File* file, long offset, int origin);
    virtual int pftell(Pack_File* file);

    virtual size_t pfread(void* data, int elemsize, int count, Pack_File* file);

    virtual int pfgetc(Pack_File* file);
    virtual int pungetc(int c, Pack_File* file);
    virtual char* pfgets(char* s, int size, Pack_File* file);

    virtual int pfeof(Pack_File* file);

private:
    Pack_Collection_List pack_collection_list_;	
    Pack_Record_Map pack_record_map_;
};

///////////////////////////////////////////////////////////////////

//}
#endif