NewBarkTown_MapScripts::
# 2 "data/maps/NewBarkTown/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION:
# 3 "data/maps/NewBarkTown/scripts.pory"
	goto_if_unset FLAG_ADVENTURE_STARTED, NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_1
	return

NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
# 4 "data/maps/NewBarkTown/scripts.pory"
	checkplayergender
# 5 "data/maps/NewBarkTown/scripts.pory"
	compare VAR_RESULT, MALE
	goto_if_ne NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_4
# 9 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL
NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_3:
# 12 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_1
# 13 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_1, 22, 13
# 14 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_1, MOVEMENT_TYPE_FACE_RIGHT
# 15 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_1
# 17 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_2
# 18 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_2, 22, 14
# 19 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_2, MOVEMENT_TYPE_FACE_RIGHT
# 20 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_2
# 22 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_3
# 23 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_3, 24, 13
# 24 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_3, MOVEMENT_TYPE_FACE_LEFT
# 25 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_3
# 27 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_4
# 28 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_4, 24, 14
# 29 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_4, MOVEMENT_TYPE_FACE_LEFT
# 30 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_4
	return

NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_4:
# 6 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL
	goto NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_3


NewBarkTown_EventScript_AreaIsClosed::
# 36 "data/maps/NewBarkTown/scripts.pory"
	lock
# 37 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_AreaIsClosed_Text_0
# 38 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Youngerster1::
# 42 "data/maps/NewBarkTown/scripts.pory"
	lock
# 43 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 44 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster1_2
# 48 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster1_Text_1
NewBarkTown_EventScript_Youngerster1_1:
# 50 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster1_2:
# 45 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster1_Text_0
	goto NewBarkTown_EventScript_Youngerster1_1


NewBarkTown_EventScript_Youngerster2::
# 54 "data/maps/NewBarkTown/scripts.pory"
	lock
# 55 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 56 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster2_2
# 60 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster2_Text_1
NewBarkTown_EventScript_Youngerster2_1:
# 62 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster2_2:
# 57 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster2_Text_0
	goto NewBarkTown_EventScript_Youngerster2_1


NewBarkTown_EventScript_Youngerster3::
# 66 "data/maps/NewBarkTown/scripts.pory"
	lock
# 67 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 68 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster3_2
# 72 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster3_Text_1
NewBarkTown_EventScript_Youngerster3_1:
# 74 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster3_2:
# 69 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster3_Text_0
	goto NewBarkTown_EventScript_Youngerster3_1


NewBarkTown_EventScript_Youngerster4::
# 78 "data/maps/NewBarkTown/scripts.pory"
	lock
# 79 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 80 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster4_2
# 84 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster4_Text_1
NewBarkTown_EventScript_Youngerster4_1:
# 86 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster4_2:
# 81 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster4_Text_0
	goto NewBarkTown_EventScript_Youngerster4_1


NewBarkTown_EventScript_Rival::
# 90 "data/maps/NewBarkTown/scripts.pory"
	lock
# 91 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 92 "data/maps/NewBarkTown/scripts.pory"
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
# 93 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_0
# 94 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 95 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_EventScript_Rival_Movement_0
# 99 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 100 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_RIVAL, NewBarkTown_EventScript_Rival_Movement_1
# 103 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 104 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_1
# 105 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 107 "data/maps/NewBarkTown/scripts.pory"
	playse SE_PIN
# 108 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_ELM, Common_Movement_ExclamationMark
# 109 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 111 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_ELM, NewBarkTown_EventScript_Rival_Movement_2
# 115 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 117 "data/maps/NewBarkTown/scripts.pory"
	playbgm MUS_RG_OAK, 0
# 118 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_2
# 119 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_3
# 120 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_4
# 121 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_5
# 122 "data/maps/NewBarkTown/scripts.pory"
	giveitem ITEM_POKE_BALL
# 123 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ChooseStarter
# 124 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_ChooseStarter::
# 128 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_0
# 130 "data/maps/NewBarkTown/scripts.pory"
	dynmultichoice 0, 0, TRUE, 6, 0, DYN_MULTICHOICE_CB_NONE, NewBarkTown_EventScript_ChooseStarter_Text_1, NewBarkTown_EventScript_ChooseStarter_Text_2, NewBarkTown_EventScript_ChooseStarter_Text_3, NewBarkTown_EventScript_ChooseStarter_Text_4, NewBarkTown_EventScript_ChooseStarter_Text_5, NewBarkTown_EventScript_ChooseStarter_Text_6, NewBarkTown_EventScript_ChooseStarter_Text_7, NewBarkTown_EventScript_ChooseStarter_Text_8, NewBarkTown_EventScript_ChooseStarter_Text_9, NewBarkTown_EventScript_ChooseStarter_Text_10, NewBarkTown_EventScript_ChooseStarter_Text_11, NewBarkTown_EventScript_ChooseStarter_Text_12, NewBarkTown_EventScript_ChooseStarter_Text_13, NewBarkTown_EventScript_ChooseStarter_Text_14, NewBarkTown_EventScript_ChooseStarter_Text_15, NewBarkTown_EventScript_ChooseStarter_Text_16, NewBarkTown_EventScript_ChooseStarter_Text_17, NewBarkTown_EventScript_ChooseStarter_Text_18, NewBarkTown_EventScript_ChooseStarter_Text_19, NewBarkTown_EventScript_ChooseStarter_Text_20, NewBarkTown_EventScript_ChooseStarter_Text_21, NewBarkTown_EventScript_ChooseStarter_Text_22, NewBarkTown_EventScript_ChooseStarter_Text_23, NewBarkTown_EventScript_ChooseStarter_Text_24, NewBarkTown_EventScript_ChooseStarter_Text_25, NewBarkTown_EventScript_ChooseStarter_Text_26, NewBarkTown_EventScript_ChooseStarter_Text_27, NewBarkTown_EventScript_ChooseStarter_Text_28, NewBarkTown_EventScript_ChooseStarter_Text_29, NewBarkTown_EventScript_ChooseStarter_Text_30
# 132 "data/maps/NewBarkTown/scripts.pory"
	showmonpic SPECIES_NONE, 10, 3
# 133 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_31, MSGBOX_YESNO
# 134 "data/maps/NewBarkTown/scripts.pory"
	hidemonpic
# 135 "data/maps/NewBarkTown/scripts.pory"
	compare VAR_RESULT, NO
	goto_if_eq NewBarkTown_EventScript_ChooseStarter_2
NewBarkTown_EventScript_ChooseStarter_1:
# 138 "data/maps/NewBarkTown/scripts.pory"
	givemon SPECIES_NONE, 5, ITEM_ORAN_BERRY
# 139 "data/maps/NewBarkTown/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 140 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_32, MSGBOX_DEFAULT
# 141 "data/maps/NewBarkTown/scripts.pory"
	waitfanfare
# 142 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 143 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ElmPokedex
# 144 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_ChooseStarter_2:
# 136 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ChooseStarter
	goto NewBarkTown_EventScript_ChooseStarter_1


NewBarkTown_EventScript_ElmPokedex::
# 148 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_0
# 149 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 150 "data/maps/NewBarkTown/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 151 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_1, MSGBOX_DEFAULT
# 152 "data/maps/NewBarkTown/scripts.pory"
	waitfanfare
# 153 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_2
# 154 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_3
# 155 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_ADVENTURE_STARTED
# 156 "data/maps/NewBarkTown/scripts.pory"
	warp MAP_NEW_BARK_TOWN, 22, 15
# 157 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Nurse::
# 161 "data/maps/NewBarkTown/scripts.pory"
	goto_if_unset FLAG_SYS_POKEDEX_GET, NewBarkTown_EventScript_Nurse_2
# 167 "data/maps/NewBarkTown/scripts.pory"
	goto Common_EventScript_PkmnCenterNurse_Updated
NewBarkTown_EventScript_Nurse_1:
# 169 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Nurse_2:
# 162 "data/maps/NewBarkTown/scripts.pory"
	lock
# 163 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 164 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Nurse_Text_0
	goto NewBarkTown_EventScript_Nurse_1


# 95 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_0:
# 96 "data/maps/NewBarkTown/scripts.pory"
	walk_left
# 97 "data/maps/NewBarkTown/scripts.pory"
	face_right
	step_end

# 100 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_1:
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 101 "data/maps/NewBarkTown/scripts.pory"
	walk_down
	step_end

# 111 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_2:
# 112 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 112 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 113 "data/maps/NewBarkTown/scripts.pory"
	face_left
	step_end

NewBarkTown_EventScript_AreaIsClosed_Text_0:
# 37 "data/maps/NewBarkTown/scripts.pory"
	.string "The door is closed.$"

NewBarkTown_EventScript_Youngerster1_Text_0:
# 45 "data/maps/NewBarkTown/scripts.pory"
	.string "I choose Squirtle for my journey, it\n"
	.string "should help me with fire Pokémon and\l"
	.string "volcanos.$"

NewBarkTown_EventScript_Youngerster1_Text_1:
# 48 "data/maps/NewBarkTown/scripts.pory"
	.string "I heard the Cinnabar Gym Leader picked\n"
	.string "an actual active volcano. An active one.\l"
	.string "I don't know if I signed up for this...$"

NewBarkTown_EventScript_Youngerster2_Text_0:
# 57 "data/maps/NewBarkTown/scripts.pory"
	.string "I was battling on route 29 and got\n"
	.string "jumped by a big Rattata, maybe I need\l"
	.string "to train more before I continue my\l"
	.string "journey.$"

NewBarkTown_EventScript_Youngerster2_Text_1:
# 60 "data/maps/NewBarkTown/scripts.pory"
	.string "Gym Leaders with 'favorable\n"
	.string "environments'... that sounds like a nice\l"
	.string "way of saying 'unfair advantage,' if you\l"
	.string "ask me.$"

NewBarkTown_EventScript_Youngerster3_Text_0:
# 69 "data/maps/NewBarkTown/scripts.pory"
	.string "I forgot to ask Elm to give me my\n"
	.string "Pokédex, I'm too shy to ask him so I\l"
	.string "asked his aide to help me, he should be\l"
	.string "back soon.$"

NewBarkTown_EventScript_Youngerster3_Text_1:
# 72 "data/maps/NewBarkTown/scripts.pory"
	.string "This is going to be the adventure of a\n"
	.string "lifetime. I can't wait!$"

NewBarkTown_EventScript_Youngerster4_Text_0:
# 81 "data/maps/NewBarkTown/scripts.pory"
	.string "This adventure I'm about to go into\n"
	.string "needs more preparations than I\l"
	.string "thought.$"

NewBarkTown_EventScript_Youngerster4_Text_1:
# 84 "data/maps/NewBarkTown/scripts.pory"
	.string "Still, this is going to take most of us\n"
	.string "years, not months. Hope everyone's\l"
	.string "ready for the long haul.$"

NewBarkTown_EventScript_Rival_Text_0:
# 93 "data/maps/NewBarkTown/scripts.pory"
	.string "So you're entering too, huh?. I already\n"
	.string "got my trainer supply kit and my\l"
	.string "starter so I will begin my journey now!$"

NewBarkTown_EventScript_Rival_Text_1:
# 104 "data/maps/NewBarkTown/scripts.pory"
	.string "Next time I see you make sure to be\n"
	.string "ready since I'm going to challenge you\l"
	.string "to a battle!$"

NewBarkTown_EventScript_Rival_Text_2:
# 118 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} you're here too!, I was just\n"
	.string "explaining the others on how the\l"
	.string "tournament will work, it has some\l"
	.string "changes from past tournaments so make\l"
	.string "sure to pay proper attention:$"

NewBarkTown_EventScript_Rival_Text_3:
# 119 "data/maps/NewBarkTown/scripts.pory"
	.string "-Eligibility for Champions league\n"
	.string "requires completion of Kanto and\l"
	.string "Rematches of visiting gym leaders now\l"
	.string "in Kanto.$"

NewBarkTown_EventScript_Rival_Text_4:
# 120 "data/maps/NewBarkTown/scripts.pory"
	.string "-Explains each gym leader has been\n"
	.string "given favorable environments to add to\l"
	.string "the challenge.$"

NewBarkTown_EventScript_Rival_Text_5:
# 121 "data/maps/NewBarkTown/scripts.pory"
	.string "That should be all, pretty simple right?,\n"
	.string "for now take this.$"

NewBarkTown_EventScript_ChooseStarter_Text_0:
# 128 "data/maps/NewBarkTown/scripts.pory"
	.string "Please choose your partner Pokémon,\n"
	.string "make sure to choose carefully.$"

NewBarkTown_EventScript_ChooseStarter_Text_1:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Bulbasaur$"

NewBarkTown_EventScript_ChooseStarter_Text_2:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Charmander$"

NewBarkTown_EventScript_ChooseStarter_Text_3:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Squirtle$"

NewBarkTown_EventScript_ChooseStarter_Text_4:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Chikorita$"

NewBarkTown_EventScript_ChooseStarter_Text_5:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Cyndaquil$"

NewBarkTown_EventScript_ChooseStarter_Text_6:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Totodile$"

NewBarkTown_EventScript_ChooseStarter_Text_7:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Treecko$"

NewBarkTown_EventScript_ChooseStarter_Text_8:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Torchic$"

NewBarkTown_EventScript_ChooseStarter_Text_9:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Mudkip$"

NewBarkTown_EventScript_ChooseStarter_Text_10:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Turtwig$"

NewBarkTown_EventScript_ChooseStarter_Text_11:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Chimchar$"

NewBarkTown_EventScript_ChooseStarter_Text_12:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Piplup$"

NewBarkTown_EventScript_ChooseStarter_Text_13:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Snivy$"

NewBarkTown_EventScript_ChooseStarter_Text_14:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Tepig$"

NewBarkTown_EventScript_ChooseStarter_Text_15:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Oshawott$"

NewBarkTown_EventScript_ChooseStarter_Text_16:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Chespin$"

NewBarkTown_EventScript_ChooseStarter_Text_17:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Fennekin$"

NewBarkTown_EventScript_ChooseStarter_Text_18:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Froakie$"

NewBarkTown_EventScript_ChooseStarter_Text_19:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Rowlet$"

NewBarkTown_EventScript_ChooseStarter_Text_20:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Litten$"

NewBarkTown_EventScript_ChooseStarter_Text_21:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Popplio$"

NewBarkTown_EventScript_ChooseStarter_Text_22:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Grookey$"

NewBarkTown_EventScript_ChooseStarter_Text_23:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Scorbunny$"

NewBarkTown_EventScript_ChooseStarter_Text_24:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Sobble$"

NewBarkTown_EventScript_ChooseStarter_Text_25:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Sprigatito$"

NewBarkTown_EventScript_ChooseStarter_Text_26:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Fuecoco$"

NewBarkTown_EventScript_ChooseStarter_Text_27:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Quaxly$"

NewBarkTown_EventScript_ChooseStarter_Text_28:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Pikachu$"

NewBarkTown_EventScript_ChooseStarter_Text_29:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Eevee$"

NewBarkTown_EventScript_ChooseStarter_Text_30:
# 130 "data/maps/NewBarkTown/scripts.pory"
	.string "Riolu$"

NewBarkTown_EventScript_ChooseStarter_Text_31:
# 133 "data/maps/NewBarkTown/scripts.pory"
	.string "Do you want the {STR_VAR_2}\n"
	.string "Pokémon {STR_VAR_1} as your first\l"
	.string "partner?$"

NewBarkTown_EventScript_ChooseStarter_Text_32:
# 140 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} received a {STR_VAR_1}!$"

NewBarkTown_EventScript_ElmPokedex_Text_0:
# 148 "data/maps/NewBarkTown/scripts.pory"
	.string "And finally here is a Pokédex, it can\n"
	.string "capture all the data of Pokémon you see\l"
	.string "or catch!$"

NewBarkTown_EventScript_ElmPokedex_Text_1:
# 151 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} received the Pokédex!$"

NewBarkTown_EventScript_ElmPokedex_Text_2:
# 153 "data/maps/NewBarkTown/scripts.pory"
	.string "You can start your journey by going to\n"
	.string "Route 29 to the left of Newbark, there\l"
	.string "should be some trainers ready for a\l"
	.string "challenge there so prepare yourself!.$"

NewBarkTown_EventScript_ElmPokedex_Text_3:
# 154 "data/maps/NewBarkTown/scripts.pory"
	.string "Good luck with your journey everyone, I\n"
	.string "will be waiting for the news for when\l"
	.string "you get your first badges!$"

NewBarkTown_EventScript_Nurse_Text_0:
# 164 "data/maps/NewBarkTown/scripts.pory"
	.string "There is something happening at the lab\n"
	.string "to the north, maybe the Professor is\l"
	.string "starting another tournament!$"
