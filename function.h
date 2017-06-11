#pragma once
#ifndef __FUNCTION_H___
#define __FUNCTION_H___

typedef struct _message
{
	char m_userName[20];
	char m_buffer[100];
	char m_topic_name[20];
	char m_time[26];
	char m_problem[5][20];

	int m_topic_select;
	int m_topic_number;

} MESSAGE;

#endif
