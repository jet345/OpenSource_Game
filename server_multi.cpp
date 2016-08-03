#include "smart.h"

void SendStringToClient(int _socket, const char *_ccp);
const char *itoa(unsigned int _uiData);


int main()
{
	MESSAGE m_message;

	fd_set status;

	int servSock; 
	int clntSock[MAXUSER];
	char is_server[10];
	int iMaxSock;  // 
	int tempSock;

	unsigned int uiUser;

	int iCount;
	int i;

	struct sockaddr_in echoServAddr;  
	struct sockaddr_in echoClntAddr; 

	unsigned short echoServPort;  
	unsigned int clntLen;  

	time_t stTempTime;

	int iRet;  

	unsigned char ucBuffer[500]; 


	///////////////////////////////////
	char For_Server_Problem[7];
	char For_Client_Nick[7];
	char For_Client_Answer[7];
	int answer_count=0;
	int winner_count=1;
	////////////////////////////////////


	echoServPort = 9999;

	servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 

	if (servSock < 0) 
	{
		printf("Socket Function Error!\n");
		return(0);
	}

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET; 
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	echoServAddr.sin_port = htons(echoServPort);

	iRet = bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));

	if (iRet < 0) 
	{
		close(servSock); 
		printf("Bind Failed Error!\n");
		return(0);
	}

	iRet = listen(servSock, MAXPENDING);

	if (iRet < 0)
	{
		close(servSock);  
		printf("Listen  Failed Error!\n");
		return(0);
	}


	clntLen = sizeof(echoClntAddr);
	iMaxSock = servSock + 1;  
	uiUser = 0;

	while (1)
	{
		FD_ZERO(&status);  
		FD_SET(0, &status);
		FD_SET(servSock, &status); 

		for (i = 0; i<uiUser; i++)
		{
			FD_SET(clntSock[i], &status);

			if (iMaxSock <= clntSock[i])
			{
				iMaxSock = clntSock[i] + 1;
			}

		}

		iRet = select(iMaxSock, &status, 0, 0, 0);
		if (iRet < 0)
		{
			printf("Select Function Error!\n");

			strcpy(m_message.m_buffer, "Server error. Connection Closed");

			for (iCount = 0; iCount<uiUser; iCount++) 
			{
				write(clntSock[iCount], &m_message, sizeof(m_message));
			}

			strcpy(m_message.m_buffer, "/q");

			for (iCount = 0; iCount<uiUser; iCount++)  
			{
				write(clntSock[iCount], &m_message, sizeof(m_message));
			}

			break;
		}

		if (1 == FD_ISSET(servSock, &status)) 
		{
			tempSock = accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen);
			
			printf("Socket Number : %d\n", tempSock);

			if (tempSock < 0)
			{
				printf("Accept Function Error!\n");
				continue;
			}


			clntSock[uiUser] = tempSock;  
			uiUser = uiUser + 1;

			printf("Now connected client number : %d\n", uiUser);

		}

		else if (1 == FD_ISSET(0, &status))
		{
			iRet = read(0, m_message.m_buffer, sizeof(m_message.m_buffer));
			strcpy(For_Server_Problem, m_message.m_buffer); 
			m_message.m_buffer[iRet - 1] = 0;
			strcpy(m_message.m_userName, "PROBLEM");

			for (iCount = 0; iCount<uiUser; iCount++) 
			{
				write(clntSock[iCount], &m_message, sizeof(m_message));				

			}

		}

		else
		{
			for (iCount = 0; iCount<uiUser; iCount++)
			{
				if (1 == FD_ISSET(clntSock[iCount], &status))  
				{
					iRet = read(clntSock[iCount], &m_message, sizeof(ucBuffer) - 1);

					if (iRet != 0)
					{
						strcpy(For_Client_Nick,m_message.m_userName);						
						fflush(stdout);
						strcpy(For_Client_Answer,m_message.m_buffer);
						
						for(int k=0; k<7;k++){
							for(int p=0; p<7;p++){
								if(For_Server_Problem[k] == For_Client_Answer[p]){										
									answer_count++;
									break;
								}
							}
						}

						printf("now answer_count = %d\n",answer_count);					


						if(answer_count==7){
							printf("**%s** is CORRECT!! :-) YOU WINNER\n",For_Client_Nick);
							printf("**%s**'s Answer is %s\n",For_Client_Nick,For_Client_Answer);
							m_message.m_Correct_Or_Wrong=1;
							printf("now correct_or_wrong values = %d",m_message.m_Correct_Or_Wrong);
							m_message.m_winner_order = winner_count;
							winner_count++;
						}

						else{
							printf("**%s** is WRONG :-( \n",For_Client_Nick);
							printf("**%s**'s Wrong answer is %s\n",For_Client_Nick,For_Client_Answer);
							m_message.m_Correct_Or_Wrong=-1;
						}

						answer_count=0;

						for (i = 0; i<uiUser; i++)  
						{
							write(clntSock[i], &m_message, sizeof(m_message));							

						}

						
					}
				}
			}

		} // End of else
	}// End of while(1)

	printf("Server Closed...\n");

	close(servSock);  

	for (i = 0; i<uiUser; i++)  
	{
		close(clntSock[i]); 
	}

	return(0); 
}
