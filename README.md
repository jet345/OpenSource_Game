# 초성퀴즈Online
## 개발언어
  server : **c++** , client : **c**


## 게임방법
게임은 3명이 모일 때 시작한다.<br/>
server는 3명의 client들 중 무작위로 주제를 선택할 수 있는 권한을 준다. <br/>
client는 제시된 주제를 선택하고, server는 해당 주제에 맞는 초성Quiz를 출제한다.<br/>
먼저 5점을 내는 client가 승자가 된다. <br/>

## 설치
<pre><code>git clone https://github.com/jet345/OpenSource_Game.git</code></pre>
<pre><code>./all_compile.sh</code></pre>


## 실행
### 1) Server
* Mysql query<br/>
  <b>sql_query.txt</b> 참조
* execute
  <pre><code>./server</code></pre>

### 2) Client
<pre><code>./client {SERVER_IP}</code></pre>

### 전체실행화면
![client 실행화면](http://postfiles10.naver.net/MjAxNzA2MjBfMTQz/MDAxNDk3OTYwMDI2MDAx.wL89UqnYEZWBCtinabuUsA4-RGIVmU-2FIJdtCuJ2tsg.xrUMXU9tViF3FpOIgywHgNl4xUWhZevm1vX_aKY5QRIg.PNG.rudal1994/5.PNG?type=w773)
![게임종료화면](http://postfiles3.naver.net/MjAxNzA2MjBfMTM4/MDAxNDk3OTYwMDI2MDY3.ubxkGwQ6RvYWURlCVJo30d_nzzvpBQr4ZwTG_FBRgpkg.9AdzkZj-t3u4mQXo4sXln2Rdq8Ki-fWOpP0x9y3t0Vkg.PNG.rudal1994/7.PNG?type=w773)


## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
