/*
 *--------------------------------------
 * Program Name: Pokemon
 * Author: FBDAJH
 * License: MIT License
 * Description: Pokemon (pokemon and mechanics are mostly from gen 1)
 *--------------------------------------
*/

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* More headers */
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

/* Include Data */
#include "text.h"
#include "misc.h"
#include "data.h"
#include "savegame.h"
#include "stats.h"
#include "map.h"
#include "battle.h"
#include "menu.h"
#include "items.h"

#include "gfx/battle_gfx.h"
#include "gfx/map_gfx.h"


/* Variables */

uint8_t gameState = 0;

void main(void) {
	int colorIndex;
	/* Seed RNG */
	unsigned seed = rtc_Time();

	/* Setup gfx */
	dbg_ClearConsole();
	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetMonospaceFont(8);

	/* Game */

	srand(seed);

	textBoxSprite1 = gfx_MallocSprite(160, 64);
	textBoxSprite2 = gfx_MallocSprite(160, 64);

	map_Initialize();
	battle_Initialize();

	FindColors();

	map_Setup();

	if (newGame) {
		uint8_t starter = 0;
		newGame = false;
		text_Display("Choose Starter", false);
		while (starter == 0) {
			starter = text_AskQuestion4("Bulbasaur", "Charmander", "Squirtle", " ", false);
		}
		if (starter == 4) {
			party[0] = stats_NewCharacter(152, 100);
		}
		else {
			party[0] = stats_NewCharacter(1 + ((starter - 1) * 3), 5);
		}
		for (starter = 0; starter < 185; starter++) {
			if (rand() % 2) {
				playerItems[starter] = 1;
			}
		}
	}
	map_LoadPokeballs();
	do {
		kb_Scan();
		if (gameState == 0) {
			gameState = map_Loop();
			if (gameState == 1) {
				map_End();
				battle_Setup();
			}
		}
		else if (gameState == 1) {
			gameState = battle_Loop();
			if (gameState == 0) {
				battle_End();
				map_Setup();
			}
		}
	} while (gameState < 2);

	map_End();
	battle_End();
	free(textBoxSprite1);
	free(textBoxSprite2);


	gfx_End();
}