#include <iostream>

#include "dbobject.h"

using namespace std;


DbObject::DbObject() :
    m_id(0)
{
}

int DbObject::Id()
{
    return m_id;
}

void DbObject::SetId(int id)
{
    m_id = id;
}

DbObject::~DbObject()
{
}
