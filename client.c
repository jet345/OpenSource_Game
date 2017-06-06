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

  memset( &echoServAddr, 0, sizeof( echoServAddr ) );
  echoServAddr.sin_family = AF_INET;
  echoServAddr.sin_addr.s_addr = inet_addr( servIP );
  echoServAddr.sin_port = htons( 9999 );

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
    //클라이언트가 입력할때
    if( 1 == FD_ISSET( 0, &status ) )
    {
      iRet = read( 0, m_message.m_buffer, sizeof( m_message.m_buffer ) );
      m_message.m_buffer[iRet-1] = 0;
      strcpy( m_message.m_userName, m_userName );
      write( sock, &m_message, sizeof( m_message ) );
    }

    //다른 socket에서 메시지가 도착했을 때
    else if( 1 == FD_ISSET( sock, &status ) )
    {
      read( sock, &m_message, sizeof( m_message ) );

      //해당 클라이언트가 topic을 선택할 차례
      if(m_message.m_topic_select == 1){
        printf("주제를 선택해주세요~\n");
        printf("1. 동물\n");
        printf("2. 영화\n");
        printf("3. 전공과목\n");

        scanf("%d", &topic_number);
        m_message.m_topic_number = topic_number+10;
        m_message.m_topic_select = -1;
        write( sock, &m_message, sizeof( m_message ) );
        topic_number = -1;
      }

      else{
        printf("[%s] : ",m_message.m_userName);
        printf("%s\n",m_message.m_buffer);
      }
    }
  }

  close( sock );
  return 0;

}
