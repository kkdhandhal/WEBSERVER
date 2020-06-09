#include <iostream>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include <ctime>
#include <vector>

using namespace std;

class HTTP_Responce
{
     private:
        vector<string> token;

    public :
        string http_ver ;
        int http_resp_code;
        tm *nowdatetime ;
        string http_resp_dscr;
        time_t date;
        string server;
        long int contnt_len;
        string contnt_type;
        string connection;
        string http_method;
        string path;
        string parameter;
        string host;
        string useragent;
        string accept;
        string acceptlanguage;

        HTTP_Responce()
        {
            http_ver="HTTP/1.1";
            server="CWEB";
            connection="Closed";
        }

    /// 
    //** Tockenize String **/////
    vector<string> StringToken(string srcString,const char * delim)
    {
        char *tok;
        vector<string> tmp_token;
        tok = strtok((char *)srcString.c_str(), delim);
        //token.push_back(tok);
        while (tok != 0) { 
            tmp_token.push_back(tok);
            tok = strtok(0, delim); 
            } 
            return tmp_token;
    }
    
    int Send_HTTP_Responce(int Cli_Sock_Fd)
    {
        string ret;
        ret=http_ver + " " + to_string(http_resp_code) + " " + http_resp_dscr + "\r\n";
        ret = ret +"Date :" + asctime(nowdatetime) +"\r\n";
        ret = ret + "Server :" + server + "\r\n";
        ret = ret + "Content-Length :" + to_string(contnt_len) + "\r\n";
        ret = ret + "Content-Type :" + contnt_type + "\r\n";
        ret = ret + "Connection :" + connection ;

        if(send(Cli_Sock_Fd,ret.c_str(),ret.length(),0)<0)
            return 1;
        else
            return 0;
    }

    
    int Get_HTTP_file(string responce)
    {
        token = StringToken(responce,"\n");
        for(int i=0;i<token.size();i++)
            std::cout<<"\n"<<i<< "  Token is"<<token[i]<<"\r\n";
        
        return 1;
    }

};