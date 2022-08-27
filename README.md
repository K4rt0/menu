# Menu
![](https://media.discordapp.net/attachments/894614791308603468/1012984618078711818/Untitled.png?width=440&height=586)

## Installation

Include in your code and begin using the library:

```pawn
#include <menu>
```
## Functions
```bash
Menu_Show(playerid, menuid, const title[]);
Menu_AddItem(playerid, const name[], type = m_TypePrice | m_TypeAmount, amount);
IsMenuOpened(playerid);
Menu_Hide(playerid);
```
## Usage

```pawn
#include <zcmd>
#include <menu>

#define       MENU_AMMUSTORE      1

CMD:testmenu(playerid)
{
    if(IsMenuOpened(playerid))
        SendClientMessage(playerid, 0xAA3333AA, "!!! Menu Opened !!!");
    else
    {
        Menu_Add(playerid, "Shotgun", m_TypePrice, 100000); // Shotgun - $100,000
        Menu_Add(playerid, "Spas-12", m_TypePrice, 70000); // Shotgun - $70,000
        Menu_Add(playerid, "M4A1", m_TypePrice, 200000); // M4A1 - $200,000
        Menu_Add(playerid, "AK-47", m_TypePrice, 250000); // AK-47 - $250,000
        Menu_Add(playerid, "Sniper", m_TypePrice, 400000); // Sniper - $400,000
        
        Menu_Show(playerid, MENU_AMMUSTORE, "Ammu Store");
    }
    return 1;
} 

public OnResponseMenu(playerid, menuid, itemid, const itemname[])
{
    new string[200];
    // if - else
    if(menuid == MENU_AMMUSTORE)
    {
        format(string, sizeof(string), "[MenuID]: %d - [ItemID]: %d - [ItemName]: %s", menuid, itemid, itemname);
        SendClientMessage(playerid, -1, string);
    }
    
    // switch case
    switch(menuid)
    {
        case MENU_AMMUSTORE:
        {
            format(string, sizeof(string), "[MenuID]: %d - [ItemID]: %d - [ItemName]: %s", menuid, itemid, itemname);
            SendClientMessage(playerid, -1, string);
        }
    }
    return 1;
}
```
