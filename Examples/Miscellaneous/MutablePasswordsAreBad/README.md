# MutablePasswordsAreBad

I've noticed a number of people mentioning that they use mutable passwords, i.e. all their passwords follow some common set of rules such that they need only to remember the rules from which they can generate any of their passwords. While this is significantly better than the common practice of using one password for everything, it is still significantly worse than using a password manager. This is because if any of your passwords are obtained by an attacker they could use it as a base for guessing your other passwords. Even worse if more than one of your passwords are discovered because it makes it easier for someone to see patterns. This might seem far fetched at first but when you consider than there are hundreds of thousands of websites allowing users to create accounts it becomes slightly more concerning. By using mutable passwords you are betting on the fact that every single one of the fifty or so websites that you have passwords with are using the latest security technologies, practices and procedures at all times. Obviously this isn't how it works in the real world and hacks occur all the time, many of which you might not hear about for years if ever (see https://haveibeenpwned.com/ for a great list of known breaches) and it's definitely fair to say that it's not unlikely that at least two of your passwords will at some point be simultaneously exposed to the world.

This small application plays on a scenario where a user has three accounts. One for Google, another for Spotify and another for Facebook. The passwords for these websites were generated using the following rules:

1. The count of letters in the domain.
2. If the Rule #1 number is even use a =, else if it is odd use a — .
3. Use the day number from the date adding the digit from Rule #1.
4. If the first letter of the domain is vowel use it in uppercase, else if it is consonant use it in lowercase.
5. Use the month digit(s) from the date adding the digit from Rule #1.
6. If the last letter of the domain is vowel use it in uppercase, else if it is consonant use it in lowercase.
7. Use the year’s digits from the date this time removing the digit from Rule #1.
8. If the domain has .com at the end use the keyword in lowercase, else if it has anything else use the keyword in uppercase.
9. Always use a . at the end of the password.

This generates the following passwords for each website:

Google: 6=20g15E2010rabbit.
Spotify: 7–21s16Y2009rabbit.
Facebook: 8=22f17k2008rabbit.

In our hypothetical situation we play the role of an attacker who has gained access to this users Google and Spotify passwords and we want to access their facebook account. How might we accomplish this? 

Firstly we would look at those two passwords and attempt to guess the rules being used. Just by looking at these two passwords (Google: 6=20g15E2010rabbit. and Spotify: 7–21s16Y2009rabbit.) the following rules can easily be approximated:

1. char 1: company name length (this one is pretty obvious)
2. char 2: = or -
3. char 3-4: some number in the twenties
4. char 5: lower case initial company name letter (again very obvious)
5. char 6-7: some number in the teens
6. char 8: lower or upper case company name ending (fairly obvious)
7. char 9-12: some number between 2005 and 2015
8. char 13-20: rabbit.

As you can see these rules are different to the originals and it's not guarenteed that approximating the rules in this way will always lead to the unknown password but for now they make a good starting point.

After running the password guessing program five times here are the number of attempts it took to guess the unknown Facebook password correctly:

1. 1560
2. 896
3. 3002
4. 77
5. 1487
