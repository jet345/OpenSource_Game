#include "smart.h"
#include "function.h"

int main(int argc, char *argv[])
{
  fd_set status;
  MESSAGE m_message;
  char m_userName[20];

  int sock;
  struct sockaddr_in echoServAddr;
  unsigned short echoServPort;
  char *servIP;
  char *echoString;
  char buffer[500];
  char For_Solving_Problem[7];
  int topic_number=0;
  time_t stTempTime;
  int iRet;

  servIP = argv[1];

  sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

  if( sock < 0 )
  {
    printf( "Socket Function Failed!\n" );
    return 0;
  }
  //서버 소켓과 같은 포트 9999번을 열어주고 인자로 들어온 servIP로 연결한다.
  memset( &echoServAddr, 0, sizeof( echoServAddr ) );
  echoServAddr.sin_family = AF_INET;
  echoServAddr.sin_addr.s_addr = inet_addr( servIP );
  echoServAddr.sin_port = htons( 9999 );
  //소켓연결 첫번째 단계. connet()
  iRet = connect( sock, ( struct sockaddr* )&echoServAddr, sizeof( echoServAddr ) );

  if( iRet < 0 )
  {
    printf( "Connect Function Failed!\n" );
    close( sock );
    return 0;
  }

  printf( "Connected. Please enter your NickName : " );
  fflush( stdout );
  iRet = read( 0, m_userName, sizeof( m_userName ) );
  m_userName[iRet-1] = 0;
  printf("                [ RULE 설명 ! ]\n");
  printf("서버가 선택한 랜덤한 유저가 주제를 선택할 수 있는 권한을 가지게 됩니다!\n");
  printf("총 3가지 주제 16문제가 출제됩니다\n");
  printf("많이 맞춘 유저가 승자~\n");
  printf("3명의 유저가 입장하면 게임시작합니다 ^.^\n");
  printf("Waiting.....\n");
  printf("\n");
  while( 1 )
  {
    FD_ZERO( &status );
    FD_SET( 0, &status );
    FD_SET( sock, &status );
    iRet = select( sock+1, &status, 0, 0, 0 );

    if( iRet < 0 )
    {
      printf( "Select Function Error!\n" );
      write( sock, LOGOUT, sizeof(LOGOUT) );
      break;
    }
    //클라이언트가 문제에 대한 답을 입력할때
    if( 1 == FD_ISSET( 0, &status ) )
    {
      iRet = read( 0, m_message.m_buffer, sizeof( m_message.m_buffer ) );
      m_message.m_buffer[iRet-1] = 0;
      strcpy( m_message.m_userName, m_userName );
      m_message.m_topic_select = -3;
      write( sock, &m_message, sizeof( m_message ) );
      fflush(stdout);
    }

    //다른 socket(서버 또는 client)에서 메시지가 도착했을 때
    else if( 1 == FD_ISSET( sock, &status ) )
    {
      read( sock, &m_message, sizeof( m_message ) );

      //해당 클라이언트가 정답인경우(server가 보냄)
      if(m_message.m_correct == 1){
        printf("정답입니다^.^\n");
        printf("정답은 %s 입니다",m_message.m_buffer);
        printf("다음 문제로 넘어갑니다\n");
        printf("현재 %s님의 score는 %d점입니다\n",m_message.m_userName, m_message.m_score);
        fflush(stdout);
      }

      if(m_message.m_end == 1){
        printf("%s의 승리로 게임이 종료되었습니다\n",m_message.m_userName);
        fflush(stdout);
        return 0;
      }


      if(m_message.m_topic_select == 1){

        printf("*********주제를 선택해주세요*********\n");
        printf("1. 동물\n");
        printf("2. 영화\n");
        printf("3. 전공과목\n");
        scanf("%d", &topic_number);
        printf("선택된 주제 : %d\n",topic_number);
        m_message.m_topic_number = topic_number + 10;
        //서버소켓에게 전송
        write( sock, &m_message, sizeof( m_message ) );
      }
      //server가 문제 낸 경우
      else if(m_message.m_topic_select == -2){
        //문제출제된 상태
        printf("[%s] : ",m_message.m_topic_name);
        printf("%s\n",m_message.m_problem);
      }
      //client가 보낸 경우
      else if(m_message.m_topic_select == -3){
        //server가 topic이 뭔지 가르쳐주거나
        //다른 client가 친 답이 뭔지 볼 때
        printf("**%s의 답**",m_message.m_userName);
        printf("%s\n",m_message.m_buffer);
        //fflush(stdout);
      }
    }
  }

  close( sock );
  return 0;

}
