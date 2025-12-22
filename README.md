# Bots login fix

- Možnost zvolit, zda se při aktivním loggeru budou INFO zprávy zobrazovat v češtině, nebo v angličtině.
- Zabraňuje připojení na aktivního AltBota ve hře. Na výběr jsou 3 různé varianty chování modulu:
  - Vykopnutí klienta, který se připojuje na aktivního AltBota. Nejbezpečnější varianta i při mikrolagu.
  - Vykopnutí vlastníka AltBota, na kterého se klient připojuje. Tím dojde k vykopnutí všech jeho AltBotů a následně k bezpečnému připojení. Pozor: při mikrolagu může klient zamrznout.
  - Vykopnutí AltBota, na kterého se klient připojuje. Pozor: při mikrolagu může klient zamrznout.
 
##
- Option to choose whether INFO messages are shown in Czech or English when the logger is enabled.
- Prevents connecting to an active AltBot in-game. There are 3 different behavior modes:
  - Kick the client that is trying to connect to an active AltBot. The safest option, even during micro-lag.
  - Kick the owner of the AltBot the client is trying to connect to. This will kick all of their AltBots and then allow a safe connection. Warning: during micro-lag the client may freeze.
  - Kick the AltBot the client is trying to connect to. Warning: during micro-lag the client may freeze.

----------------------------------------------------------------------------------------------------------------

🔧 Tento modul funguje pouze s tímto jádrem a modulem:  
[AzerothCore WotLK (Playerbot branch)](https://github.com/liyunfan1223/azerothcore-wotlk/tree/Playerbot)

[mod-playerbots (master branch)](https://github.com/liyunfan1223/mod-playerbots) 

##

🔧 This module works only with this core and module:   
[AzerothCore WotLK (Playerbot branch)](https://github.com/liyunfan1223/azerothcore-wotlk/tree/Playerbot)

[mod-playerbots (master branch)](https://github.com/liyunfan1223/mod-playerbots)

----------------------------------------------------------------------------------------------------------------

# License
This module is licensed under the [GNU Affero General Public License v3.0](https://github.com/BeardBear33/mod-guild-village/blob/main/LICENSE).
