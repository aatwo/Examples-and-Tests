#ifndef ICUSTOMMODEL_H
#define ICUSTOMMODEL_H

// Provides an interface for a simple tree model of depth two that takes the following structure (root is typically invisible):
//
//      Root
//          Section 0
//              row 0
//              row 1
//              row 2
//
//          Section 1
//              row 0
//              row 1
//              row 2
//
//      ... etc ...

#include <QList>

template <class RowData>
class ICustomModel
{
    public:

        // Fetch data
        virtual RowData     GetData( int section, int row ) = 0;                                                // Gets the data stored in the specified section / row. Returns null for invalid section / row combinations

        // Headers
        virtual void        SetHeaders( const QStringList& headers ) = 0;

        // Sections
        virtual void        AddSection( QString sectionTitle ) = 0;                                             // Appends a section at the end of the section list
        virtual bool        AddSection( int sectionInsertionIndex, QString sectionTitle ) = 0;                  // Adds a section at the specified insertion index. Can return false if the sectionInsertionIndex is invalid
        virtual bool        SetSectionTitle( int section, QString sectionTitle ) = 0;                           // Sets the title for the specified section. Can return false if the section is invalid

        // Data
        virtual bool        InsertRowData( int section, int rowInsertionIndex, QList<RowData>& rowData ) = 0;   // Inserts rowData into the model. Can fail if the specified section / insertion index is invalid
        virtual bool        InsertRows( int section, int rowInsertionIndex, int count ) = 0;                    // Inserts count rows into the model using RowDatas default constructor. Can fail if the specified section / insertion index combination is invalid
        virtual bool        SetRowData( int section, int row, RowData rowData ) = 0;                            // Sets the data for the specified row. Fails if the section / row does not exist
        virtual bool        RemoveRows( int section, int rowRemovalIndex, int count ) = 0;                      // Removes count rows from the model starting with row rowRemovalIndex. Can return false if the section / rowRemovalIndex / count combination is invalid
        virtual bool        RemoveRow( int section, int rowRemovalIndex ) = 0;                                  // Removes the row at rowRemovalIndex. Can return false if the section and rowRemovalIndex is invalid
        virtual bool        ClearSection( int section ) = 0;                                                    // Removes all the data for the specified section (can return false if section is invalid)

        // Misc
        virtual int         ColumnCount() = 0;

};

#endif // ICUSTOMMODEL_H
