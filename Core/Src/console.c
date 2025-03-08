 /******************************************************************************
  	* @file : timer.c
  	* @date : Feb 11, 2025
	* @author : Matt Andersen & Andrew Schuman
	* @course : CPE3300
	* @section : 111
	* @assignment : Project 1 Network Interface Node
	* @brief :
  ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart_driver.h"
#include "console.h"

/*
 * @brief  This method initializes the console and prompts
 *         the user to choose which menu to use
 */
void console_start(void)
{
	init_usart2(57600,F_CPU);

//	// fget string
//	char string[3];
//
//	// song choice variable
//	int menu;
//
//	while(1)
//	{
//		menu = 0;
//
//		// Prompt user
//		printf("\nEnter a Message\n");
//	}
}

/*
 * @brief  This method prompt the user to choose a song
 * 		   to play and then calls the play_song() method
 * 		   with the chosen song.
 */
void console_song_selection(void)
{
	// fget string
	char string[3];

	// song choice variable
	int song;

	// This loops until the user enters 'q' to quit
	while(1)
	{
		song = 0;

		// Prompt user
		printf("\nSelect choose number to play song:\n"
				"1 - Imperial March Darth Vader Theme\n"
				"2 - Pirates of the Caribbean Theme\n"
				"3 - quit menu\n\n");

		// Read the input with fgets and convert to int with atoi
		song = atoi(fgets(string,3,stdin));

		// Based of the users input play a song, quit, or print error
		if(song == 1)
		{
			printf("Not a valid input!!");
		}
	}
}

