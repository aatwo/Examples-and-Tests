#GoogleOAuth2Test
This test demonstrates that it is possible to use C++ and Qt to authenticate with Googles OAuth2 servers.

In order for this to work you need three things: a client ID, a client secret and a scope.

Using the Google developer console do the following (https://console.developers.google.com):
1. Create a new project
2. Enable to Google Drive API for this new project
3. Create some 'OAuth client ID' credentials for this new project

Once this is complete copy the client ID and client secret into the application. Lastly you need to specify a scope. This is essentially a permissions level. Different actions require different scopes and you can find the complete list here: https://developers.google.com/identity/protocols/googlescopes (for example for read / write access to a users google drive data use the value 'https://www.googleapis.com/auth/drive'.

<b>Note:</b> Make sure you have the Qt web engine module installed (you can check using the Qt maintenance tool).

TODO: support the saving and loading of the refresh token so the user doesn't need to log in each time.