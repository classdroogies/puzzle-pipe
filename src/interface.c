/*
 * interface.c
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

void Game_Init_Surface(Game_Surface *surface)
{
	int i = 0;
	surface->background = NULL;
	for (i = 0; i < NOMBRE_IMAGE; i++)
	{
		surface->image[i] = NULL;
	}
	surface->src_water = NULL;
	surface->dst_water = NULL;	
	surface->texte_chrono = NULL;
	surface->texte_level = NULL;
	surface->texte_score = NULL;	
}

int Game_Init(void)
{
	//Initialistation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER))
	{
		fprintf(stderr,"! ERREUR : L'initialisation de la SDL à échoué : %s'\n", SDL_GetError());
		return 1;
	}
	//Initialisation de bibliothèque pour le texte
	if (TTF_Init())
	{
		fprintf(stderr,"! ERREUR : L'initialisation du module TTF à échoué : %s'\n", SDL_GetError());
		return 1;
	}
	//Initialisation de bibliothèque pour le son
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
	{
		fprintf(stderr,"! ERREUR : L'initialisation du module SDL_MIXER à échoué : %s'\n", Mix_GetError());
		return 1;
	}	
	return 0;
}

int Game_Img_Load(SDL_Surface *image[])
{
	char urlimg[11] = {'\0'};
	int i = 0;
	
	for (i = 0; i < NOMBRE_IMAGE; i++)
	{
		sprintf(urlimg,"img/%d.png",i);
		
		image[i]= IMG_Load(urlimg);
		
		if (image[i] == NULL)
		{
			fprintf(stderr,"ERREUR : L'ouverture de l'image \"%s\" à échoué : %s'\n", urlimg, SDL_GetError());
			return  1;
		}	
	}			
	return 0;	
}

int Game_Snd_Load(Mix_Chunk *sound[])
{
	int i = 0;
	char urlsnd[10] = {'\0'};
	
	for (i = 0; i < NOMBRE_SON; i++)
	{
		sprintf(urlsnd,"snd/%d.wav",i);
		sound[i] = Mix_LoadWAV(urlsnd);
		
		if (!sound[i])
		{
			fprintf(stderr,"ERREUR : L'ouverture du son \"%s\" à échoué : %s'\n", urlsnd, SDL_GetError());
			return  1;
		}
	}
	return 0;
}		

int Game_Menu_Bouton (int mouseposx, int mouseposy, int surfposx, int surfposy, int surfsizex, int surfsizey)
{
	//Si la souris est dans la zone prédéfinie
	if ((mouseposx >= surfposx) && (mouseposy >= surfposy) && (mouseposx <= (surfposx + surfsizex)) && (mouseposy <= (surfposy + surfsizey)))
	{		
		return 1;
	}		
	return 0;		
}

Uint32 Game_Time_Event (Uint32 interval, void *param)
{
	SDL_Event myevent;

	myevent.type = SDL_USEREVENT;
	myevent.user.code = 2;
	myevent.user.data1 = NULL;
	myevent.user.data2 = NULL;
	SDL_PushEvent(&myevent);
	
	return interval;	
}
 
void Game_Quit(Game_Surface *surface, Mix_Chunk *sound[], TTF_Font *font_big, TTF_Font *font_normal, SDL_TimerID timer_id)
{
	int i = 0;	
	for (i = 0; i < NOMBRE_IMAGE; i++)
	{
		SDL_FreeSurface(surface->image[i]);
	}
	for (i = 0; i < NOMBRE_SON; i++)
	{
		Mix_FreeChunk(sound[i]);
	}	
	SDL_FreeSurface(surface->texte_chrono);
	SDL_FreeSurface(surface->texte_level);
	SDL_FreeSurface(surface->texte_score);
	SDL_FreeSurface(surface->src_water);
	SDL_FreeSurface(surface->dst_water);
	SDL_FreeSurface(surface->background);
		
	TTF_CloseFont(font_big); 
	TTF_CloseFont(font_normal); 
	
	SDL_RemoveTimer(timer_id);
	
	Mix_CloseAudio();
	
	SDL_Quit();
	TTF_Quit();	
}

