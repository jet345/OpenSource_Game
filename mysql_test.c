#include <stdio.h>
#include "/usr/include/mysql/mysql.h"

char query[255];
char topic[255];
int main(){
        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW row;

        char *server = "localhost";                            //혹은 ip
        char *user = "root";
        char *password = "rudal45";
        char *database = "game_db";

        if( !(conn = mysql_init((MYSQL*)NULL))){        //초기화 함수
                printf("init fail\n");
                exit(1);
        }

        printf("mysql_init sucsess.\n");

        if(!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0)){
                printf("connect error.\n");    
                exit(1);
        }

        printf("mysql_real_connect suc.\n");

        if(mysql_select_db(conn, database) != 0){
                mysql_close(conn);
                printf("select_db fail.\n");
                exit(1);
        }
        printf("select mydb suc.\n");

 

        if(mysql_query(conn,"select * from game_word" )){
                printf("query fail\n");
                exit(1);
        }

        printf("query sucsess\n");

        res = mysql_store_result(conn);
        printf("res suceese\n");

       

        while( (row=mysql_fetch_row(res))!=NULL){
                printf("%s %s\n", row[0], row[1]); 
        }

        topic = ''
        sprintf(query, "select * from game_word where topic =" "'%s'",'');

        if(mysql_query(conn,, ))

        mysql_close(conn);

        return 0;
}