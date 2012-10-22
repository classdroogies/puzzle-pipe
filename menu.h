/*
 * menu.h
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
 * 
 * Fonctions pour la gestions de l'affichage des flèches des différents menu du jeu et boucle d'évènement principale.
 * 
 */


#ifndef DEF_MENU
#define DEF_MENU

//Fonction pour l'affichage des images des différents menus
void Game_Menu_Display(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal, int num_level, int menu_select);
//Gestion de la sélection du menu selon le positionnement de la souris
int Game_Menu_Selection(SDL_Surface *background, SDL_Rect *arrow_position, int x_mouse, int y_mouse, int menu_select, int *num_level, int mouse_clic);
//Gestion de la sélection du menu principale selon le positionnement de la souris
int Game_Menu_Mouse_Select(SDL_Surface *background, int y_mouse, int menu_select);
//Sélection du niveau par le joueur
int Game_Menu_Level_Select(SDL_Surface *background, int y_mouse);
//Gestion de l'affichage de la flèche de sélection du menu
void Game_Menu_Arrow(Game_Surface *surface, SDL_Rect *arrow_position, int *affiche_arrow, int menu_select);
//Fonction d'effacement de la zone d'affichage des flèches
void Game_Menu_Clean_Zone(SDL_Surface *background, int menu_select, int y_mouse);
//Boucle principale du jeu
int Game_Menu_Loop(Game_Surface *surface, Mix_Chunk *sound[], TTF_Font *font_big, TTF_Font *font_normal, SDL_TimerID *timer_id);

#endif
