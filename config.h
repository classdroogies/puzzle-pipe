/*
 * config.h
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
 */


#ifndef DEF_CONFIG
#define DEF_CONFIG

//Nom et Version logiciel
#define GAME_VERSION "PuzzlePipe - 1.2"

//Résolution de la fenêtre
#define WIDTH  800
#define HEIGHT 600

//Taille des sprites
#define SPRITE_WIDTH  100
#define SPRITE_HEIGHT 100

//Nombre de niveau
#define NOMBRE_NIVEAU 10

//Compte à rebours en seconde
#define CHRONO 90

//Intervalle timer pour le chrono
#define CHRONO_TIME 1000

//Intervalle timer pour la vidange liquide en milliseconde
#define CHRONO_VIDANGE 100

//Intervalle timer pour la vidange liquide déclenché par le joueur en ms
#define CHRONO_PLAYER_VIDANGE 15

//Police d'écriture
#define FONT "img/Ubuntu-B.ttf"
#define FONT_SIZE_BIG 50
#define FONT_SIZE_NORMAL 25

//Nombre son
#define NOMBRE_SON 5

//Nombre d'image
#define NOMBRE_IMAGE 25

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

#endif
