/*
 * player.h
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
 * Fonctions pour la sauvegarde, le chargement, la gestion et l'affichage des données du joueur.
 * 
 */


#ifndef DEF_PLAYER
#define DEF_PLAYER

//Chargement du fichier de score du joueur
int Player_Score_Load(Player_Score *score);
//Gestion de l'affichage de la liste des niveaux
void Player_Level_Select(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal);
//Formatage de l'affichage des scores
void Player_Score_Format(Game_Surface *surface, int nbr_point, TTF_Font *font_normal, SDL_Rect position);
//Gestion de l'affichage du tableau de score du joueur
void Player_Score_Menu(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal);
//Gestion de l'affichage du score du joueur à la fin du niveau
void Player_Score_Level(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal, int num_level, int menu_select);
//Sauvegarde des scores du joueur et des niveaux débloqués
int Player_Score_Save(Player_Score *score);

#endif
