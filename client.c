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
  printf("YOU MUST WRITE YOUR ANSWER UPTO 7 WORDS!\n");

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

    if( 1 == FD_ISSET( 0, &status ) )
    {
      iRet = read( 0, m_message.m_buffer, sizeof( m_message.m_buffer ) );        
      m_message.m_buffer[iRet-1] = 0;      
      strcpy( m_message.m_userName, m_userName );
      write( sock, &m_message, sizeof( m_message ) ); 

    }
    else if( 1 == FD_ISSET( sock, &status ) )
    {
      read( sock, &m_message, sizeof( m_message ) );

      if(1==m_message.m_Correct_Or_Wrong || 3==m_message.m_winner_order)
      {
        if(3==m_message.m_winner_order && 1==m_message.m_Correct_Or_Wrong){
          printf("**%s** is LOOSER :-( \n",m_message.m_userName);
          printf("------------GAME OVER---------------\n");
          printf("------------Plese Wait Next Round-----------\n");
          break;
        }
        else{
          printf("**%s** is CORRECT!! :-) YOU WINNER\n",m_message.m_userName);
          printf("**%s**'s Answer is %s\n",m_message.m_userName,m_message.m_buffer);
        }       
      }

      else if(-1==m_message.m_Correct_Or_Wrong){
        printf("**%s** is WRONG :-( \n",m_message.m_userName);
        printf("**%s**'s Wrong answer is %s\n",m_message.m_userName,m_message.m_buffer);
        
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

