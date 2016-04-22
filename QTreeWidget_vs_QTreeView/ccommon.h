#ifndef CCOMMON_H
#define CCOMMON_H

#include <QtCore>

namespace common
{
    struct FilterGroupDescription
    {
        QList< QPair<int, QString> > filterItemDescriptions; // Pairs of columns and filter-values
    };
}

#endif // CCOMMON_H
