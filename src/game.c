/*
 * game.c
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

#include "interface.h"
#include "player.h"
#include "game.h"

int Game_Level_Load(Game_Level *level)
{
	FILE *level_file = NULL;
	char carac_get = 0;
	int num_get = 0, num_line = 0, i = 0;
	//Ouverture du fichier contenant les données des différents niveaux du jeu
	level_file=fopen("game.level","r");
	if (!level_file)
	{
		fprintf(stderr,"ERREUR : L'ouverture du fichier game.level à échoué.\n");		
		return 1;
	}
	//On boucle tant qu'il y a des lignes dans le fichier, un niveau = une ligne
	do
	{
		carac_get=fgetc(level_file);
		//Conversion alpha->numérique
		switch (carac_get)
		{
			case '0':
				num_get = PIECE_NONE;
				break;
			case '1':
				num_get = PIPE_NONE;
				break;
			case '2':
				num_get = PIPE_HORIZONTAL;
				break;			  
			case '3':
				num_get = PIPE_T_DOWN;
				break;  			   
			case '4':
				num_get = PIPE_T_UP;  
				break;  				  
			case '5':
				num_get = PIPE_T_LEFT;
				break; 				  
			case '6':
				num_get = PIPE_C_UP_LEFT;  
				break;				  
			case '7':
				num_get = PIPE_C_DOWN_LEFT;								
				break;				 
			case '8':
				num_get = PIPE_T_RIGHT;
				break;
			case '9':
				num_get = PIPE_C_UP_RIGHT;
				break; 					
			case 'a':
				num_get = PIPE_C_DOWN_RIGHT;  
				break;						  
			case 'b':
				num_get = PIPE_VERTICAL;
				break;				
		}
		//Si on est pas à la fin de la ligne on charge la donnée
		if (carac_get != '\n')
		{
			level[num_line].piece[i++] = num_get;							
		}
		//Sinon on change de ligne et donc de niveau
		else
		{
			i = 0;
			++num_line;
		}							
	} while (carac_get != EOF);	
	fclose(level_file);
	return 0;
}

void Game_Level_Play(Game_Surface *surface, TTF_Font *font_big, Game_Level *level, int num_level, SDL_Rect *piece_none_pos)
{
	SDL_Rect position;
	SDL_Color color_texte = {0};
	char texte[9] = {'\0'};
	int i = 0; 
	//On efface l'écran
	SDL_FillRect(surface->background, NULL, SDL_MapRGB(surface->background->format, 200, 190, 183));	
	//Positionement du reservoir source						
	position.x = 100;
	position.y = 0;
	//On colle l'image du reservoir source
	SDL_BlitSurface(surface->image[RESERVOIR_SOURCE], NULL, surface->background, &position);
	//Positionnement du robinet
	position.x = 700;
	//On colle l'image du robinet
	SDL_BlitSurface(surface->image[ROBINET_FERMER], NULL, surface->background, &position);
	//Concaténation du numéro du niveau
	sprintf(texte,"NIVEAU %d", num_level + 1);
	//Chargement de la surface avec le texte du niveau				
	surface->texte_level = TTF_RenderUTF8_Blended(font_big, texte, color_texte); 
	//Positionnement du texte
	position.x = 30;
	position.y = 120;	
	//On colle la surface contenant le texte
	SDL_BlitSurface(surface->texte_level, NULL, surface->background, &position);
	//Positionnement du reservoir de destination			
	position.x = 0;
	position.y = 199;
	//On colle l'image du reservoir de destination
	SDL_BlitSurface(surface->image[RESERVOIR_DESTINATION], NULL, surface->background, &position);
	//Positionnement de la première plaque de la grille du jeu
	position.x = 300;
	position.y = 100;
	//Boucle pour le placement et l'affichage des plaques du niveau dans la grille
	for (i = 0; i < 25; i++)
	{
		/*	Si on atteint le bord droit de la grille
		 * 	On positionne la plaque sur la ligne suivante et on repart a
		 * 	partir du bord gauche de la grille
		 */
		if (position.x == WIDTH)
		{
			position.x = 300;
			position.y += SPRITE_HEIGHT;
		}
		/*	On récupère l'emplacement de la plaque vide
		 * 	La plaque qui permet les déplacements
		 */
		if (level[num_level].piece[i] == PIECE_NONE)
		{
			piece_none_pos->x = position.x / 100;
			piece_none_pos->y = position.y / 100;
		}
		//On colle la plaque				
		SDL_BlitSurface(surface->image[level[num_level].piece[i]], NULL, surface->background, &position);
		//On décale l'absisse pour le positionnement de la plaque suivante
		position.x += SPRITE_WIDTH;
	}									
}

void Game_Level_Player(Game_Level *level, int player_level[], int num_level)
{
	int i = 0;
	//Boucle de chargement des données du niveau en cours
	for (i = 0; i < 25; i++)
	{
		player_level[i]=level[num_level].piece[i];
	}	
}

void Game_Piece_Move(Game_Surface *surface, Mix_Chunk *sound[], int player_level[], SDL_Rect *piece_none_pos, int x_mouse, int y_mouse)
{
	SDL_Rect position;
	//on récupère le numéro de la plaque cliqué et de la plaque vide d'après les valeurs x et y
	int case_clic = ((x_mouse - 3) + ((y_mouse - 1) * 5)), case_piece_none = ((piece_none_pos->x - 3) + ((piece_none_pos->y - 1) * 5));
	//On vérifie si la pièce vide est adjacente à la pièce cliqué	
	if (((piece_none_pos->x + 1) == x_mouse && piece_none_pos->y == y_mouse) || \
		((piece_none_pos->x - 1) == x_mouse && piece_none_pos->y == y_mouse) || \
		(piece_none_pos->x == x_mouse && (piece_none_pos->y + 1) == y_mouse) || \
		(piece_none_pos->x == x_mouse && (piece_none_pos->y - 1) == y_mouse))
	{
		//On positionne la plaque vide à l'emplacement de la plaque cliqué
		position.x = x_mouse * 100;
		position.y = y_mouse * 100;
		//On colle la plaque
		SDL_BlitSurface(surface->image[player_level[case_piece_none]], NULL, surface->background, &position);
		//On positionne la plaque cliqué à l'emplacement de la plaque vide
		position.x = piece_none_pos->x * 100;
		position.y = piece_none_pos->y * 100;
		//On colle la plaque
		SDL_BlitSurface(surface->image[player_level[case_clic]], NULL, surface->background, &position);
		//On met à jour les données du tableau de la partie
		player_level[case_piece_none] = player_level[case_clic];
		player_level[case_clic] = 0;
		//On met à jour le positionnement de la plaque vide
		piece_none_pos->x = x_mouse;
		piece_none_pos->y = y_mouse;
		//On joue le son de déplacement d'une plaque
		if (Mix_PlayChannel(-1, sound[0], 0))
		{
			printf ("%s\n", Mix_GetError());
		}
	}
	//Si il n'y a pas de plaque vide adjacente
	else
	{
		//On joue le son de la plaque bloquée
		if (Mix_PlayChannel(-1, sound[1], 0))
		{
			printf ("%s\n", Mix_GetError());
		}
	}	
}

int Game_Validation(int player_level[], int num_level)
{	
	int i = 0;
	 //Si les pièces au niveau du robinet et au niveau du tuyau de réception sont bonnes
	if ((player_level[4] == PIPE_VERTICAL || player_level[4] == PIPE_T_LEFT || player_level[4] == PIPE_C_UP_LEFT) && \
		(player_level[20] == PIPE_HORIZONTAL || player_level[20] == PIPE_T_UP || player_level[20] == PIPE_C_UP_LEFT))	
	{		
		//Parcours de la grille
		for (i = 0; i < 25; i++)
		{
			//Si il y a un tuyau
			if (player_level[i] > PIECE_NONE)
			{			
				//Si le tuyau peu accepter un tuyau à gauche
				if (player_level[i] > 1 && player_level[i] < 8)
				{	
					//Si il est sur le bord gauche
					if ((i % 5) == 0 && i != 20)
					{
						return 1;					
					}	
					//Si le tuyau à gauche peut se connecter									
					if ((player_level[i - 1] < 2 || \
						(player_level[i - 1] > 4 && player_level[i - 1] < 8) || \
						player_level[i - 1] == 11) && i != 20)
					{
						return 1;		
					}
				}
				//Si le tuyau peu accepter un tuyau à droite
				if ((player_level[i] > 1 && player_level[i] < 5) || \
						(player_level[i] > 7 && player_level[i] < 11))
				{	
					//Si il est sur le bord droit
					if ((i % 5) == 4)
					{
						return 1;
					}
					//Si le tuyau à droite peut se connecter				
					if (player_level[i + 1] < 2 || \
						(player_level[i + 1] > 7 && player_level[i + 1] < 12))								
					{
						return 1;																	
					}
				}
				//Si le tuyau peu accepter un tuyau en haut
				if ((player_level[i] > 3 && player_level[i] < 7) || \
					(player_level[i] > 7 && player_level[i] < 10)|| \
					player_level[i] == 11)
				{
					//Si il est sur le bord haut
					if(i > 0 && i < 4)
					{
						return 1;						
					}
					//Si le tuyau au dessus peut se connecter
					if ((player_level[i - 5] < 3 || player_level[i - 5] == 4 || \
						player_level[i - 5] == 6|| player_level[i - 5] == 9) &&  i != 4)
					{	
						return 1;
					}	
				}
				//Si le tuyau peu accepter un tuyau en bas
				if (player_level[i] == 3 || player_level[i] == 5 || \
					(player_level[i] > 6 && player_level[i] < 9) || \
					(player_level[i] > 9 && player_level[i] < 12))
				{	
					//Si il est sur le bord bas
					if (i > 20 && i < 25)
					{
						return 1;
					}				
					//Si le tuyau du bas peut se connecter													
					if (player_level[i + 5] < 4 || \
						player_level[i + 5] == 7 || player_level[i + 5] == 10)
					{
						return 1;
					}					
				}								
			}			
		}					
	}
	else
	{
		return 1;
	}	
	return 0;	
}

void Game_Piece_Valid(Game_Surface *surface, Mix_Chunk *sound[])
{
	SDL_Rect position;
	SDL_Surface *robinet_bg = NULL;
	//Positionnemet de la surface du robinet
	position.x = 700;
	position.y = 0;
	//Création d'une surface d'effacement pour le robinet
	robinet_bg = SDL_CreateRGBSurface(SDL_HWSURFACE,SPRITE_WIDTH,SPRITE_HEIGHT, 32, 0, 0, 0, 0);
	SDL_FillRect(robinet_bg, NULL, SDL_MapRGB(surface->background->format, 200, 190, 183));	
	//On colle la surface pour effacer le robinet fermer
	SDL_BlitSurface(robinet_bg, NULL, surface->background, &position);
	//On colle le robinet ouvert
	SDL_BlitSurface(surface->image[ROBINET_OUVERT], NULL, surface->background, &position);
	//On libère la surface d'éffacement
	SDL_FreeSurface(robinet_bg);
	//On joue le son d'ouverture du robinet
	if (Mix_PlayChannel(1, sound[2], 0))
	{
		printf ("%s\n", Mix_GetError());
	}
}

void Game_Chrono_Texte(Game_Surface *surface, int chrono_time, TTF_Font *font_big)
{
	SDL_Rect position;
	SDL_Color color_texte = {255, 0, 0};
	char texte[3] = {'\0'};
	//Positionement de la surface du chrono
	position.x = 50;
	position.y = 0;
	//Création d'un surface d'effacement du chrono
	surface->texte_chrono = SDL_CreateRGBSurface(SDL_HWSURFACE,SPRITE_WIDTH,SPRITE_HEIGHT, 32, 0, 0, 0, 0);
	SDL_FillRect(surface->texte_chrono, NULL, SDL_MapRGB(surface->background->format, 200, 190, 183));	
	//On colle la surface d'éffacement
	SDL_BlitSurface(surface->texte_chrono, NULL, surface->background, &position);
	//Formatage du texte du chrono		
	if (chrono_time > 9)
	{
		sprintf(texte,"%d", chrono_time);
	}
	else
	{
		sprintf(texte,"0%d", chrono_time);
	}
	//On charge la surface avec la valeur du chrono				
	surface->texte_chrono = TTF_RenderUTF8_Blended(font_big, texte, color_texte); 
	//Positionnement du texte
	position.x = 70;
	position.y = 20;	
	//On colle le texte
	SDL_BlitSurface(surface->texte_chrono, NULL, surface->background, &position);	
}

int Game_Chrono_Reservoir(Game_Surface *surface, TTF_Font *font_normal, int *pos_source, int *pos_destination, int you_lose)
{	
	SDL_Surface *point_texte = NULL;
	SDL_Rect position;
	int point = 0;
	//Positionnement de la surface d'effacement du liquide source																				
	position.x = *pos_source;
	position.y = 16;	
	//On colle la surface d'effacement
	SDL_BlitSurface(surface->src_water, NULL, surface->background, &position);			
	//On décale la surface d'effacement du liquide	
	*pos_source += 2;			
	//Si le joueur a gagné	
	if (!you_lose)
	{	
		/*	On colle la surface du liquide dans le reservoir de destination	
		 *	en partant du bas 
		 */			
	    *pos_destination -= 1;																
		position.x = 70;
		position.y = *pos_destination;	
		//On colle la surface du liquide						
		SDL_BlitSurface(surface->dst_water, NULL, surface->background, &position);
		//On créé une surface pour l'effacement des points au dessus du reservoir de destination		
		point_texte = SDL_CreateRGBSurface(SDL_HWSURFACE, 76, 24, 32, 0, 0, 0, 0);		
		SDL_FillRect(point_texte, NULL, SDL_MapRGB(surface->background->format, 255, 122, 0));	
		//Positionnement de la surface d'effacement
		position.x = 112;
		position.y = 212;		
		//On colle la surface d'effacement
		SDL_BlitSurface(point_texte, NULL, surface->background, &position);	
		//On met à jour le nombre de point		
		point = ((497 - *pos_destination) * 200) / 497;
		//On positionne l'emplacement du texte des points		
		position.x = 130;
		position.y = 210;
		//On formatte er colle le nombre de points
		Player_Score_Format(surface, point, font_normal, position);																					
	}	
	//On libère la surface d'effacement
	SDL_FreeSurface(point_texte);
	//On retourne le nombre de points
	return point; 					
}

int Game_Loop(Game_Surface *surface, Mix_Chunk *sound[], TTF_Font *font_big, TTF_Font *font_normal, Game_Level *level, Player_Score *score, int num_level, SDL_Rect *piece_none_pos, SDL_TimerID * timer_id)
{
	int player_level[25] = {0}, continuer = 1, menu_select = MENU_MAIN, chrono_time = CHRONO, pos_source = 187, pos_destination = 497, you_lose = 1, time_change = 1;
	
	SDL_Event event;
	//Creation de la surface permettant la réduction du liquide dans le reservoir source
	surface->src_water = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 68, 32, 0, 0, 0, 0);	
	SDL_FillRect(surface->src_water, NULL, SDL_MapRGB(surface->background->format, 44, 91, 113));
	//Création de la surface permettant l'augmentation du liquide dans le reservoir de destination
	surface->dst_water = SDL_CreateRGBSurface(SDL_HWSURFACE, 159, 1, 32, 0, 0, 0, 0);	
	SDL_FillRect(surface->dst_water, NULL, SDL_MapRGB(surface->background->format, 74, 170, 189));
  	//Création du timer SDL permettant d'envoyer des évènements à intervalle prédéfini soit ici toute les secondes
	*timer_id = SDL_AddTimer(CHRONO_TIME, Game_Time_Event, NULL);
	//Chargement des données du niveau en cours dans un tableau indépendant
	Game_Level_Player(level, player_level, num_level);
	//Boucle d'évènement SDL				
	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				//On quitte la partie
				continuer = 0;
				//On arrete le timer d'envoi d'évènement
				SDL_RemoveTimer(*timer_id);
				//On arrête les sons en cours de lecture
				Mix_HaltChannel(0);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					//On quitte la partie
					continuer = 0;
					//On arrete le timer d'envoi d'évènement
					SDL_RemoveTimer(*timer_id);
					//On arrête les sons en cours de lecture
					Mix_HaltChannel(0);
				}			
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					//Si le joueur clique dans la grille du jeu
					if (event.button.x > 300 && event.button.y > 100 && you_lose)
					{	
						//On vérifie si la plaque cliqué peu bouger
						Game_Piece_Move(surface, sound, player_level, piece_none_pos, event.button.x / 100, event.button.y / 100);					
															
					}
					//Si le joueur clique sur le robinet 
					if (event.button.x > 700 && event.button.x<WIDTH && event.button.y > 0 && event.button.y < 100 && you_lose)
					{
						//On vérifie si le circuit est correct
						you_lose = Game_Validation(player_level, num_level);
						//Si il est correct
						if (!you_lose)
						{
							//Le robinet s'ouvre							
							Game_Piece_Valid(surface, sound);
							//On supprime le timer en cours
							SDL_RemoveTimer(*timer_id);
							//On créer uun nouveau a intervalle plus rapide pour accélérer la vidange du liquide
							*timer_id=SDL_AddTimer(CHRONO_PLAYER_VIDANGE, Game_Time_Event, NULL);	
						}						
					}					
				}				
				break;
			//Evenenemet déclenché par le SDL timer
			case  SDL_USEREVENT:
				//Si le chrono n'est pas terminé et que le joueur n'a pas gagné
				if (chrono_time > -1 && you_lose)
				{
					//On met à jour l'affichage du chrono
					Game_Chrono_Texte(surface, chrono_time--, font_big);										
				}
				//Sinon si le chrono est à 0, alors le liquide commence à descendre dans le reservoir source
				else if (pos_source < 684)
				{	
					//On change alors la durée du timer
					if (time_change)
					{	
						//si le joueur n'a pas gagné
						if (you_lose)
						{
							//On supprime le timer en cours pour modifier l'intervalle de déclenchement de l'évènement
							SDL_RemoveTimer(*timer_id);
							//On créer un nouveau timer pour que la vidange soit lente
							*timer_id = SDL_AddTimer(CHRONO_VIDANGE, Game_Time_Event, NULL);					
						}						
						//On déclenche le son du liquide
						if (Mix_PlayChannel(0, sound[3], 1))
						{
							printf ("%s\n", Mix_GetError());
						}
						//Variable pour le changement de l'intervalle de temps des évènements
						time_change = 0;
					}
					//On met à jour l'affichage du liquide dans les réservoirs			
					score[num_level].score = Game_Chrono_Reservoir(surface, font_normal, &pos_source, &pos_destination, you_lose);					
				}
				else 
				{	
					/*	Si la partie est finie
					 * 	On réinitialise les variable du jeu
					 * 	 Et met à jour les données du joueur
					 * 	sur le niveau terminé
					 */					
					score[num_level].bonus = chrono_time + 1;								
					chrono_time = CHRONO;
					pos_source = 187;
					pos_destination = 497;
					time_change = 1;
					SDL_RemoveTimer(*timer_id);
					Mix_HaltChannel(0);
					continuer = 0;
					//Si le joueur à perdu						
					if (you_lose)
					{
						//on affiche le menu "Perdu"						
						menu_select = MENU_LOSE;
					}
					else
					{
						//Sinon on affiche le menu "Gagné"
						menu_select = MENU_WIN;
					}					
				}												
				break;
		}
		//On met à jour l'affichage
		SDL_Flip(surface->background);		
	}
	//on retourne le menu à afficher
	return menu_select;
}
