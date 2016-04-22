#ifndef CCUSTOMDATATYPE_H
#define CCUSTOMDATATYPE_H

#include <QObject>
#include <QString>
#include <QDateTime>

class CCustomDataType : public QObject
{
    Q_OBJECT
    public:

        CCustomDataType( QObject* parent = nullptr );
        CCustomDataType( QString id, QString username, QString email, int energy, QDateTime date, QObject* parent = nullptr );

        Q_PROPERTY( QString id MEMBER mId READ GetID WRITE SetID NOTIFY IDChanged )
        Q_PROPERTY( QString username MEMBER mUsername READ GetUsername WRITE SetUsername NOTIFY UsernameChanged )
        Q_PROPERTY( QString email MEMBER mEmail READ GetEmail WRITE SetEmail NOTIFY EmailChanged )
        Q_PROPERTY( int energy MEMBER mEnergy READ GetEnergy WRITE SetEnergy NOTIFY EnergyChanged )
        Q_PROPERTY( QDateTime date MEMBER mDate READ GetDate WRITE SetDate NOTIFY DateChanged )

        void SetID( QString id );
        void SetUsername( QString username );
        void SetEmail( QString email );
        void SetEnergy( int energy );
        void SetDate( QDateTime date );

        QString GetID();
        QString GetUsername();
        QString GetEmail();
        int GetEnergy();
        QDateTime GetDate();


    signals:

        void IDChanged();
        void UsernameChanged();
        void EmailChanged();
        void EnergyChanged();
        void DateChanged();


    private:

        QString mId, mUsername, mEmail;
        int mEnergy;
        QDateTime mDate;

};

#endif // CCUSTOMDATATYPE_H
