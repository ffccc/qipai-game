#ifndef __XLB_PACK_INTERFACE_H__
#define __XLB_PACK_INTERFACE_H__

#include <map>
#include <list>
#include <string>

#include <windows.h>

#include "prereqs.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Pack_Collection
{
public:
    HANDLE					file_handle_;
    HANDLE					mapping_handle_;
    void*					  data_;
};

class Pack_Record
{
public:
    Pack_Collection*    collection_;
    std::string         file_name_;
    FILETIME            file_time_;
    int                 start_pos_;
    int                 size_;	
};

struct Pack_File
{
    Pack_Record*				record_;
    int						      pos_;
    FILE*					      file_;
};

typedef std::map<std::string, Pack_Record> Pack_Record_Map;
typedef std::list<Pack_Collection> Pack_Collection_List;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Pack_Interface
{
public:
    Pack_Interface() {}
    virtual ~Pack_Interface() {}

public:
    virtual void add_pack_file(const std::string &file) = 0;

    virtual Pack_File* pfopen(const char* filename, const char* access) = 0;
    virtual int pfclose(Pack_File* file) = 0;

    virtual int	pfseek(Pack_File* file, long offset, int origin) = 0;
    virtual int pftell(Pack_File* file) = 0;

    virtual size_t pfread(void* data, int elemsize, int count, Pack_File* file) = 0;

    virtual int pfgetc(Pack_File* file) = 0;
    virtual int pungetc(int c, Pack_File* file) = 0;
    virtual char* pfgets(char* s, int size, Pack_File* file) = 0;

    virtual int pfeof(Pack_File* file) = 0;
};

///////////////////////////////////////////////////////////////////
extern  Xlbe_Export Pack_Interface*  g_pack_interface;

static Pack_Interface* pack_interface()
{
    return g_pack_interface;
}

static Pack_File* pfopen(const char* filename, const char* access) 
{
    if (pack_interface() != 0)
        return pack_interface()->pfopen(filename, access);

    FILE* f = fopen(filename, access);
    if (f == 0)
        return 0;

    Pack_File* pf = new Pack_File();
    pf->record_ = 0;
    pf->pos_ = 0;
    pf->file_ = f;

    return pf;
}

static int pfclose(Pack_File* file)
{
    if (pack_interface() != 0)
        return pack_interface()->pfclose(file);	

    int result = fclose(file->file_);
    delete file;
    return result;
}

static int	pfseek(Pack_File* file, long offset, int origin) 
{
    if (pack_interface() != 0)
        return pack_interface()->pfseek(file, offset, origin);	

    return fseek(file->file_, offset, origin);
}

static int pftell(Pack_File* file) 
{
    if (pack_interface() != 0)
        return pack_interface()->pftell(file);	

    return ftell(file->file_);
}

static size_t pfread(void* data, int elemsize, int count, Pack_File* file)
{
    if (pack_interface() != 0)
        return pack_interface()->pfread(data,elemsize,count,file);


    return fread(data, elemsize, count, file->file_);

}

static size_t pfwrite(const void* data, int elemsize, int count, Pack_File* file)
{	
    if (file->file_ == 0)
        return 0;

    return fwrite(data, elemsize, count, file->file_);
}

static int pfgetc(Pack_File* file)
{
    if (pack_interface() != 0)
        return pack_interface()->pfgetc(file);	

    return fgetc(file->file_);
}

static int pungetc(int c, Pack_File* file) 
{
    if (pack_interface() != 0)
        return pack_interface()->pungetc(c, file);	

    return ungetc(c, file->file_);
}

static char* pfgets(char* s, int size, Pack_File* file) 
{
    if (pack_interface() != 0)
        return pack_interface()->pfgets(s, size, file);	

    return fgets(s, size, file->file_);
}

static int pfeof(Pack_File* file) 
{
    if (pack_interface() != 0)
        return pack_interface()->pfeof(file);	

    return feof(file->file_);
}

/////////////////////////////////////////////////////////////////////

//}
#endif
