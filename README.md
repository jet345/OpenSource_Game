# 초성퀴즈Online
## 게임방법
게임은 3명이 모일 때 시작한다.<br/>
server는 3명의 client들 중 무작위로 주제를 선택할 수 있는 권한을 준다. <br/>
client는 제시된 주제를 선택하고, server는 해당 주제에 맞는 초성Quiz를 출제한다.<br/>
먼저 5점을 내는 client가 승자가 된다. <br/>

## 1. 설치
<pre><code>git clone https://github.com/jet345/OpenSource_Game.git</code></pre>
<pre><code>./all_compile.sh</code></pre>


## 2. 실행
### 1) Server
* Mysql query<br/>
  <b>sql_query.txt</b> 참조
* execute
  <pre><code>./server</code></pre>

### 2) Client
<pre><code>./client {SERVER_IP}</code></pre>

