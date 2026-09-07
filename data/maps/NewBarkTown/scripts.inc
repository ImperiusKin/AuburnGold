NewBarkTown_MapScripts::
	.byte 0


NewBarkTown_EventScript_Youngerster1::
# 6 "data/maps/NewBarkTown/scripts.pory"
	lock
# 7 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 8 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster1_Text_0
# 9 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Youngerster2::
# 13 "data/maps/NewBarkTown/scripts.pory"
	lock
# 14 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 15 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster2_Text_0
# 16 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Youngerster3::
# 20 "data/maps/NewBarkTown/scripts.pory"
	lock
# 21 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 22 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster3_Text_0
# 23 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Youngerster4::
# 27 "data/maps/NewBarkTown/scripts.pory"
	lock
# 28 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 29 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster4_Text_0
# 30 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Rival::
# 35 "data/maps/NewBarkTown/scripts.pory"
	lock
# 36 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 37 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_0
# 38 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 39 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_EventScript_Rival_Movement_0
# 43 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 44 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_RIVAL, NewBarkTown_EventScript_Rival_Movement_1
# 47 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 48 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_TEMP_1
# 49 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_1
# 50 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 52 "data/maps/NewBarkTown/scripts.pory"
	playse SE_PIN
# 53 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_ELM, Common_Movement_ExclamationMark
# 54 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 56 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_ELM, NewBarkTown_EventScript_Rival_Movement_2
# 60 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 62 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_2
# 63 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_3
# 64 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_4
# 65 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_5
# 66 "data/maps/NewBarkTown/scripts.pory"
	giveitem ITEM_POKE_BALL
# 67 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ChooseStarter
# 68 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_ChooseStarter::
# 72 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_0
# 74 "data/maps/NewBarkTown/scripts.pory"
	dynmultichoice 0, 0, TRUE, 6, 0, DYN_MULTICHOICE_CB_NONE, NewBarkTown_EventScript_ChooseStarter_Text_1, NewBarkTown_EventScript_ChooseStarter_Text_2, NewBarkTown_EventScript_ChooseStarter_Text_3, NewBarkTown_EventScript_ChooseStarter_Text_4, NewBarkTown_EventScript_ChooseStarter_Text_5, NewBarkTown_EventScript_ChooseStarter_Text_6, NewBarkTown_EventScript_ChooseStarter_Text_7, NewBarkTown_EventScript_ChooseStarter_Text_8, NewBarkTown_EventScript_ChooseStarter_Text_9, NewBarkTown_EventScript_ChooseStarter_Text_10, NewBarkTown_EventScript_ChooseStarter_Text_11, NewBarkTown_EventScript_ChooseStarter_Text_12, NewBarkTown_EventScript_ChooseStarter_Text_13, NewBarkTown_EventScript_ChooseStarter_Text_14, NewBarkTown_EventScript_ChooseStarter_Text_15, NewBarkTown_EventScript_ChooseStarter_Text_16, NewBarkTown_EventScript_ChooseStarter_Text_17, NewBarkTown_EventScript_ChooseStarter_Text_18, NewBarkTown_EventScript_ChooseStarter_Text_19, NewBarkTown_EventScript_ChooseStarter_Text_20, NewBarkTown_EventScript_ChooseStarter_Text_21, NewBarkTown_EventScript_ChooseStarter_Text_22, NewBarkTown_EventScript_ChooseStarter_Text_23, NewBarkTown_EventScript_ChooseStarter_Text_24, NewBarkTown_EventScript_ChooseStarter_Text_25, NewBarkTown_EventScript_ChooseStarter_Text_26, NewBarkTown_EventScript_ChooseStarter_Text_27, NewBarkTown_EventScript_ChooseStarter_Text_28, NewBarkTown_EventScript_ChooseStarter_Text_29, NewBarkTown_EventScript_ChooseStarter_Text_30
# 76 "data/maps/NewBarkTown/scripts.pory"
	showmonpic SPECIES_NONE, 10, 3
# 77 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_31, MSGBOX_YESNO
# 78 "data/maps/NewBarkTown/scripts.pory"
	hidemonpic
# 79 "data/maps/NewBarkTown/scripts.pory"
	compare VAR_RESULT, NO
	goto_if_eq NewBarkTown_EventScript_ChooseStarter_2
NewBarkTown_EventScript_ChooseStarter_1:
# 82 "data/maps/NewBarkTown/scripts.pory"
	givemon SPECIES_NONE, 5, ITEM_ORAN_BERRY
# 83 "data/maps/NewBarkTown/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 84 "data/maps/NewBarkTown/scripts.pory"
	waitfanfare
# 85 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_32, MSGBOX_DEFAULT
# 86 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 87 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ElmPokedex
# 88 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_ChooseStarter_2:
# 80 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ChooseStarter
	goto NewBarkTown_EventScript_ChooseStarter_1


NewBarkTown_EventScript_ElmPokedex::
# 92 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_0
# 93 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 94 "data/maps/NewBarkTown/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 95 "data/maps/NewBarkTown/scripts.pory"
	waitfanfare
# 96 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_1, MSGBOX_DEFAULT
# 97 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_2
# 98 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_3
# 99 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_ADVENTURE_STARTED
# 100 "data/maps/NewBarkTown/scripts.pory"
	warp MAP_NEW_BARK_TOWN, 22, 15
# 101 "data/maps/NewBarkTown/scripts.pory"
	release
	return


# 39 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_0:
# 40 "data/maps/NewBarkTown/scripts.pory"
	walk_left
# 41 "data/maps/NewBarkTown/scripts.pory"
	face_right
	step_end

# 44 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_1:
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 45 "data/maps/NewBarkTown/scripts.pory"
	walk_down
	step_end

# 56 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_2:
# 57 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 57 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 58 "data/maps/NewBarkTown/scripts.pory"
	face_left
	step_end

NewBarkTown_EventScript_Youngerster1_Text_0:
# 8 "data/maps/NewBarkTown/scripts.pory"
	.string "I heard the Cinnabar Gym Leader picked\n"
	.string "an actual active volcano. An active one.\l"
	.string "I don't know if I signed up for this...$"

NewBarkTown_EventScript_Youngerster2_Text_0:
# 15 "data/maps/NewBarkTown/scripts.pory"
	.string "Gym Leaders with 'favorable\n"
	.string "environments'... that sounds like a nice\l"
	.string "way of saying 'unfair advantage,' if you\l"
	.string "ask me.$"

NewBarkTown_EventScript_Youngerster3_Text_0:
# 22 "data/maps/NewBarkTown/scripts.pory"
	.string "This is going to be the adventure of a\n"
	.string "lifetime. I can't wait!$"

NewBarkTown_EventScript_Youngerster4_Text_0:
# 29 "data/maps/NewBarkTown/scripts.pory"
	.string "Still, this is going to take most of us\n"
	.string "years, not months. Hope everyone's\l"
	.string "ready for the long haul.$"

NewBarkTown_EventScript_Rival_Text_0:
# 37 "data/maps/NewBarkTown/scripts.pory"
	.string "So you're entering too, huh?. I already\n"
	.string "got my trainer supply kit and my\l"
	.string "starter so I will begin my journey now!$"

NewBarkTown_EventScript_Rival_Text_1:
# 49 "data/maps/NewBarkTown/scripts.pory"
	.string "Next time I see you make sure to be\n"
	.string "ready since I'm going to challenge you\l"
	.string "to a battle!$"

NewBarkTown_EventScript_Rival_Text_2:
# 62 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} you're here too!, I was just\n"
	.string "explaining the others on how the\l"
	.string "tournament will work, it has some\l"
	.string "changes from past tournaments so make\l"
	.string "sure to pay proper attention:$"

NewBarkTown_EventScript_Rival_Text_3:
# 63 "data/maps/NewBarkTown/scripts.pory"
	.string "-Eligibility for Champions league\n"
	.string "requires completion of Kanto and\l"
	.string "Rematches of visiting gym leaders now\l"
	.string "in Kanto.$"

NewBarkTown_EventScript_Rival_Text_4:
# 64 "data/maps/NewBarkTown/scripts.pory"
	.string "-Explains each gym leader has been\n"
	.string "given favorable environments to add to\l"
	.string "the challenge.$"

NewBarkTown_EventScript_Rival_Text_5:
# 65 "data/maps/NewBarkTown/scripts.pory"
	.string "That should be all, pretty simple right?,\n"
	.string "for now take this.$"

NewBarkTown_EventScript_ChooseStarter_Text_0:
# 72 "data/maps/NewBarkTown/scripts.pory"
	.string "Please choose your partner Pokémon,\n"
	.string "make sure to choose carefully.$"

NewBarkTown_EventScript_ChooseStarter_Text_1:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Bulbasaur$"

NewBarkTown_EventScript_ChooseStarter_Text_2:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Charmander$"

NewBarkTown_EventScript_ChooseStarter_Text_3:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Squirtle$"

NewBarkTown_EventScript_ChooseStarter_Text_4:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Chikorita$"

NewBarkTown_EventScript_ChooseStarter_Text_5:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Cyndaquil$"

NewBarkTown_EventScript_ChooseStarter_Text_6:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Totodile$"

NewBarkTown_EventScript_ChooseStarter_Text_7:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Treecko$"

NewBarkTown_EventScript_ChooseStarter_Text_8:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Torchic$"

NewBarkTown_EventScript_ChooseStarter_Text_9:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Mudkip$"

NewBarkTown_EventScript_ChooseStarter_Text_10:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Turtwig$"

NewBarkTown_EventScript_ChooseStarter_Text_11:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Chimchar$"

NewBarkTown_EventScript_ChooseStarter_Text_12:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Piplup$"

NewBarkTown_EventScript_ChooseStarter_Text_13:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Snivy$"

NewBarkTown_EventScript_ChooseStarter_Text_14:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Tepig$"

NewBarkTown_EventScript_ChooseStarter_Text_15:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Oshawott$"

NewBarkTown_EventScript_ChooseStarter_Text_16:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Chespin$"

NewBarkTown_EventScript_ChooseStarter_Text_17:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Fennekin$"

NewBarkTown_EventScript_ChooseStarter_Text_18:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Froakie$"

NewBarkTown_EventScript_ChooseStarter_Text_19:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Rowlet$"

NewBarkTown_EventScript_ChooseStarter_Text_20:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Litten$"

NewBarkTown_EventScript_ChooseStarter_Text_21:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Popplio$"

NewBarkTown_EventScript_ChooseStarter_Text_22:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Grookey$"

NewBarkTown_EventScript_ChooseStarter_Text_23:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Scorbunny$"

NewBarkTown_EventScript_ChooseStarter_Text_24:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Sobble$"

NewBarkTown_EventScript_ChooseStarter_Text_25:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Sprigatito$"

NewBarkTown_EventScript_ChooseStarter_Text_26:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Fuecoco$"

NewBarkTown_EventScript_ChooseStarter_Text_27:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Quaxly$"

NewBarkTown_EventScript_ChooseStarter_Text_28:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Pikachu$"

NewBarkTown_EventScript_ChooseStarter_Text_29:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Eevee$"

NewBarkTown_EventScript_ChooseStarter_Text_30:
# 74 "data/maps/NewBarkTown/scripts.pory"
	.string "Riolu$"

NewBarkTown_EventScript_ChooseStarter_Text_31:
# 77 "data/maps/NewBarkTown/scripts.pory"
	.string "Do you want the {STR_VAR_2}\n"
	.string "Pokémon {STR_VAR_1} as your first\l"
	.string "partner?$"

NewBarkTown_EventScript_ChooseStarter_Text_32:
# 85 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} received a {STR_VAR_1}!$"

NewBarkTown_EventScript_ElmPokedex_Text_0:
# 92 "data/maps/NewBarkTown/scripts.pory"
	.string "And finally here is a Pokédex, it can\n"
	.string "capture all the data of Pokémon you see\l"
	.string "or catch!$"

NewBarkTown_EventScript_ElmPokedex_Text_1:
# 96 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} received the Pokédex!$"

NewBarkTown_EventScript_ElmPokedex_Text_2:
# 97 "data/maps/NewBarkTown/scripts.pory"
	.string "You can start your journey by going to\n"
	.string "Route 29 to the left of Newbark, there\l"
	.string "should be some trainers ready for a\l"
	.string "challenge there so prepare yourself!.$"

NewBarkTown_EventScript_ElmPokedex_Text_3:
# 98 "data/maps/NewBarkTown/scripts.pory"
	.string "Good luck with your journey everyone, I\n"
	.string "will be waiting for the news for when\l"
	.string "you get your first badges!$"
