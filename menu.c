/*
 * menu.c
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

void Game_Menu_Display(Game_Surface *surface, Player_Score *score, TTF_Font *font_normal, int num_level, int menu_select)
{
	SDL_Rect position;
	
	//Position par defaut des différents background du jeu
	position.x=0;
	position.y=0;	
	//Effacement de l'écran
	SDL_FillRect(surface->background, NULL, SDL_MapRGB(surface->background->format, 200, 190, 183));
	//Affichage de l'image correspondant au menu sélectioné
	switch (menu_select)
	{
		case MENU_MAIN: 
				SDL_BlitSurface(surface->image[IMG_MENU_MAIN], NULL, surface->background, &position);
			break;
		case MENU_LEVEL:
				//Fonction d'affichage du menu niveaux
				Player_Level_Select(surface, score, font_normal);
			break;
		case MENU_SCORE:
				//Fonction d'affichage du menu score
				Player_Score_Menu(surface, score, font_normal);
			break;
		case MENU_WIN:
				//Fonction d'affichage du panneau de victoire à la fin d'un niveau
				Player_Score_Level(surface, score, font_normal, num_level, menu_select);
			break;
		case MENU_LOSE:
				//Fonction d'affichage du panneau de défaite à la fin d'un niveau
				Player_Score_Level(surface, score, font_normal, num_level, menu_select);
			break;
		case MENU_FINISH:
				//Affichage du panneau finale lorsque l'on à  terminé le jeu
				SDL_BlitSurface(surface->image[IMG_MENU_FINISH], NULL, surface->background, &position);
			break;
		
	}	
}

int Game_Menu_Loop(Game_Surface *surface, Mix_Chunk *sound[], TTF_Font *font_big, TTF_Font *font_normal, SDL_TimerID *timer_id)
{	
	SDL_Event event;
	
	SDL_Rect piece_none_pos, arrow_position;
	
	//Structure contenant les données des niveaux		
	Game_Level level[NOMBRE_NIVEAU]={{{0}}};
	//Structure contenant les données du joueur
	Player_Score score[NOMBRE_NIVEAU]={{0}};
	
	int num_level=0, continuer=1, affiche_arrow=MENU_HIDE_ARROW, menu_select=MENU_MAIN;	
	
	//Chargement des données du joueur
	if (Player_Score_Load(score))
	{
		return EXIT_FAILURE;
	}
	//Chargement des niveaux
	if (Game_Level_Load(level))
	{
		return EXIT_FAILURE;
	}
	//Affichage du menu principale			
	Game_Menu_Display(surface, score, font_normal, num_level, MENU_MAIN);
	
	//Débloquage du premier niveau
	score[num_level].levelok=1;
	//Boucle d'évènement de la SDL
	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
			break;
			case SDL_KEYDOWN:
					//Si on presse la touche ECHAP, et que l'on est pas sur le menu principale on affiche ce dernier
					if ((event.key.keysym.sym == SDLK_ESCAPE) && menu_select != MENU_MAIN)
					{
						menu_select=MENU_MAIN;
						Game_Menu_Display(surface, score, font_normal, num_level, MENU_MAIN);
					}					
				break;
			case SDL_MOUSEMOTION:	
				//Fonction permettant de positionner la flèche de sélection du menu selon l'emplacement de la souris
				affiche_arrow=Game_Menu_Selection(surface->background, &arrow_position, event.motion.x, event.motion.y, menu_select, &num_level, 0);				
				//Gestion de l'affichage de la flèche de sélection du menu
				Game_Menu_Arrow(surface, &arrow_position, &affiche_arrow, menu_select);					
													
			break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{	
					//Fonction qui permet de sélectionner le menu correspondant au choix de l'utilisateur selon le positionnement de la souris							
					menu_select=Game_Menu_Selection(surface->background, &arrow_position, event.motion.x, event.motion.y, menu_select, &num_level, 1);
					//Operation à effectuer selon les choix de l'utilisateur
					switch (menu_select)
					{
						case MENU_NEXT:
								//On passe au niveau suivant, sauf si l'utilisateur à termnier le jeu
								if (num_level<NOMBRE_NIVEAU-1)
								{
									num_level++;
								}
								else
								{									
									menu_select=MENU_FINISH;
									num_level=0;
									break;
								}															
						case MENU_GAME:
							//On lance une partie si le niveau choisis est débloqué
							if (score[num_level].levelok)
							{
								//Chargement du niveau
								Game_Level_Play(surface, font_big, level, num_level, &piece_none_pos);
								//Boucle de la partie, retourne le menu à afficher selon si le joueur à gagné ou perdu
								menu_select = Game_Loop(surface, sound, font_big, font_normal, level, score, num_level, &piece_none_pos, timer_id);
							}
							else
							{
								//Si l'utilisateur à choisi un niveau bloqué on réaffiche le menu level
								menu_select=MENU_LEVEL;
							}
																					
							if (menu_select == MENU_WIN)
							{
								//Si le joueur à gagné on débloque le niveau suivant sauf si c'est à la fin du jeu
								if (num_level<NOMBRE_NIVEAU-1)
								{
									score[num_level+1].levelok=1;
								}								
								//On joue le son de fin du niveau
								if (Mix_PlayChannel(0, sound[4], 0))
								{
									printf ("%s\n", Mix_GetError());
								}
							}
							break;					
						case MENU_QUIT:
								continuer=0;
							break;	
					}
					//Selon le choix du joueur ou le résultat de la partie on affiche l'image correspondante															
					Game_Menu_Display(surface, score, font_normal, num_level, menu_select);				
				}							
			break;
		}
		//On met à jour l'affichage			
		SDL_Flip(surface->background);			
	}	
	//On sauvegarde les données du joueur
	if (Player_Score_Save(score))
	{
		return EXIT_FAILURE;
	}
	
	return 0;
}

int Game_Menu_Selection(SDL_Surface *background, SDL_Rect *arrow_position, int x_mouse, int y_mouse, int menu_select, int *num_level, int mouse_clic)
{	
	//Si le menu actuellement affiché est : le principale, de victoire, de fin de jeu ou d'échec			
	if (menu_select == MENU_MAIN || menu_select == MENU_WIN || menu_select == MENU_FINISH || menu_select == MENU_LOSE)
	{	
		//Si la souris est dans la zone du menu
		if (Game_Menu_Bouton (x_mouse, y_mouse, 260, 255, 280, 295))
		{	
			//Si l'utilisateur n'a pas cliquer sur la souris	
			if (!mouse_clic)
			{
				//On positionne la flèche de sélection du menu selon l'élément du menu selectionné
				arrow_position->y=255+(80*Game_Menu_Mouse_Select(background, y_mouse-255, menu_select));
				
				/*	Si on est sur le menu de victoire, d'échec ou de fin du jeu il y a moins d'éléments dans ces menus
				 * 	On affiche donc pas les flèches
				 */	
				if ((menu_select == MENU_WIN || menu_select == MENU_LOSE || menu_select == MENU_FINISH) && arrow_position->y<415)
				{
					return MENU_HIDE_ARROW;
				}				
				return MENU_SHOW_ARROW;				
			}
			else
			{
				/*	Quand l'utilisateur clique on retourne le menu sélectionné
				 *	Selon le menu actuellement affiché certain éléments n'ont pas la même valeur de retour
				 */
				switch (Game_Menu_Mouse_Select(background, y_mouse-255, menu_select))
				{
					case 0:
						//Si l'utilisateur à cliqué sur "Jouer" sur le menu principale
						if (menu_select == MENU_MAIN)
						{
							return MENU_GAME;
						}
						break;							
					case 1:
						//Si l'utilisateur à cliqué sur "Niveau sur le menu pricipale
						if (menu_select == MENU_MAIN)
						{
							return MENU_LEVEL;
						}
						break;							
					case 2:
						//Si l'utilisateur à cliqué sur "Score" sur le menu princpale
						if (menu_select == MENU_MAIN)
						{	
							return MENU_SCORE;
						}
						//Sur les autres menu la valeur de retour lance une partie
						else
						{
							return MENU_GAME;
						}
						break;
					case 3:
						//Si l'utilisateur à cliqué sur "Quitter" sur le menu princpale
						if (menu_select == MENU_MAIN)
						{
							return MENU_QUIT;
						}
						/*	Si le menu affiché est la fin du jeu ou echec la valeur de retour 
						 * 	permet de revenir au menu principale
						 */
						else if (menu_select == MENU_FINISH || menu_select == MENU_LOSE)
						{
							return MENU_MAIN;
						}			
						else
						{
							//Sinon la valeur de retour des autre menu permet de passer au niveau suivant
							return MENU_NEXT;
						};
						break;				
				}
				
			}	
			 
		}
	}
	else
	{
		//Sur les menus "Score" et "Level"  il y a un lien "Retour" pour revenir au menu principale
		if (Game_Menu_Bouton (x_mouse, y_mouse, 346, 550, 114, 30))
		{			
			if (!mouse_clic)
			{
				arrow_position->y=550;
				return MENU_SHOW_ARROW;				
			}
			else
			{
				return MENU_MAIN;
			}
		}
		//Sur le menu "Level" l'utiliateur peut choisir le niveau débloqué à lancer			
		if ((Game_Menu_Bouton (x_mouse, y_mouse, 225, 175, 125, 355)) && (menu_select == MENU_LEVEL))
		{
			if (!mouse_clic)
			{
				//Positionnement de la flèche de sélection selon le niveau survolé avec la souris
				arrow_position->y=175+(36*Player_Level_Selection(background, y_mouse-175));
				return MENU_SHOW_ARROW;				
			}
			else
			{	
				/*	Si l'utilisateur clique sur le lien d'un niveau on charge le numéro du niveau
				 * 	Puis on retourne la valeur pour lancer la partie
				 */
				*num_level=Player_Level_Selection(background, y_mouse-175);
				return MENU_GAME;
			}					
		}		
	}
	
	//Si l'utilisateur clique n'importe ou on renvoie le menu actuellement affiché	
	if (mouse_clic)
	{
		return menu_select;
	}		
	
	return MENU_HIDE_ARROW;
}

int Game_Menu_Mouse_Select(SDL_Surface *background, int y_mouse, int menu_select)
{	
	//Numéro correspondant à un élément du menu		
	int mouse_select=0;
					
	/* On divise par 80 ce qui correspond à l'espacement entre chaque éléments du menu
	 * Pour déterminer quel élément à été sélectionné
	 * */
	if ((y_mouse/8)>10)
	{
		mouse_select=y_mouse/80;
	}
	
	//Effacement de la zone d'affichage des flèches
	Game_Menu_Clean_Zone(background, menu_select, y_mouse);	
	
	//On retourne le numéro de l'élément sélectionné
	return mouse_select;
}

void Game_Menu_Arrow(Game_Surface *surface, SDL_Rect *arrow_position, int *affiche_arrow, int menu_select)
{	
	//Affichage de la flèche de sélection
	if (*affiche_arrow)
	{	
		/*	Si l'utilisateur est sur le menu "Level" ou "Score"
		 * 	le positionnement et la taille de la flèche est différent
		 */
		if (menu_select == MENU_SCORE || menu_select == MENU_LEVEL)
		{			
			arrow_position->x=318;
			
			/*	Si l'ordonnée de la flèche correspond 
			 * 	au positionnement du lien "Retour" l'absisse de la flèche est différent
			 */
			if (arrow_position->y<500)
			{
				arrow_position->x=225;
			}						
			SDL_BlitSurface(surface->image[IMG_ARROW_LITTLE], NULL, surface->background, arrow_position);			
		}
		else
		{
			/*	Sur les autres menu : principale, victoire, echec, fin 
			 * 	la flèche est d'une autre taille avec un absisse identique
			 */
			arrow_position->x=205;			
			SDL_BlitSurface(surface->image[IMG_ARROW], NULL, surface->background, arrow_position);
		}
				
		*affiche_arrow=MENU_HIDE_ARROW;		
	}	
	else
	{	
		//Effacement de la zone d'affichage des flèches en dehors des zones de menu
		Game_Menu_Clean_Zone(surface->background, menu_select, arrow_position->y);		
	}

}

void Game_Menu_Clean_Zone(SDL_Surface *background, int menu_select, int y_mouse)
{
	SDL_Surface *arrow_clear = NULL;
	
	SDL_Rect position;
	
	//Taille et couleur de la zone d'effacement
	int size_arrow_clear=145, r=200, v= 190, b=183;
	
	//Position initiale de la surface d'effacement des flèches
	position.x=205;	
	position.y=410;
	
	//La position et la taille est différent si l'on est sur le menu principale
	if (menu_select == MENU_MAIN)
	{
		size_arrow_clear=300;
		position.y=255;			
	}
	//Sur les menus "Score" et "Level" la taille, le positionnement et la couleur sont différents
	else if (menu_select == MENU_SCORE || menu_select == MENU_LEVEL)
	{
		r=204; 
		v=255;
		b=171;

		size_arrow_clear=50;
		position.x=296;
		position.y=535;	
		
		//Le menu level à une zone d'éffacement supplémentaire pour la sélection des levels
		if(menu_select == MENU_LEVEL && y_mouse<550)
		{
			size_arrow_clear=360;
			position.x=205;
			position.y=175;
		}
				
	}		
	//Affichage de la zone d'éffacement						
	arrow_clear=SDL_CreateRGBSurface(SDL_HWSURFACE,50,size_arrow_clear,32,0,0,0,0);								
	SDL_FillRect(arrow_clear, NULL, SDL_MapRGB(background->format, r, v, b));
	SDL_BlitSurface(arrow_clear, NULL, background, &position);			
		
	//On libère la mémoire de la surface d'éffacement
	SDL_FreeSurface(arrow_clear);
}
