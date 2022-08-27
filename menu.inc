#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

#define     MAX_PAGE_ITEM       10
#define     MAX_MENU_ITEM       500

#define     COLOR_ROW_NORMAL    276422143
#define     COLOR_ROW_CLICKED   -357140737

forward OnResponseMenu(playerid, menuid, itemid, const itemname[]);

enum // Add new type for amount right here
{
    m_TypePrice,
    m_TypeAmount
}

static
    Text:TD_Menu[11],
    PlayerText:PTD_Menu_Item_BG[MAX_PLAYERS][MAX_PAGE_ITEM],
    PlayerText:PTD_Menu_Item_SBG[MAX_PLAYERS][MAX_PAGE_ITEM],
    PlayerText:PTD_Menu_Item_Name[MAX_PLAYERS][MAX_PAGE_ITEM],
    PlayerText:PTD_Menu_Item_Amount[MAX_PLAYERS][MAX_PAGE_ITEM],
    PlayerText:PTD_Menu[MAX_PLAYERS][2],
    Iterator:m_Item[MAX_PLAYERS]<MAX_MENU_ITEM>,
    bool:PTD_Menu_Item_Opened[MAX_PLAYERS][MAX_PAGE_ITEM],
    m_MenuID[MAX_PLAYERS],
    m_Item_Amount[MAX_PLAYERS][MAX_MENU_ITEM],
    m_Item_Type[MAX_PLAYERS][MAX_MENU_ITEM],
    m_Item_Name[MAX_PLAYERS][MAX_MENU_ITEM][24],
    // Click (Double Click)
    m_Item_SelectedTick[MAX_PLAYERS],
    m_Item_SelectedTickRow[MAX_PLAYERS],
    m_Item_Selected[MAX_PLAYERS];

const
    Float:szMenu = 19.359070,
    Float:szMenuBG = 116.726302,
    Float:szMenuSBG = 118.518783,
    Float:szMenuName = 120.707610,
    Float:szMenuAmount = 121.707671;

stock Menu_Show(playerid, menuid, const title[])
{
    Menu_FixSync(playerid, 1);

    // Background Menu
    for(new i = 0; i < 11; i++)
        TextDrawShowForPlayer(playerid, TD_Menu[i]);

    // Title Menu
    PlayerTextDrawSetString(playerid, PTD_Menu[playerid][0], title);
    PlayerTextDrawShow(playerid, PTD_Menu[playerid][0]);
    
    // Page Menu
    m_MenuID[playerid] = menuid;
    SetPVarInt(playerid, "menu.Opened", 1);
    SetPVarInt(playerid, "menu.Page", 1);
    Menu_SetPage(playerid, GetPVarInt(playerid, "menu.Page"));
    return 1;
}

stock Menu_AddItem(playerid, const name[], type = m_TypePrice | m_TypeAmount, amount)
{
    new  index = Iter_Free(m_Item[playerid]);

    if(-1 < index < MAX_MENU_ITEM)
    {
        strcat((m_Item_Name[playerid][index][0] = EOS, m_Item_Name[playerid][index]), name);
        m_Item_Type[playerid][index] = type;
        m_Item_Amount[playerid][index] = amount;
        Iter_Add(m_Item[playerid], index);
    }
    return 1;
}

stock IsMenuOpened(playerid)
{
    return GetPVarInt(playerid, "menu.Opened");
}

static stock Menu_FixSync(playerid, type)
{
    if(type == 1)
    {
        for(new i = 0; i < 11; i++)
            TextDrawHideForPlayer(playerid, TD_Menu[i]);
        PlayerTextDrawHide(playerid, PTD_Menu[playerid][0]);
    }

    for(new i = 0; i < 10; i++)
    {
        PTD_Menu_Item_Opened[playerid][i] = false;
        PlayerTextDrawHide(playerid, PTD_Menu_Item_BG[playerid][i]);
        PlayerTextDrawHide(playerid, PTD_Menu_Item_SBG[playerid][i]);
        PlayerTextDrawHide(playerid, PTD_Menu_Item_Name[playerid][i]);
        PlayerTextDrawHide(playerid, PTD_Menu_Item_Amount[playerid][i]);
    }
    PlayerTextDrawHide(playerid, PTD_Menu[playerid][1]);
    return 1;
}

static stock Menu_CreatePTD(playerid)
{
    // Header
    PTD_Menu[playerid][0] = CreatePlayerTextDraw(playerid, 152.917465, 95.629539, "_"); // Menu Title
    PlayerTextDrawLetterSize(playerid, PTD_Menu[playerid][0], 0.458332, 2.009628);
    PlayerTextDrawAlignment(playerid, PTD_Menu[playerid][0], 2);
    PlayerTextDrawColor(playerid, PTD_Menu[playerid][0], -1);
    PlayerTextDrawSetShadow(playerid, PTD_Menu[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, PTD_Menu[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, PTD_Menu[playerid][0], 255);
    PlayerTextDrawFont(playerid, PTD_Menu[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, PTD_Menu[playerid][0], 1);
    PlayerTextDrawSetShadow(playerid, PTD_Menu[playerid][0], 0);
    PlayerTextDrawShow(playerid, PTD_Menu[playerid][0]);

    PTD_Menu[playerid][1] = CreatePlayerTextDraw(playerid, 226.450988, 126.429840, "_"); // Page Number
    PlayerTextDrawLetterSize(playerid, PTD_Menu[playerid][1], 0.188997, 0.852962);
    PlayerTextDrawAlignment(playerid, PTD_Menu[playerid][1], 3);
    PlayerTextDrawColor(playerid, PTD_Menu[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, PTD_Menu[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, PTD_Menu[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, PTD_Menu[playerid][1], 255);
    PlayerTextDrawFont(playerid, PTD_Menu[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, PTD_Menu[playerid][1], 1);
    PlayerTextDrawSetShadow(playerid, PTD_Menu[playerid][1], 0);

    // Body
    for(new i = 0; i < 10; i++)
    {
        PTD_Menu_Item_BG[playerid][i] = CreatePlayerTextDraw(playerid, 76.666687, szMenuBG + (szMenu * (i + 1)), "LD_SPAC:white"); // Background
        PlayerTextDrawLetterSize(playerid, PTD_Menu_Item_BG[playerid][i], 0.000000, 0.000000);
        PlayerTextDrawTextSize(playerid, PTD_Menu_Item_BG[playerid][i], 151.940689, 18.070024);
        PlayerTextDrawAlignment(playerid, PTD_Menu_Item_BG[playerid][i], 1);
        PlayerTextDrawColor(playerid, PTD_Menu_Item_BG[playerid][i], 1179076095);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_BG[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, PTD_Menu_Item_BG[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, PTD_Menu_Item_BG[playerid][i], 255);
        PlayerTextDrawFont(playerid, PTD_Menu_Item_BG[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, PTD_Menu_Item_BG[playerid][i], 0);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_BG[playerid][i], 0);

        PTD_Menu_Item_SBG[playerid][i] = CreatePlayerTextDraw(playerid, 77.366645, szMenuSBG + (szMenu * (i + 1)), "LD_SPAC:white"); // Second Background (For click)
        PlayerTextDrawLetterSize(playerid, PTD_Menu_Item_SBG[playerid][i], 0.000000, 0.000000);
        PlayerTextDrawTextSize(playerid, PTD_Menu_Item_SBG[playerid][i], 149.729705, 14.130107);
        PlayerTextDrawAlignment(playerid, PTD_Menu_Item_SBG[playerid][i], 1);
        PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][i], COLOR_ROW_NORMAL);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_SBG[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, PTD_Menu_Item_SBG[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, PTD_Menu_Item_SBG[playerid][i], 255);
        PlayerTextDrawFont(playerid, PTD_Menu_Item_SBG[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, PTD_Menu_Item_SBG[playerid][i], 0);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_SBG[playerid][i], 0);
        PlayerTextDrawSetSelectable(playerid, PTD_Menu_Item_SBG[playerid][i], true);

        PTD_Menu_Item_Name[playerid][i] = CreatePlayerTextDraw(playerid, 80.016685, szMenuName + (szMenu * (i + 1)), "_"); // Item Name
        PlayerTextDrawLetterSize(playerid, PTD_Menu_Item_Name[playerid][i], 0.253333, 1.060739);
        PlayerTextDrawAlignment(playerid, PTD_Menu_Item_Name[playerid][i], 1);
        PlayerTextDrawColor(playerid, PTD_Menu_Item_Name[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_Name[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, PTD_Menu_Item_Name[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, PTD_Menu_Item_Name[playerid][i], 255);
        PlayerTextDrawFont(playerid, PTD_Menu_Item_Name[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, PTD_Menu_Item_Name[playerid][i], 1);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_Name[playerid][i], 0);

        PTD_Menu_Item_Amount[playerid][i] = CreatePlayerTextDraw(playerid, 224.601196, szMenuAmount + (szMenu * (i + 1)), "_"); // Item Amount
        PlayerTextDrawLetterSize(playerid, PTD_Menu_Item_Amount[playerid][i], 0.219330, 0.820738);
        PlayerTextDrawAlignment(playerid, PTD_Menu_Item_Amount[playerid][i], 3);
        PlayerTextDrawColor(playerid, PTD_Menu_Item_Amount[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_Amount[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, PTD_Menu_Item_Amount[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, PTD_Menu_Item_Amount[playerid][i], 255);
        PlayerTextDrawFont(playerid, PTD_Menu_Item_Amount[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, PTD_Menu_Item_Amount[playerid][i], 1);
        PlayerTextDrawSetShadow(playerid, PTD_Menu_Item_Amount[playerid][i], 0);
    }
    return 1;
}

static stock Menu_SetPage(playerid, page)
{
    Menu_FixSync(playerid, 0);

    new start, end, string[128];
    Menu_SetPosStartEnd(playerid, page, start, end);
    
    for(new i = start; i < end; i++)
    {
        switch(m_Item_Type[playerid][i])
        {
            case m_TypeAmount:
                format(string, sizeof(string), "%s", number_format(m_Item_Amount[playerid][i]));
            case m_TypePrice:
                format(string, sizeof(string), "$%s", number_format(m_Item_Amount[playerid][i]));
        }
        PlayerTextDrawSetString(playerid, PTD_Menu_Item_Name[playerid][i % 10], m_Item_Name[playerid][i]);
        PlayerTextDrawSetString(playerid, PTD_Menu_Item_Amount[playerid][i % 10], string);

        if(m_Item_Selected[playerid] == i)
            PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][i % 10], COLOR_ROW_CLICKED);
        else
            PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][i % 10], COLOR_ROW_NORMAL);

        PTD_Menu_Item_Opened[playerid][i % 10] = true;
        PlayerTextDrawShow(playerid, PTD_Menu_Item_BG[playerid][i % 10]);
        PlayerTextDrawShow(playerid, PTD_Menu_Item_SBG[playerid][i % 10]);
        PlayerTextDrawShow(playerid, PTD_Menu_Item_Name[playerid][i % 10]);
        PlayerTextDrawShow(playerid, PTD_Menu_Item_Amount[playerid][i % 10]);
    }
    Menu_SetStringPage(playerid);
    SelectTextDraw(playerid, 0x33CCFFAA);
    return 1;
}

static stock Menu_SetStringPage(playerid)
{
    new page[11];
    format(page, sizeof(page), "%d/%d", GetPVarInt(playerid, "menu.Page"), Menu_TotalPage(playerid));
    PlayerTextDrawSetString(playerid, PTD_Menu[playerid][1], page);
    PlayerTextDrawShow(playerid, PTD_Menu[playerid][1]);
    return 1;
}

static stock Menu_TotalPage(playerid)
{
    return floatround(Iter_Count(m_Item[playerid]) % MAX_PAGE_ITEM == 0 ? Iter_Count(m_Item[playerid]) / MAX_PAGE_ITEM : Iter_Count(m_Item[playerid]) / MAX_PAGE_ITEM + 1);
}

static stock Menu_SetPosStartEnd(playerid, page, &start, &end)
{
    new
        item = Iter_Count(m_Item[playerid]);
        
    start = MAX_PAGE_ITEM * (page - 1);
    end = start + MAX_PAGE_ITEM;

    if (item % end == item)
        end = start + item % MAX_PAGE_ITEM;
    return 1;
}

static stock Menu_Hide(playerid)
{
    for(new i = 0; i < 11; i++)
        TextDrawHideForPlayer(playerid, TD_Menu[i]);

    for(new i = 0; i < 10; i++)
    {
        PTD_Menu_Item_Opened[playerid][i] = false;
        PlayerTextDrawHide(playerid, PTD_Menu_Item_BG[playerid][i]);
        PlayerTextDrawHide(playerid, PTD_Menu_Item_SBG[playerid][i]);
        PlayerTextDrawHide(playerid, PTD_Menu_Item_Name[playerid][i]);
        PlayerTextDrawHide(playerid, PTD_Menu_Item_Amount[playerid][i]);
    }
    PlayerTextDrawHide(playerid, PTD_Menu[playerid][0]);
    PlayerTextDrawHide(playerid, PTD_Menu[playerid][1]);
    
    Menu_ResetVariables(playerid);
    CancelSelectTextDraw(playerid);
    return 1;
}

hook OnGameModeInit()
{
    Iter_Init(m_Item);

    TD_Menu[0] = TextDrawCreate(75.416641, 83.444389, "LD_SPAC:white");
    TextDrawLetterSize(TD_Menu[0], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[0], 153.940689, 266.080078);
    TextDrawAlignment(TD_Menu[0], 1);
    TextDrawColor(TD_Menu[0], 255);
    TextDrawSetShadow(TD_Menu[0], 0);
    TextDrawSetOutline(TD_Menu[0], 0);
    TextDrawBackgroundColor(TD_Menu[0], 255);
    TextDrawFont(TD_Menu[0], 4);
    TextDrawSetProportional(TD_Menu[0], 0);
    TextDrawSetShadow(TD_Menu[0], 0);

    TD_Menu[0] = TextDrawCreate(75.416641, 83.444389, "LD_SPAC:white");
    TextDrawLetterSize(TD_Menu[0], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[0], 153.940689, 266.080078);
    TextDrawAlignment(TD_Menu[0], 1);
    TextDrawColor(TD_Menu[0], 255);
    TextDrawSetShadow(TD_Menu[0], 0);
    TextDrawSetOutline(TD_Menu[0], 0);
    TextDrawBackgroundColor(TD_Menu[0], 255);
    TextDrawFont(TD_Menu[0], 4);
    TextDrawSetProportional(TD_Menu[0], 0);
    TextDrawSetShadow(TD_Menu[0], 0);

    TD_Menu[1] = TextDrawCreate(76.416625, 84.744392, "LD_SPAC:white");
    TextDrawLetterSize(TD_Menu[1], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[1], 151.931793, 40.079986);
    TextDrawAlignment(TD_Menu[1], 1);
    TextDrawColor(TD_Menu[1], 276422143);
    TextDrawSetShadow(TD_Menu[1], 0);
    TextDrawSetOutline(TD_Menu[1], 0);
    TextDrawBackgroundColor(TD_Menu[1], 255);
    TextDrawFont(TD_Menu[1], 4);
    TextDrawSetProportional(TD_Menu[1], 0);
    TextDrawSetShadow(TD_Menu[1], 0);

    TD_Menu[2] = TextDrawCreate(116.066551, 330.012969, "LD_SPAC:white");
    TextDrawLetterSize(TD_Menu[2], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[2], 20.000000, 17.990022);
    TextDrawAlignment(TD_Menu[2], 1);
    TextDrawColor(TD_Menu[2], 1179076095);
    TextDrawSetShadow(TD_Menu[2], 0);
    TextDrawSetOutline(TD_Menu[2], 0);
    TextDrawBackgroundColor(TD_Menu[2], 255);
    TextDrawFont(TD_Menu[2], 4);
    TextDrawSetProportional(TD_Menu[2], 0);
    TextDrawSetShadow(TD_Menu[2], 0);

    TD_Menu[3] = TextDrawCreate(117.166511, 331.805450, "LD_SPAC:white"); // Back Page (For Click)
    TextDrawLetterSize(TD_Menu[3], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[3], 17.929998, 14.050107);
    TextDrawAlignment(TD_Menu[3], 1);
    TextDrawColor(TD_Menu[3], 276422143);
    TextDrawSetShadow(TD_Menu[3], 0);
    TextDrawSetOutline(TD_Menu[3], 0);
    TextDrawBackgroundColor(TD_Menu[3], 255);
    TextDrawFont(TD_Menu[3], 4);
    TextDrawSetProportional(TD_Menu[3], 0);
    TextDrawSetShadow(TD_Menu[3], 0);
    TextDrawSetSelectable(TD_Menu[3], true);

    TD_Menu[4] = TextDrawCreate(120.100082, 333.044433, "<<");
    TextDrawLetterSize(TD_Menu[4], 0.253333, 1.330736);
    TextDrawAlignment(TD_Menu[4], 1);
    TextDrawColor(TD_Menu[4], -1);
    TextDrawSetShadow(TD_Menu[4], 0);
    TextDrawSetOutline(TD_Menu[4], 0);
    TextDrawBackgroundColor(TD_Menu[4], 255);
    TextDrawFont(TD_Menu[4], 1);
    TextDrawSetProportional(TD_Menu[4], 1);
    TextDrawSetShadow(TD_Menu[4], 0);

    TD_Menu[5] = TextDrawCreate(137.066589, 330.012969, "LD_SPAC:white");
    TextDrawLetterSize(TD_Menu[5], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[5], 20.000000, 17.990022);
    TextDrawAlignment(TD_Menu[5], 1);
    TextDrawColor(TD_Menu[5], 1179076095);
    TextDrawSetShadow(TD_Menu[5], 0);
    TextDrawSetOutline(TD_Menu[5], 0);
    TextDrawBackgroundColor(TD_Menu[5], 255);
    TextDrawFont(TD_Menu[5], 4);
    TextDrawSetProportional(TD_Menu[5], 0);
    TextDrawSetShadow(TD_Menu[5], 0);

    TD_Menu[6] = TextDrawCreate(138.066650, 331.805450, "LD_SPAC:white"); // Next Page (For Click)
    TextDrawLetterSize(TD_Menu[6], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[6], 17.929998, 14.050107);
    TextDrawAlignment(TD_Menu[6], 1);
    TextDrawColor(TD_Menu[6], 276422143);
    TextDrawSetShadow(TD_Menu[6], 0);
    TextDrawSetOutline(TD_Menu[6], 0);
    TextDrawBackgroundColor(TD_Menu[6], 255);
    TextDrawFont(TD_Menu[6], 4);
    TextDrawSetProportional(TD_Menu[6], 0);
    TextDrawSetShadow(TD_Menu[6], 0);
    TextDrawSetSelectable(TD_Menu[6], true);

    TD_Menu[7] = TextDrawCreate(141.300125, 333.044342, ">>");
    TextDrawLetterSize(TD_Menu[7], 0.253333, 1.330736);
    TextDrawAlignment(TD_Menu[7], 1);
    TextDrawColor(TD_Menu[7], -1);
    TextDrawSetShadow(TD_Menu[7], 0);
    TextDrawSetOutline(TD_Menu[7], 0);
    TextDrawBackgroundColor(TD_Menu[7], 255);
    TextDrawFont(TD_Menu[7], 1);
    TextDrawSetProportional(TD_Menu[7], 1);
    TextDrawSetShadow(TD_Menu[7], 0);

    TD_Menu[8] = TextDrawCreate(157.867874, 330.012969, "LD_SPAC:white");
    TextDrawLetterSize(TD_Menu[8], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[8], 30.000000, 17.990022);
    TextDrawAlignment(TD_Menu[8], 1);
    TextDrawColor(TD_Menu[8], 1179076095);
    TextDrawSetShadow(TD_Menu[8], 0);
    TextDrawSetOutline(TD_Menu[8], 0);
    TextDrawBackgroundColor(TD_Menu[8], 255);
    TextDrawFont(TD_Menu[8], 4);
    TextDrawSetProportional(TD_Menu[8], 0);
    TextDrawSetShadow(TD_Menu[8], 0);

    TD_Menu[9] = TextDrawCreate(159.067932, 331.805450, "LD_SPAC:white"); // Close Menu (For Click)
    TextDrawLetterSize(TD_Menu[9], 0.000000, 0.000000);
    TextDrawTextSize(TD_Menu[9], 27.329984, 14.050107);
    TextDrawAlignment(TD_Menu[9], 1);
    TextDrawColor(TD_Menu[9], 276422143);
    TextDrawSetShadow(TD_Menu[9], 0);
    TextDrawSetOutline(TD_Menu[9], 0);
    TextDrawBackgroundColor(TD_Menu[9], 255);
    TextDrawFont(TD_Menu[9], 4);
    TextDrawSetProportional(TD_Menu[9], 0);
    TextDrawSetShadow(TD_Menu[9], 0);
    TextDrawSetSelectable(TD_Menu[9], true);

    TD_Menu[10] = TextDrawCreate(172.600753, 331.644256, "Dong");
    TextDrawLetterSize(TD_Menu[10], 0.253333, 1.330736);
    TextDrawAlignment(TD_Menu[10], 2);
    TextDrawColor(TD_Menu[10], -1);
    TextDrawSetShadow(TD_Menu[10], 0);
    TextDrawSetOutline(TD_Menu[10], 0);
    TextDrawBackgroundColor(TD_Menu[10], 255);
    TextDrawFont(TD_Menu[10], 1);
    TextDrawSetProportional(TD_Menu[10], 1);
    TextDrawSetShadow(TD_Menu[10], 0);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    Menu_ResetVariables(playerid);
    Menu_CreatePTD(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid)
{
    Menu_ResetVariables(playerid);

    for(new i = 0; i < 2; i++)
    {
        if(PTD_Menu[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, PTD_Menu[playerid][i]);
            PTD_Menu[playerid][i] = PlayerText:-1;
        }
    }

    for(new i = 0; i < MAX_PAGE_ITEM; i++)
    {
        if(PTD_Menu_Item_BG[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, PTD_Menu_Item_BG[playerid][i]);
            PTD_Menu_Item_BG[playerid][i] = PlayerText:-1;
        }
        if(PTD_Menu_Item_SBG[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, PTD_Menu_Item_SBG[playerid][i]);
            PTD_Menu_Item_SBG[playerid][i] = PlayerText:-1;
        }
        if(PTD_Menu_Item_Name[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, PTD_Menu_Item_Name[playerid][i]);
            PTD_Menu_Item_Name[playerid][i] = PlayerText:-1;
        }
        if(PTD_Menu_Item_Amount[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, PTD_Menu_Item_Amount[playerid][i]);
            PTD_Menu_Item_Amount[playerid][i] = PlayerText:-1;
        }
    }
    return 1;
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid)
{
    if(IsMenuOpened(playerid))
    {
        new start, end;
        Menu_SetPosStartEnd(playerid, GetPVarInt(playerid, "menu.Page"), start, end);
        
        for(new i = start; i < end; i++)
        {
            if(playertextid == PTD_Menu_Item_SBG[playerid][i % 10])
            {
                if((GetTickCount() - m_Item_SelectedTick[playerid]) < 300 && m_Item_SelectedTickRow[playerid] == i)
                {
                    CallRemoteFunction("OnResponseMenu", "ddds[24]", playerid, m_MenuID[playerid], i, m_Item_Name[playerid][i]);
                    CancelSelectTextDraw(playerid);
                    Menu_ResetVariables(playerid);
                    return 1;
                }
                else
                {
                    m_Item_SelectedTickRow[playerid] = i;
					m_Item_SelectedTick[playerid] = GetTickCount();
                }
				if(m_Item_Selected[playerid] == -1)
                {
                    PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][i % 10], COLOR_ROW_CLICKED);
                    PlayerTextDrawShow(playerid, PTD_Menu_Item_SBG[playerid][i % 10]);
                }
                else if(m_Item_Selected[playerid] != i)
                {
                    if(PTD_Menu_Item_Opened[playerid][m_Item_Selected[playerid] % 10])
                        PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][m_Item_Selected[playerid] % 10], COLOR_ROW_NORMAL),
                        PlayerTextDrawShow(playerid, PTD_Menu_Item_SBG[playerid][m_Item_Selected[playerid] % 10]);
					PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][i % 10], COLOR_ROW_CLICKED);
                    PlayerTextDrawShow(playerid, PTD_Menu_Item_SBG[playerid][i % 10]);
                }
                else
				{
                    m_Item_Selected[playerid] = -1;
					PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][i % 10], COLOR_ROW_NORMAL);
                    PlayerTextDrawShow(playerid, PTD_Menu_Item_SBG[playerid][i % 10]);
					return 1;
				}
                m_Item_Selected[playerid] = i;
                break;
            }
        }
    }
    return 1;
}

hook OnPlayerClickTD(playerid, Text:clickedid)
{
    if(IsMenuOpened(playerid))
    {
		if(clickedid == TD_Menu[3])
		{
			if(GetPVarInt(playerid, "menu.Page") > 1)
			{
                SetPVarInt(playerid, "menu.Page", GetPVarInt(playerid, "menu.Page")-1);
				Menu_SetPage(playerid, GetPVarInt(playerid, "menu.Page"));
                Menu_SetStringPage(playerid);
			}
		}
        else if(clickedid == TD_Menu[6])
		{
			if(GetPVarInt(playerid, "menu.Page") < Menu_TotalPage(playerid))
			{
				SetPVarInt(playerid, "menu.Page", GetPVarInt(playerid, "menu.Page")+1);
				Menu_SetPage(playerid, GetPVarInt(playerid, "menu.Page"));
                Menu_SetStringPage(playerid);
			}
		}
        else if(clickedid == TD_Menu[9] || clickedid == Text:INVALID_TEXT_DRAW)
		{
            Menu_ResetVariables(playerid);
            CancelSelectTextDraw(playerid);      
		}
    }
    return 1;
}

static stock Menu_ResetVariables(playerid)
{
    Menu_FixSync(playerid, 1);
    DeletePVar(playerid, "menu.Opened");
    DeletePVar(playerid, "menu.Page");
    Iter_Clear(m_Item[playerid]);
    if(m_Item_Selected[playerid] != -1)
    {
        for(new i = 0; i < MAX_PAGE_ITEM; i++)
        {
            PTD_Menu_Item_Opened[playerid][i] = false;
            PlayerTextDrawColor(playerid, PTD_Menu_Item_SBG[playerid][m_Item_Selected[playerid] % 10], COLOR_ROW_NORMAL);
        }
    }
    m_MenuID[playerid] = -1;
    m_Item_SelectedTick[playerid] = 0;
    m_Item_SelectedTickRow[playerid] = 0;
    m_Item_Selected[playerid] = -1;
    for(new i = 0; i < MAX_MENU_ITEM; i++)
    {
        m_Item_Name[playerid][i][0] = EOS;
        m_Item_Amount[playerid][i] = 0;
        m_Item_Type[playerid][i] = -1;
    }
    return 1;
}

static number_format(number)
{
	new i, string[15];
	FIXES_valstr(string, number);
	if(strfind(string, "-") != -1) i = strlen(string) - 4;
	else i = strlen(string) - 3;
	while (i >= 1)
 	{
		if(strfind(string, "-") != -1) strins(string, ",", i + 1);
		else strins(string, ",", i);
		i -= 3;
	}
	return string;
}

static FIXES_valstr(dest[], value, bool:pack = false)
{
    // format can't handle cellmin properly
    static const cellmin_value[] = !"-2147483648";

    if (value == cellmin)
        pack && strpack(dest, cellmin_value, 12) || strunpack(dest, cellmin_value, 12);
    else
        format(dest, 12, "%d", value) && pack && strpack(dest, dest, 12);
}