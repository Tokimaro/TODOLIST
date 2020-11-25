# SpotifyPlaylistsBot
SpotifyPlaylistsBot — это телеграм бот для переноса плейлистов в Spotify из Вконтакте, Яндекс Музыки и YouTube. Также у 
него есть дополнительные функции, такие как создание "перевернутого" плейлиста и просмотр топа треков или исполнителей 
своего аккаунта. Чтобы найти бота в Телеграме, можно ввести в поиске **@playlistsforspotifybot**
## Команды
**/start** — начало работы. Приветствие и краткое описание работы бота.  
**/help** — полный список всех команд.  
**/auth** — авторизация в свой аккаунт Spotify.  
**/tutorial** — инструкция о том, как получить правильные ссылки на плейлисты.  
**/transfer** — перенос плейлистов по ссылке из VK, Яндекс Музыки или YouTube в Spotify.  
**/statistics** — вывод топа 20 треков или исполнителей за месяц, полгода или все время данного пользователя.  
**/reverse** — создание "перевернутого" плейлиста какого-либо существующего плейлиста в Spotify по ссылке.  
## Принцип работы
### Telegram Bot API
Бот работает через **Telegram Bot API** (библиотека telebot). Создавался SpotifyPlaylistsBot через другого бота 
**BotFather**, при помощи которого был получен токен для управления ботом.  
```
bot = telebot.TeleBot(config.TOKEN)  # создание объекта "bot"
... функции с декораторами
@bot.message_handler(content_types=['text'])
def text_reaction(message):  # реакция на любое сообщение, которое не является командой
    bot.send_message(message.chat.id, 'Неизвестная команда')
...
bot.polling(none_stop=True)  # запуск бота
```
Реакция на команды:
```
@bot.message_handler(commands=['transfer'])  # срабатывает после команды /transfer
def transfer(message):
    # code 
```
Многоуровневое меню:
```
@bot.callback_query_handler(func=lambda call: True)
def callback_inline(call):  # callback_data передается в данную функцию
    if call.data == '1':
        # code
    elif call.data == '2':
        # code


@bot.message_handler(commands=['transfer'])
def transfer(message):
    markup = types.InlineKeyboardMarkup(row_width=2)  # создание "клавиатуры" внутри сообщения, при нажатии на которую
                                                        будет передаваться callback
    item1 = types.InlineKeyboardButton('1', callback_data='1')
    item2 = types.InlineKeyboardButton('2', callback_data='2')
    markup.add(item1, item2)
    bot.send_message(message.chat.id, '1 2', reply_markup=markup)
```
### Spotify Web API
Добавление плейлистов в Spotify осуществляется при помощи **Spotify Web API** (библиотека 
spotipy). Данное API дает полный доступ ко всем музыкальным данным, хранящимся на сервисе. Для доступа к ним сначала 
необходимо зарегистрировать свое приложение на developer.spotify.com. После получения CLIENT ID и CLIENT SECRET 
откроется доступ ко всем данным.  
<img src="https://i.ibb.co/F4tPPJw/dashboard.png" height="350" />
  
```
spotify = spotipy.Spotify(auth=token)  #создание объекта spotify
```
В объекте spotify хранятся методы, которые позволяют получать информацию об исполнителях, треках, альбомах, создавать 
и изменять плейлисты, смотреть статистику и т.д.
```
playlist = spotify.user_playlist_create(username, 'name', public=True, description='description')  # создание плейлиста
results = spotify.search(q='track:' + 'track name', type='track')  # поиск треков
spotify.playlist_add_items('playlist-id', ['track-id1', 'track_id2'])  # добавление треков в плейлист
playlist = spotify.playlist('playlist-spotify-uri')  # получение объекта плейлиста
top = spotify.current_user_top_tracks(time_range='short_term')  # топ 20 треков пользователя за месяц
top = spotify.current_user_top_artists(time_range='medium_term')  # топ 20 исполнителей пользователя за полгода
```
Для взаимодействия с аккаунтом пользователя, необходимо при авторизации прописать "scopes". Они определяют, какие 
действия может совершать данное приложение с аккаунтом (может ли смотреть приватные плейлисты, создавать новые, 
получать информацию о прослушанных треках и т.д.).
```
SCOPE = 'playlist-modify-public playlist-modify-private playlist-read-private user-top-read'
```
#### /transfer
После ввода команды /transfer пользователю предлагается выбрать сайт, с которого переносить плейлист. После чего 
необходимо следующим сообщением отправить ссылку на плейлист (важно, чтобы он не был закрытым). Страница с плейлистом 
парсится при помощи библиотеки **BeautifulSoup**. По тегам собирается информация о названиях треков и их исполнителях и 
выгружается в json объект. Далее создается плейлист методом `user_playlist_create` и осуществляется поиск треков при 
помощи метода `search`. Методом `playlist_add_items` самый популярный найденный трек добавляется в плейлист.  
<img src="https://i.ibb.co/pxjNn7j/transfer.png" height="500" />
  
#### /statistics
После ввода команды /statistics у пользователя есть выбор: посмотреть топ своих треков или топ своих исполнителей в 
Spotify. Эти топы можно посмотреть за месяц (`short_term`), полгода (`medium_term`) и несколько лет (`long_term`). Топ 
треков формируется методом `current_user_top_tracks`, а исполнителей – `current_user_top_artists`.  
<img src="https://i.ibb.co/nm1NQs2/statistics.png" height="500" />
  
#### /reverse
Командой /reverse пользователь может продублировать существующий плейлист в Spotify, чтобы треки в нем при этом шли 
с противоположной стороны. После ввода команды пользователю необходимо отправить URI плейлиста. Приложение получает 
плейлист при помощи метода `playlist`, получает из него информацию о всех треках и создает (`user_playlist_create`) 
"перевернутый" дубликат, добавляя (`playlist_add_items`) все треки с обратной стороны.  
<img src="https://i.ibb.co/gvnrd9W/reverse.png" height="500" />
  
#### /auth
Для начала работы пользователю необходимо войти в свой аккаунт Spotify. Параллельно боту работает приложение, которое 
управляет авторизацией при помощи веб-фреймворка Flask. После ввода команды пользователю отправляется ссылка на сайт с 
авторизацией. При переходе по этой ссылке (которая генерируется при помощи сервиса для создания туннелей ngrok) 
отобразится страница с пользовательским соглашением, которое нужно принять для дальнейшей работы с ботом. Ссылка с 
авторизацией "строится", исходя из CLIENT ID (id приложения), REDIRECT URI (указывается в настройках приложения) и 
SCOPE (разрешения приложения).
```
@app.route('/')
def verify():
    auth_url = f'https://accounts.spotify.com/authorize?client_id={config.CLI_ID}' \
               f'&response_type=code&redirect_uri={config.REDIRECT_URI}&scope={config.SCOPE}' \
               f'&show_dialog={config.SHOW_DIALOG}'
    return redirect(auth_url)


@app.route('/api_callback')
def api_callback():
    session.clear()
    code = request.args.get('code')
    auth_token_url = f'https://accounts.spotify.com/api/token'
    res = requests.post(auth_token_url, data={
        'grant_type': 'authorization_code',
        'code': code,
        'redirect_uri': config.REDIRECT_URI,
        'client_id': config.CLI_ID,
        'client_secret': config.CLI_SEC
        })
    res_body = res.json()
    session['toke'] = res_body.get('access_token')
    token = session['toke']
```
  
<img src="https://i.ibb.co/ggyJ5Vd/auth.png" height="500" />
  
После нажатия кнопки "Принять" отправляется запрос на страницу api_callback (указывается в настройках приложения), а 
телеграм боту отправляется токен, при помощи которого и происходит авторизация.
```
spotipy.Spotify(auth=token)
```
Для получения пользователя можно воспользоваться методом `current_user`, а впоследствии и получить никнейм.
```
user = spotify.current_user()
username = user['uri'].split(':')[2]
```
