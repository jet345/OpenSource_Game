
#include "smart.h"
#include "/usr/include/mysql/mysql.h"

#define TOPIC_NUM 3
#define TOPIC_LEN	15
/////mysql var//////
char query[255];
char topic[255];
MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
char *server = "localhost";                            //혹은 ip
char *user = "root";
char *password = "rudal45";
char *database = "game_db";
int a;
char topic_query[20];
char known_topic[TOPIC_NUM][TOPIC_LEN] = {"동물","영화","전공과목"};
//////////////////////////////mysql var end//

///////////////////////////////////
char For_Server_Problem[20];
char For_Client_Nick[10];
char For_Client_Answer[20];
char For_Server_Answer[20];
int answer_count=0;
int winner_count=1;
int topic_select_user;
int rng[5];
int score[3]={0,0,0};
////////////////////////////////////

void random_number_generator(){
	int i=0;
	srand(time(NULL));
	for(i=0; i<5; i++){
		rng[i] = rand()%10;
		printf("%d ",rng[i]);
	}
}


void mysql_initial(){

		if( !(conn = mysql_init((MYSQL*)NULL))){        //초기화 함수
						printf("init fail\n");
						exit(1);
		}
		if(!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0)){
						printf("connect error.\n");
						exit(1);	}
		if(mysql_select_db(conn, database) != 0){
						mysql_close(conn);
						printf("select_db fail.\n");
						exit(1);
		}
}
void mysql_query(char query[]){
	int count=0;
	int index=0;
	int n;
	n= (rand()%10);
	if(mysql_query(conn,query)){
					printf("query fail\n");
					exit(1);
	}
	res = mysql_store_result(conn);
	//random_number_generator();
	while( (row=mysql_fetch_row(res))!=NULL){
		/*for(index=0; index<5; index++){
			if(rng[index] == count){
				//random하게 문제를 선택하도록
				strcpy(For_Server_Answer[count],row[0]);
				strcpy(For_Server_Problem[count],row[1]);
				printf("%s \n",For_Server_Problem[count]);
				printf("problem ok\n");
			}
		}*/
		if(n == count){
			strcpy(For_Server_Answer,row[0]);
			strcpy(For_Server_Problem,row[1]);
		}
		count++;
	}
}
void mysql_problem(int topic_number){
	a = topic_number-11;

	switch (a) {
		case 0:
			sprintf(query, "select word, word_initial from game_word where topic =" "'%s'",known_topic[0]);
			mysql_query(query);
			mysql_close(conn);
			break;
		case 1:
			sprintf(query, "select word, word_initial from game_word where topic =" "'%s'",known_topic[1]);
			mysql_query(query);
			mysql_close(conn);
			break;
		case 2:
			sprintf(query, "select word, word_initial from game_word where topic =" "'%s'",known_topic[2]);
			mysql_query(query);
			mysql_close(conn);
			break;
		default:
			printf("Wrong number");
			break;
	}
}

int main()
{
	MESSAGE m_message;

	fd_set status;

	int servSock;
	int clntSock[MAXUSER];
	char is_server[10];
	int iMaxSock;
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
	srand(time(NULL));

	int problem_num_count=0;


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

		//새로운 client소켓이 들어올때
		if (1 == FD_ISSET(servSock, &status))
		{
			tempSock = accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen);
			printf("Socket Number : %d\n", tempSock);
			//m_message.m_socketnumber = tempSock - 4;
			if (tempSock < 0)
			{
				printf("Accept Function Error!\n");
				continue;
			}
			clntSock[uiUser] = tempSock;
			uiUser = uiUser + 1;
			printf("Now connected client number : %d\n", uiUser);
			//3명의 유저가 모였을 때 "게임을 시작합니다"라는 메세지를 모든 소켓에게 전송
			if(uiUser==3){
				strcpy(m_message.m_userName, "SERVER");
				strcpy(m_message.m_buffer,"<<<<<<<< 게임을 시작합니다 >>>>>>>>");
				m_message.m_score = 0;
				//주제를 선택할 유저(0~2) random하게 선택
				topic_select_user = (rand() % 3);

				for (iCount = 0; iCount<uiUser; iCount++)
				{
					if(topic_select_user == iCount){
						m_message.m_topic_select = 1;}

					else{
						m_message.m_topic_select = 0;}
					write(clntSock[iCount], &m_message, sizeof(m_message));
				}

			}
		}

		//server측에서 입력이 들어왔을때
		else if (1 == FD_ISSET(0, &status))
		{
			iRet = read(0, m_message.m_buffer, sizeof(m_message.m_buffer));
				//strcpy(For_Server_Problem, m_message.m_buffer);
				m_message.m_buffer[iRet - 1] = 0;
				strcpy(m_message.m_userName, "PROBLEM");
				for (iCount = 0; iCount<uiUser; iCount++)
				{
					write(clntSock[iCount], &m_message, sizeof(m_message));
				}
		}

		//클라이언트쪽에서 message를 보낸경우
		else
		{
			//모든 클라이언트 소켓 확인
			for (iCount = 0; iCount<uiUser; iCount++)
			{
				//클라이언트 소켓에서 입력이 있을 때
				if (1 == FD_ISSET(clntSock[iCount], &status))
				{
					iRet = read(clntSock[iCount], &m_message, sizeof(ucBuffer) - 1);
					if (iRet != 0)
					{
						//topic_number는 10부터 시작한다.
						if(m_message.m_topic_number >= 10){
							mysql_initial();
							mysql_problem(m_message.m_topic_number); //For_Server_Problem에 문제가 저장됨
							strcpy(m_message.m_topic_name, known_topic[m_message.m_topic_number-11]);

							m_message.m_topic_select = -2;
							m_message.m_topic_number = -1;

							strcpy(m_message.m_problem, For_Server_Problem);
							for(i=0; i<uiUser; i++)
								write(clntSock[i], &m_message, sizeof(m_message));
						}
						else{
							//주제를 고를 때가 아니면 해당 주제에 대해 1문제 출제
							//정답을 맞춘다면
							if(strcmp(For_Server_Answer, m_message.m_buffer)==0){
								score[iCount]++;
								m_message.m_score = score[iCount];

								if(m_message.m_score == 5){
									m_message.m_end = 1;
									printf("%s의 승리로 게임이 종료되었습니다\n",m_message.m_userName);
									for(i=0; i<uiUser; i++){
										write(clntSock[i], &m_message, sizeof(m_message));
									}
									fflush(stdout);
									return 0;
								}
								m_message.m_correct = 1;
								m_message.m_topic_select = 1;
								write(clntSock[iCount],&m_message, sizeof(m_message));
							}
							else{//문제를 못맞추면
								m_message.m_correct = -1;
								//m_message.m_topic_select = 1;
								for(i=0; i<uiUser; i++){
									if(i!=iCount)
										write(clntSock[i], &m_message, sizeof(m_message));
								}
							}

						}
						fflush(stdout);
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
