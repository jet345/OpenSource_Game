#pragma once
#ifndef __FUNCTION_H___
#define __FUNCTION_H___

typedef struct _message
{
	char m_userName[20];
	char m_buffer[100];
	char m_time[26];
	int m_Correct_Or_Wrong;
	int m_winner_order;
} MESSAGE;

#endif
