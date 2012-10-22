/*
 * player.c
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
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "config.h"
#include "interface.h"
#include "player.h"
#include "menu.h"
#include "game.h"

void Player_Score_Menu(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal)
{
	SDL_Rect position;
	
	SDL_Color color_texte={0};
	
	int i=0;
	
	char texte[10]={'\0'};
	
	position.x=0;
	position.y=0;
	//Affichage du menu des scores
	SDL_BlitSurface(surface->image[IMG_MENU_SCORE], NULL, surface->background, &position);
	//Positionnement initiale de la première ligne de donnée	
	position.y = 175;
	//Boucle pour l'affichage des scores par niveau
	for (i = 0; i < NOMBRE_NIVEAU; i++)
	{
		//Positionnement de la première colonne "NIVEAU"
		position.x = 78;
		//Concaténation du numéro du niveau
		sprintf(texte,"Niveau %d", i+1);
		//Creation de la surface avec le texte											
		surface->texte_score = TTF_RenderUTF8_Blended(font_normal, texte, color_texte); 
		//On colle le texte	
		SDL_BlitSurface(surface->texte_score, NULL, surface->background, &position);
		//Positionnement de la deuxième colonne "SCORE"
		position.x = 286;
		//Formatage et affichage des scores sous la forme "000"
		Player_Score_Format(surface, score[i].score, font_normal, position);
		//Positionnement de la troisieme colonne "BONUS"
		position.x = 484;	
		//Formatage et affichage
		Player_Score_Format(surface, score[i].bonus, font_normal, position);
		//Positionnement de la quatrieme colonne "TOTAL"	
		position.x = 656;	
		//Formatage et affichage
		Player_Score_Format(surface, (score[i].score+score[i].bonus), font_normal, position);
		//Changement de ligne
		position.y+=36;
	}
	
}

void Player_Level_Select(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal)
{
	SDL_Rect position;
	
	SDL_Color color_texte={0};
	
	int i=0;
	
	char texte[10]={'\0'};
	
	position.x=0;
	position.y=0;
	//Affichage du menu "Level"
	SDL_BlitSurface(surface->image[IMG_MENU_LEVEL], NULL, surface->background, &position);
	//Position initiale de la première ligne	
	position.y = 175;
	//Boucle pour l'affichage des niveau avec le statut : "Bloqué" - "Ok"
	for (i = 0; i < NOMBRE_NIVEAU; i++)
	{
		//Positionnement de la première colonne "LEVEL"
		position.x = 255;
		//Concaténation du numéro du niveau
		sprintf(texte,"Niveau %d", i+1);
		//Creation de la surface avec le texte												
		surface->texte_score = TTF_RenderUTF8_Blended(font_normal, texte, color_texte); 
		//On colle le texte	
		SDL_BlitSurface(surface->texte_score, NULL, surface->background, &position);		
		//Affichage du statut du niveau
		if (score[i].levelok)
		{
			//Positionnement et afficage si le niveau est débloqué
			position.x = 490;
			surface->texte_score = TTF_RenderUTF8_Blended(font_normal, "Ok", color_texte); 
						
		}
		else
		{
			//Positionnement et afficage si le niveau est bloqué
			position.x = 465;
			surface->texte_score = TTF_RenderUTF8_Blended(font_normal, "Bloqué", color_texte); 
		}
		//On colle le texte		
		SDL_BlitSurface(surface->texte_score, NULL, surface->background, &position);	
		//On passe à la ligne suivante
		position.y+=36;
	}
	
}

int Player_Level_Selection(SDL_Surface *background, int y_mouse)
{
	//Numéro du niveau sélectionné par l'utilisateur
	int num_level=0;
	
	/* On divise par 36 ce qui correspond à l'espacement entre chaque éléments du menu
	 * Pour déterminer quel élément à été sélectionné
	 * */
	if ((y_mouse/3)>10)
	{
		num_level=y_mouse/36;
	}
	//Effacement de la zone d'affichage des flèches
	Game_Menu_Clean_Zone(background, MENU_LEVEL, y_mouse);
	
	//On retourne le numéro du niveau sélectionné par l'utilisateur
	return num_level;

}

void Player_Score_Level(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal, int num_level, int menu_select)
{
	SDL_Rect position;
	
	SDL_Color color_texte={0};
	
	char texte[10]={'\0'};
	
	position.x=0;
	position.y=0;
	//Affichage du menu de fin de niveau selon si le joueur à gagné ou perdu
	if (menu_select == MENU_WIN)
	{
		SDL_BlitSurface(surface->image[IMG_MENU_WIN], NULL, surface->background, &position);
	}
	else
	{
		SDL_BlitSurface(surface->image[IMG_MENU_LOSE], NULL, surface->background, &position);
	}
	//Concaténation du numéro de niveau						
	sprintf(texte,"NIVEAU %d", num_level+1);
	//Création de la surface texte											
	surface->texte_score = TTF_RenderUTF8_Blended(font_normal, texte, color_texte); 
	//Positionnement et affichage du nom du niveau
	position.x = 345;
	position.y = 160;	
	SDL_BlitSurface(surface->texte_score, NULL, surface->background, &position);
	//Positionnement du score 
	position.x = 588;
	position.y = 232;	
	//Formatage et affichage
	Player_Score_Format(surface, score[num_level].score, font_normal, position);
	//Positionnement du bonus
	position.y = 285;	
	//Formatage et affichage
	Player_Score_Format(surface, score[num_level].bonus, font_normal, position);
	//Positionnement du total
	position.y = 338;	
	//Formatage et affichage
	Player_Score_Format(surface, (score[num_level].score+score[num_level].bonus), font_normal, position);
	
}

void Player_Score_Format(Game_Surface *surface, int nbr_point, TTF_Font *font_normal, SDL_Rect position)
{
	SDL_Color color_texte={0};
	
	char texte[4]={'\0'};
	/*	Formatage de l'affichage des nombres avec ajout
	 * 	de 0 selon si le nombre est inférieur à 10 ou 100
	 */
	if (nbr_point<10)
	{
		sprintf(texte,"00%d", nbr_point);
	}
	else if (nbr_point<100)
	{
		sprintf(texte,"0%d", nbr_point);
	}
	else
	{
		sprintf(texte,"%d", nbr_point);
	}
	//Affichage du nombre
	surface->texte_score = TTF_RenderUTF8_Blended(font_normal, texte, color_texte); 
	SDL_BlitSurface(surface->texte_score, NULL, surface->background, &position);
	
}

int Player_Score_Load(Player_Score *score)
{
	FILE *file_score=NULL;
	int i=0;	
	/*	Ouverture du fichier contenant les données du joueur
	 * 	Statut du niveau : Statut-Score-Bonus
	 *  Pour récupérer les données
	 */	
	file_score=fopen("player.score","r");
	
	if (!file_score)
	{
		fprintf(stderr,"ERREUR : L'ouverture du fichier player à échoué.\n");
		
		return 1;
	}
	
	for (i = 0; i < NOMBRE_NIVEAU; i++)
	{
		fscanf(file_score,"%d-%d-%d", &score[i].levelok, &score[i].score, &score[i].bonus);
	}
	
	fclose(file_score);
		
	return 0;
}

int Player_Score_Save(Player_Score *score)
{
	FILE *file_score=NULL;
	int i=0;
	/*	Ouverture du fichier contenant les données du joueur
	 * 	Statut du niveau : Statut-Score-Bonus
	 * 	Pour la sauvegarde des données
	 */	
	file_score=fopen("player.score","w");
	
	if (!file_score)
	{
		fprintf(stderr,"ERREUR : L'ouverture du fichier player à échoué.\n");
		
		return 1;
	}
	
	for (i = 0; i < NOMBRE_NIVEAU; i++)
	{
		fprintf(file_score,"%d-%d-%d\n", score[i].levelok, score[i].score, score[i].bonus);
	}
	
	fclose(file_score);
		
	return 0;
}


