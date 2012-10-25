/*
 * puzzlepipe.c
 * 
 * Copyright 2012 Don Renardo <Don_Renardo@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Compilation : gcc -Wall -Wextra -ansi -O -Wwrite-strings -Wstrict-prototypes -Wuninitialized -Wunreachable-code -o puzzlepipe puzzlepipe.o interface.o menu.o game.o player.o `sdl-config --cflags --libs ` -lSDL_image -lSDL_ttf -lSDL_mixer
 * 
 */
#include "interface.h"
#include "menu.h"

int main(int argc, char **argv)
{	
	Game_Surface surface;
	//Initialisation de la structure des surfaces du jeu
	Game_Init_Surface(&surface);
	TTF_Font *font_big = NULL, *font_normal = NULL;
	Mix_Chunk *sound[NOMBRE_SON] = {NULL};
	SDL_TimerID timer_id = NULL;			
	//Initialisation du jeu : SDL, Image, TTF, Sound.
	if (Game_Init())
	{
		return EXIT_FAILURE;
	}
	//Chargement des images du jeu
	if (Game_Img_Load(surface.image))
	{
		return EXIT_FAILURE;
	}
	//Chargement des sons
	if (Game_Snd_Load(sound))
	{
		return EXIT_FAILURE;
	}
	//Nom de la fenêtre -> define	
	SDL_WM_SetCaption(GAME_VERSION, NULL);
	//Icone de la fenêtre et de la barre des taches -> define
	SDL_WM_SetIcon(surface.image[ROBINET_OUVERT], NULL);
	//Création de la fenêtre		
	surface.background = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	//Chargement des fonts du jeu -> define
	font_big = TTF_OpenFont(FONT, FONT_SIZE_BIG);	
	font_normal = TTF_OpenFont(FONT, FONT_SIZE_NORMAL);
	//Vérification de l'ouverture de la fenêtre
	if (surface.background == NULL)
	{
		fprintf(stderr, "! ERREUR : L'ouverture de la fenêtre à échoué : %s'\n", SDL_GetError());
		return  EXIT_FAILURE;
	}
	//Lancement de la boucle principale du jeu									
	if (Game_Menu_Loop(&surface, sound, font_big, font_normal, &timer_id))
	{
		return EXIT_FAILURE;
	}
	//Libération de la mémoire avant de quitter		
	Game_Quit(&surface, sound, font_big, font_normal, timer_id);	
	return EXIT_SUCCESS;
}

