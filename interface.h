/*
 * interface.h
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
 * Fonctions pour la mise en place des éléments de base de l'interface, initialisation des bibliothèques,
 * surfaces, images, sons, fonts, fermeture et libération de la mémoire. Déclenchement des évènements auto.
 * 
 * 
 */

#ifndef DEF_INTERFACE
#define DEF_INTERFACE

//Structure des surfaces du jeu
typedef struct Game_Surface Game_Surface;
struct Game_Surface
{
	SDL_Surface *background;
	SDL_Surface *image[NOMBRE_IMAGE];
	SDL_Surface *src_water;
	SDL_Surface *dst_water;
	SDL_Surface *texte_chrono;
	SDL_Surface *texte_level;
	SDL_Surface *texte_score;
};

//Initialisation des différentes bibliothèques SDL, SDL_ttf, SDL_image
int Game_Init();

//Initialisation des surfaces du jeu
void Game_Init_Surface(Game_Surface *surface);

//Chargement des images et affichage
int Game_Img_Load(SDL_Surface *image[]);

//Chargement des sons du jeu
int Game_Snd_Load(Mix_Chunk *sound[]);

//Gestion des boutons des menus.
int Game_Menu_Bouton (int mouseposx, int mouseposy, int surfposx, int surfposy, int surfsizex, int surfsizey);

//Déclenchement d'évènement pour la mise à jours de l'interface pour les réservoirs et le chrono
Uint32 Game_Time_Event (Uint32 interval, void *param);

//Fermeture des différentes bibliothèques SDL, SDL_ttf, SDL_image
void Game_Quit(Game_Surface *surface, Mix_Chunk *sound[], TTF_Font *font_big, TTF_Font *font_normal, SDL_TimerID timer_id);


#endif
