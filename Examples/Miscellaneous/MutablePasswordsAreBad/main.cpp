#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QDateTime>

int     crackPassword( QString passwordToGuess, QString companyName );
int     randomNumber( int min, int max );
QChar   randomChar( QString& possibleChars );

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    srand( QDateTime::currentMSecsSinceEpoch() );

    //----------------------------------------------------------------------------------------------------
    // Passwords 1 and 2 were obtained by some security breach.
    QString password1 = "6=20g15E2010rabbit."; // Google
    QString password2 = "7–21s16Y2009rabbit."; // Spotify

    //----------------------------------------------------------------------------------------------------
    // Password 3 is the password we want to guess
    QString password3 = "8=22f17k2008rabbit."; // Facebook

    //----------------------------------------------------------------------------------------------------
    // After looking at passwords 1 and 2 here are some rudimentary rules that are pretty obvious:
    //      char 1: company name length
    //      char 2: = or -
    //      char 3-4: some number in the twenties
    //      char 5: lower case initial company name letter
    //      char 6-7: some number in the teens
    //      char 8: lower or upper case company name ending
    //      char 9-12: some number between 2000 and 2020
    //      char 13-20: rabbit.

    int guessCount = crackPassword( password3, "Facebook" );
    qDebug() << QString( "Attempts to guess password: %1" ).arg( guessCount );

    return a.exec();
}

int crackPassword( QString passwordToGuess, QString companyName )
{
    QStringList usedPasswords;
    QString     currentGuess;
    int         guessCount                      = 0;
    QString     lowerCaseCompanyName            = companyName.toLower();
    QString     upperCaseCompanyName            = companyName.toUpper();
    QChar       lowerCaseFinalCompanyNameChar   = lowerCaseCompanyName.at( lowerCaseCompanyName.size() - 1 );
    QChar       upperCaseFinalCompanyNameChar   = upperCaseCompanyName.at( upperCaseCompanyName.size() - 1 );
    QString     symbols                         = "=-";
    QString     companyEndings                  = QString( "%1%2" ).arg( lowerCaseFinalCompanyNameChar ).arg( upperCaseFinalCompanyNameChar );

    while( currentGuess != passwordToGuess )
    {
        currentGuess.clear();

        int     finalNumber         = randomNumber( 5, 15 );
        QString finalNumberString   = QString( "%1" ).arg( finalNumber, 2, 10, QChar( '0' ) );

        currentGuess += QString::number( companyName.length() );
        currentGuess += randomChar( symbols );
        currentGuess += QString::number( randomNumber( 20, 29 ) );
        currentGuess += lowerCaseCompanyName.at( 0 );
        currentGuess += QString::number( randomNumber( 10, 19 ) );
        currentGuess +=randomChar( companyEndings );
        currentGuess += "20";
        currentGuess += finalNumberString;
        currentGuess += "rabbit.";

        if( !usedPasswords.contains( currentGuess ) )
        {
            guessCount++;
            usedPasswords.append( currentGuess );
            qDebug() << QString( "Current guess: %1" ).arg( currentGuess );
        }
    }

    return guessCount;
}

int randomNumber( int min, int max )
{
    return qrand() % ( max - min + 1 ) + min;
}

QChar randomChar( QString& possibleChars )
{
    int index = randomNumber( 0, possibleChars.size() - 1 );
    return possibleChars.at( index );
}
