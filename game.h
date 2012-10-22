/*
 * game.h
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
 * Fonctions pour le chargement des niveaux et affichage des éléments, boucle pour la partie.
 * 
 */


#ifndef DEF_GAME
#define DEF_GAME

//Chargement du fichier contenant l'ensemble des niveaux du jeu
int Game_Level_Load(Game_Level *level);
//Chargement pour l'affichage du niveau en court
void Game_Level_Play(Game_Surface *surface, TTF_Font *font_big, Game_Level *level, int num_level, SDL_Rect *piece_none_pos);
//Copie des données du niveau en court dans le tableau de niveau du joueur
void Game_Level_Player(Game_Level *level, int player_level[], int num_level);
//Gestion du déplacement des pièces
void Game_Piece_Move(Game_Surface *surface, Mix_Chunk *sound[], int player_level[], SDL_Rect *piece_none_pos, int x_mouse, int y_mouse);
//Validation de la tuyauterie afin d'activer l'ouverture du robinet
int Game_Validation(int player_level[], int num_level);
//Ouverture du robinet
void Game_Piece_Valid(Game_Surface *surface, Mix_Chunk *sound[]);
//Affichage du chrono
void Game_Chrono_Texte(Game_Surface *surface, int chrono_time, TTF_Font *font_big);
//Affichage du remplissage des réservoirs
int Game_Chrono_Reservoir(Game_Surface *surface, TTF_Font *font_normal, int *pos_source, int *pos_destination, int you_lose);
//Boucle principale du jeu, gestion des évènements SDL
int Game_Loop(Game_Surface *surface, Mix_Chunk *sound[], TTF_Font *font_big, TTF_Font *font_normal, Game_Level *level, Player_Score *score, int num_level, SDL_Rect *piece_none_pos, SDL_TimerID *timer_id);

#endif
