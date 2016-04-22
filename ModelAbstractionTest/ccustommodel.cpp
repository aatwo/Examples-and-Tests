#include "ccustommodel.h"

CCustomModel::CCustomModel()
{

}

CCustomDataType* CCustomModel::GetData( int section, int row )
{
    // TODO
}

void CCustomModel::SetHeaders(const QStringList& headers)
{
    // TODO
}

void CCustomModel::AddSection( QString sectionTitle )
{
    // TODO
}

bool CCustomModel::AddSection( int sectionInsertionIndex, QString sectionTitle )
{
    //TODO
    return false;
}

bool CCustomModel::SetSectionTitle( int section, QString sectionTitle )
{
    //TODO
    return false;
}

bool CCustomModel::InsertRowData( int section, int rowInsertionIndex, QList<CCustomDataType*>& rowData )
{
    //TODO
    return false;
}

bool CCustomModel::InsertRows( int section, int rowInsertionIndex, int count )
{
    //TODO
    return false;
}

int CCustomModel::ColumnCount()
{
    static CCustomDataType data;
    return data.metaObject()->propertyCount();
}
