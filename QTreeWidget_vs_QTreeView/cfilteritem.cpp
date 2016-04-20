#include "cfilteritem.h"

CFilterItem::CFilterItem(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout( this );

    columnEdit = new QSpinBox( this );
    valueEdit = new QLineEdit( this );
    QPushButton* removeButton = new QPushButton( "Remove", this );

    connect( columnEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CFilterItem::FilterChanged );
    connect( valueEdit, &QLineEdit::editingFinished, this, &CFilterItem::FilterChanged );
    connect( removeButton, &QPushButton::clicked, this, &CFilterItem::FilterRemoved );

    layout->addWidget( new QLabel( "Column:", this ) );
    layout->addWidget( columnEdit );
    layout->addWidget( valueEdit );
    layout->addWidget( removeButton );
}

int CFilterItem::GetColumn()
{
    return columnEdit->value();
}

QString CFilterItem::GetValue()
{
    return valueEdit->text();
}
