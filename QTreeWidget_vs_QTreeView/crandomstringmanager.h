#ifndef CRANDOMSTRINGMANAGER_H
#define CRANDOMSTRINGMANAGER_H

#include <QStringList>
#include <QQueue>

class CRandomStringManager
{
    public:
        CRandomStringManager();

        int RandomNumber( int min, int max );
        QString RandomString( int length );
        QString RandomString();
        QStringList RandomStringList( int stringCount );

        int RandomStringCount();
        void AppendRandomString( QString string );
        void Clear();

        void SetRandomStringGenerationRange( int minLength, int maxLength );
        void SetRandomStringModifierStates( bool prepend, bool inject, bool append );
        void SetRandomStringModifiers( QString prependText, QString injectText, QString appendText );


    private:

        QQueue<QString> randomStringQueue;
        int minStringSize, maxStringSize;
        QString prependText, injectText, appendText;
        bool prependTextState, injectTextState, appendTextState;
};

#endif // CRANDOMSTRINGMANAGER_H
