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
 * Fichier d'entête contenant les defines pour la configuration du jeu
 * Résolution fenetre, sprite, grille du jeu....
 * Enum des éléments graphiques.
 * Structures pour les données.
 * Fonctions pour la mise en place des éléments de base de l'interface, initialisation des bibliothèques,
 * surfaces, images, sons, fonts, fermeture et libération de la mémoire. Déclenchement des évènements auto.
 * 
 * 
 * 
 * 
 */

#ifndef DEF_INTERFACE
#define DEF_INTERFACE

//Includes globales
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

//Nom et Version logiciel
#define GAME_VERSION "PuzzlePipe - 1.3"
//Résolution de la fenêtre
#define WIDTH  800
#define HEIGHT 600
//Taille des sprites
#define SPRITE_WIDTH  100
#define SPRITE_HEIGHT 100
//Nombre de niveau
#define NOMBRE_NIVEAU 10
//Nombre son
#define NOMBRE_SON 5
//Nombre d'image
#define NOMBRE_IMAGE 25
//Police d'écriture
#define FONT "img/Ubuntu-B.ttf"
#define FONT_SIZE_BIG 50
#define FONT_SIZE_NORMAL 25
//Compte à rebours en seconde
#define CHRONO 90
//Intervalle timer pour le chrono en milliseconde
#define CHRONO_TIME 1000
//Intervalle timer pour la vidange liquide en milliseconde
#define CHRONO_VIDANGE 100
//Intervalle timer pour la vidange liquide déclenché par le joueur en milliseconde
#define CHRONO_PLAYER_VIDANGE 15

//Enumeration du type des pièces du puzzle et des images du jeu
enum piece_type
{
	PIECE_NONE,						
	PIPE_NONE,			
	PIPE_HORIZONTAL,
	PIPE_T_DOWN,          
	PIPE_T_UP,                 
	PIPE_T_LEFT, 
	PIPE_C_UP_LEFT,
	PIPE_C_DOWN_LEFT,        
	PIPE_T_RIGHT,       
	PIPE_C_UP_RIGHT,                         
	PIPE_C_DOWN_RIGHT,
	PIPE_VERTICAL, 
	RESERVOIR_SOURCE,
	RESERVOIR_DESTINATION,
	ROBINET_FERMER,
	ROBINET_OUVERT,
	BLOC,
	IMG_ARROW,
	IMG_ARROW_LITTLE,
	IMG_MENU_MAIN,
	IMG_MENU_LEVEL,
	IMG_MENU_SCORE,
	IMG_MENU_WIN,
	IMG_MENU_LOSE,
	IMG_MENU_FINISH     
};

//Enumeration pour les différents menu du jeu
enum menu_type
{
	MENU_HIDE_ARROW,
	MENU_SHOW_ARROW,
	MENU_MAIN,
	MENU_GAME,
	MENU_NEXT,
	MENU_LEVEL,
	MENU_SCORE,
	MENU_QUIT,
	MENU_WIN,
	MENU_LOSE,
	MENU_FINISH
};

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

//Structure pour les niveaux
typedef struct Game_Level Game_Level;
struct Game_Level
{
	int piece[25];
};

/*	Structure pour les données du joueur
 * 	Bool pour le bloquage/debloquage du niveau
 * 	Le score du joueur
 *  Le bonus temps restant
 */ 
typedef struct Player_Score Player_Score;
struct Player_Score
{
	int levelok;
	int score;
	int bonus;
};

//Initialisation des surfaces du jeu
void Game_Init_Surface(Game_Surface *surface);
//Initialisation des différentes bibliothèques SDL, SDL_ttf, SDL_image
int Game_Init(void);
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
