#ifndef CCUSTOMMODEL_H
#define CCUSTOMMODEL_H

#include "QAbstractItemModel"
#include "icustommodel.h"
#include "ccustomdatatype.h"

class CCustomModel : public QAbstractItemModel, public ICustomModel<CCustomDataType>
{
    Q_OBJECT
    public:

        // TODO
};

#endif // CCUSTOMMODEL_H
