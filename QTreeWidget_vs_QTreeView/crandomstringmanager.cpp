#include "crandomstringmanager.h"

CRandomStringManager::CRandomStringManager()
{
    minStringSize = 5;
    maxStringSize = 10;
    prependTextState = false;
    injectTextState = false;
    appendTextState = false;
}

int CRandomStringManager::RandomNumber(int min, int max)
{
    return ( qrand() % ( max - min + 1 ) + min );
}

QString CRandomStringManager::RandomString(int length)
{
    const int minAsciiValue = 97;
    const int maxAsciiValue = 122;

    QString ret;

    for( int i = 0; i < length; i++ )
    {
        char c = static_cast<char>( RandomNumber( minAsciiValue, maxAsciiValue ) );
        ret.append( c );
    }

    if( prependTextState )
    {
        ret.prepend( prependText );
    }

    if( injectTextState )
    {
        ret.insert( ret.length() / 2, injectText );
    }

    if( appendTextState )
    {
        ret.append( appendText );
    }

    return ret;
}

QString CRandomStringManager::RandomString()
{
    if( !randomStringQueue.isEmpty() )
        return randomStringQueue.dequeue();

    int length = RandomNumber( minStringSize, maxStringSize );

    return RandomString( length );
}

QStringList CRandomStringManager::RandomStringList(int stringCount)
{
    QStringList ret;

    for( int i = 0; i < stringCount; i++ )
    {
        ret.append( RandomString() );
    }

    return ret;
}

int CRandomStringManager::RandomStringCount()
{
    return randomStringQueue.size();
}

void CRandomStringManager::AppendRandomString( QString string )
{
    randomStringQueue.append( string );
}

void CRandomStringManager::Clear()
{
    randomStringQueue.clear();
}

void CRandomStringManager::SetRandomStringGenerationRange(int minLength, int maxLength)
{
    minStringSize = minLength;
    maxStringSize = maxLength;
}

void CRandomStringManager::SetRandomStringModifierStates(bool prepend, bool inject, bool append)
{
    this->prependTextState = prepend;
    this->injectTextState = inject;
    this->appendTextState = append;
}

void CRandomStringManager::SetRandomStringModifiers(QString prependText, QString injectText, QString appendText)
{
    this->prependText = prependText;
    this->injectText = injectText;
    this->appendText = appendText;
}
