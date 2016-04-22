#include "ccustomdatatype.h"

CCustomDataType::CCustomDataType( QObject*  parent )
    : QObject( parent )
{
    mEnergy = 0;
}

CCustomDataType::CCustomDataType( QString id, QString username, QString email, int energy, QDateTime date, QObject* parent )
    : QObject( parent )
{
    mId = id;
    mUsername = username;
    mEmail = email;
    mEnergy = energy;
    mDate = date;
}

void CCustomDataType::SetID( QString id )
{
    mId = id;
    emit IDChanged();
}

void CCustomDataType::SetUsername( QString username )
{
    mUsername = username;
    emit UsernameChanged();
}

void CCustomDataType::SetEmail( QString email )
{
    mEmail = email;
    emit EmailChanged();
}

void CCustomDataType::SetEnergy( int energy )
{
    mEnergy = energy;
    emit EnergyChanged();
}

void CCustomDataType::SetDate( QDateTime date )
{
    mDate = date;
    emit DateChanged();
}

QString CCustomDataType::GetID()
{
    return mId;
}

QString CCustomDataType::GetUsername()
{
    return mUsername;
}

QString CCustomDataType::GetEmail()
{
    return mEmail;
}

int CCustomDataType::GetEnergy()
{
    return mEnergy;
}

QDateTime CCustomDataType::GetDate()
{
    return mDate;
}
