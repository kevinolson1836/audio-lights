import spotipy
from spotipy.oauth2 import SpotifyClientCredentials
import os

os.environ['SPOTIPY_CLIENT_ID'] = '853c7b679de7421f9feb896896a62592'
os.environ['SPOTIPY_CLIENT_SECRET'] = 'ab3bfed834564cea870cbaee1590560d'
# os.environ['SPOTIPY_REDIRECT_URI'] = 'variable_value'

# export SPOTIPY_CLIENT_ID='your-spotify-client-id'
# export SPOTIPY_CLIENT_SECRET='your-spotify-client-secret'
# export SPOTIPY_REDIRECT_URI='your-app-redirect-url'

auth_manager = SpotifyClientCredentials()
sp = spotipy.Spotify(auth_manager=auth_manager)

playlists = sp.user_playlists('test')
while playlists:
    for i, playlist in enumerate(playlists['items']):
        print("%4d %s %s" % (i + 1 + playlists['offset'], playlist['uri'],  playlist['name']))
    if playlists['next']:
        playlists = sp.next(playlists)
    else:
        playlists = None