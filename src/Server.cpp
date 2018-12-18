#include <iostream>
#include <algorithm>//?
#include <fstream>
#include <thread>
#include <string>
#include <mutex>
#include <sstream>
#include <ctime>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "calc.h"

#include <openssl/sha.h>
bool stop_server=false;

#define MAX_CONNECTIONS 255

using namespace std;

mutex mutex_user_list;
mutex mutex_add_user;

struct online_user_block
{
    struct online_user_block *next=NULL;
    struct online_user_block *prev=NULL;
    char name[33];
}typedef online_user_block;

struct online_user_list
{
    struct online_user_block *head=NULL;
    struct online_user_block *tail=NULL;
}typedef online_user_list;

online_user_list *user_list;

online_user_block *find_user_to_list(const char username[33])
{
    online_user_block *tmp=NULL;
    if (user_list->head==NULL && user_list->tail==NULL) return NULL;
    tmp=user_list->head;
    for(;;)
    {
        if (strcmp(tmp->name,username)==0) return tmp;
        else if (tmp->next!=NULL) tmp=tmp->next;
        else break;
    }
    return NULL;
}


int add_user_to_list(const char username[33])
{
    online_user_block *tmp=NULL;
    if(find_user_to_list(username)==NULL)
    {
        if(user_list->head==NULL && user_list->tail==NULL)
        {
            tmp=(online_user_block*)malloc(sizeof(online_user_block));
            strcpy(tmp->name,username);
            tmp->next=NULL;
            tmp->prev=NULL;
            user_list->head=tmp;
            user_list->tail=tmp;
            return 0;
        }
        else
        {
            tmp=(online_user_block*)malloc(sizeof(online_user_block));
            strcpy(tmp->name,username);
            tmp->next=NULL;
            tmp->prev=user_list->tail;
            user_list->tail->next=tmp;
            user_list->tail=tmp;
            return 0;
        }
    }
    return -1;
}

int del_user_to_list(const char username[33])
{
    online_user_block *tmp=NULL;
    if(find_user_to_list(username)!=NULL)
    {
        tmp=find_user_to_list(username);
        if (tmp->next!=NULL && tmp->prev!=NULL)
        {
            tmp->next->prev=tmp->prev;
            tmp->prev->next=tmp->next;
            free(tmp);
            return 0;
        }

        if (tmp->next!=NULL && tmp->prev==NULL) 
        {
            user_list->head=tmp->next;
            tmp->next->prev=NULL;
            free(tmp);
           return 0;
        }

        if (tmp->next==NULL && tmp->prev!=NULL) 
        {
            user_list->tail=tmp->prev;
            tmp->prev->next=NULL;
            free(tmp);
            return 0;
        }

        if(tmp->next==NULL&&tmp->prev==NULL)
        {
            user_list->head=NULL;
            user_list->tail=NULL;
            free(tmp);
            return 0;
        }  
    }
    return -1;
}

void print_online_user_list()
{
    if(user_list->head==NULL&&user_list->tail==NULL)
                cout<<endl<<"Users: NULL"<<endl;
    else
    {
        online_user_block *tmp=NULL;
        tmp=user_list->head;
        cout<<endl<<"Users:"<<endl;
        for(;;)
        {
            cout<<tmp->name<<endl;
            if (tmp->next!=NULL) tmp=tmp->next;
            else break;
        }
    }
}
//-------------------------------------------------
int login_user(string s_buff, int comm_size, int &is_op)
{ 
    if (s_buff.size()<47) return -4;
    string buff_hash="";
    string user_hash="";
	user_hash=s_buff.substr(comm_size+1,40);
	s_buff=s_buff.substr(comm_size+1+user_hash.size(),30); //Парсинг параметра к команде
	cout<<"hash:"<<user_hash<<"!"<<endl;
	cout<<"login:"<<s_buff<<"!"<<endl;
                
                //mutex_user_list.lock();
                for(int x=0;x<user_hash.size();x++)
                {
                    if (!isalnum(user_hash[x]))
                    {
                        return -1;
                        
                    }
                }

                for(int x=0;x<s_buff.size();x++)
                {
                    if (!isalnum(s_buff[x]))
                    {
                        if(s_buff[x]=='_') continue;
                        return -2;
                        
                    }
                }

                    mutex_user_list.lock();
                    if(add_user_to_list(s_buff.c_str())==-1)
                        {
                            mutex_user_list.unlock();
                            return -6;
                        }
                    mutex_user_list.unlock();
                    
    string name = s_buff;
				s_buff="data/"+s_buff;
				ifstream fin(s_buff.c_str());
				if (fin.is_open())
				{
                    getline(fin,buff_hash);
                    if(buff_hash==user_hash)
                    {
                        getline(fin,buff_hash);
					    fin.close();
                        if(buff_hash=="is_op")
                            is_op=1;
                        
                    }
                    else
                    {
                        mutex_user_list.lock();
                        del_user_to_list(name.c_str());
                        mutex_user_list.unlock();
                    
                        return -5;
                    }
				}
				else
                {
                    mutex_user_list.lock();
                    del_user_to_list(name.c_str());
                    mutex_user_list.unlock();
                
				    return -3;
                }
                

				/*
                if (add_user_to_list(s_buff.c_str())==0)
                {
                    name=s_buff;
                    send (Socket,"Hello",strlen("Hello"),0);
                    send (Socket,name.c_str(),strlen(name.c_str()),0);
                } 
                else
                {
                    send (Socket,"Name alredy taken",strlen("Name alredy taken"),0);
                    mutex_user_list.unlock();
                    break;
                }
				*/
              //  mutex_user_list.unlock();  


    return 0;
}

int registered (string s_buff, int comm_size)
{
    
    //cout<<s_buff<<s_buff.size()<<endl;
    if (s_buff.size()<45) return -4;

    string user_hash="";
	user_hash=s_buff.substr(comm_size+1,40);
				s_buff=s_buff.substr(comm_size+1+user_hash.size(),30); //Парсинг параметра к команде
				cout<<"hash:"<<user_hash<<"!"<<endl;
				cout<<"login:"<<s_buff<<"!"<<endl;

				for(int x=0;x<user_hash.size();x++)
                {
                    if (!isalnum(user_hash[x]))
                    {
                        return -1;
                        
                    }
                }

				for(int x=0;x<s_buff.size();x++)
                {
                    if (!isalnum(s_buff[x]))
                    {
                        if(s_buff[x]=='_') continue;
                        return -2;
                        
                    }
                }
                string name =s_buff;
			    s_buff="data/"+s_buff;
				ifstream fin(s_buff.c_str());
				if (fin.is_open())
				{
					fin.close();
					return -3;
				}
				else
				{
                    
					ofstream fout(s_buff.c_str());
					fout<<user_hash<<endl;
                    fout<<"not_op"<<endl;
					fout.close();
                    mutex_user_list.lock();
                    add_user_to_list(name.c_str());
                    mutex_user_list.unlock();
				}
				return 0;
}

int get_all_file(string name, int user_socket)
{
	int flag = 0;
	string buff="";
    name="data/"+name;
	ifstream fin(name.c_str());
		if (fin.is_open())
		{
			while (getline(fin,buff))
			{
                buff=buff+"<>\n";
				cout<<buff<<endl;
				if(send(user_socket,buff.c_str(),buff.size(),0)==-1)
                    return -1;
			}
			fin.close();
            send(user_socket,"EOF<>",strlen("EOF<>"),0);
		}
		else
		{
			return -1;
		}
	return 0;
}

int del_task(string name, string title)
{
    int flag=0;
    string bath="data/"+name;
    string buff="";
    string target_buff="";
    ifstream fin(bath.c_str());
    
    bath=bath+".tmp";
    ofstream fout(bath.c_str());

    getline(fin,buff);
    fout<<buff<<endl;
    getline(fin,buff);
    fout<<buff<<endl;
    while(getline(fin,buff))
    {
        if (buff=="<title>")
        {
            getline(fin,target_buff);
            if (target_buff==title)
                {
                    flag=1;
                    continue;
                }
            else
            {
                fout<<buff<<endl;
                fout<<target_buff<<endl;
                continue;
            }
        }
        if (flag==0)
            fout<<buff<<endl;
        if (buff=="</body>")
            flag=0;
    }
		fin.close();
        fout.close();

        bath="mv -f "+bath+" data/"+name;
        system(bath.c_str());		
    return 0;
}

int Test::add_task(int id)
{
    return 12 + id;
}

int check_file(string name)
{
    int title_flag=-1;
    int status_flag=-1;
    int body_flag=-1;
    string buff="";
    name="data/"+name;
    ifstream fin(name.c_str());
    while (getline(fin,buff))
    {
        if (buff=="<title>")
        {
            title_flag=1;
            while (getline(fin,buff))
            {
                if (buff=="</title>")
                {
                    title_flag=0;
                    break;
                }
            }
        }
        if (buff=="<status>")
        {
            status_flag=1;
            while (getline(fin,buff))
            {
                if (buff=="</status>")
                {
                    status_flag=0;
                    break;
                }
            }
        }
        if (buff=="<body>")
        {
            body_flag=1;
            while (getline(fin,buff))
            {
                if (buff=="</body>")
                {
                    body_flag=0;
                    break;
                }
            }
        }
    }
    if (title_flag==0&&status_flag==0&&body_flag==0)
        return 0;
    else
        return -1;
    
}

int edit_status(string name, string title)
{
    int flag=0;
    string bath="data/"+name;
    string buff="";
    string target_buff="";
    ifstream fin(bath.c_str());
    
    bath=bath+".tmp";
    ofstream fout(bath.c_str());

    getline(fin,buff);
    fout<<buff<<endl;
    getline(fin,buff);
    fout<<buff<<endl;
    while(getline(fin,buff))
    {
        if (buff=="<title>")
        {
            fout<<buff<<endl;
            getline(fin,target_buff);
            if (target_buff==title)
                {
                    fout<<target_buff<<endl;
                    flag=1;
                    continue;
                }
        }
        if (flag && buff=="<status>")
        {
            fout<<buff<<endl;
            getline(fin,target_buff);
            if(target_buff=="waiting")
                fout<<"in process"<<endl;
            else if(target_buff=="in process")
                fout<<"done"<<endl;
            flag=0;
        }
        if (flag==0)
            fout<<buff<<endl;
    }
		fin.close();
        fout.close();

        bath="mv -f "+bath+" data/"+name;
        system(bath.c_str());		
    return 0;
}

string load_str(int Socket, int tryes)
{
    char buff[1024];
    string s_buff;
    string tmp;
    for(int i=0;i<tryes+1;i++)
    {
        if(stop_server) break;
        recv(Socket,buff,1024,MSG_DONTWAIT);
        tmp=buff;
        s_buff=s_buff+tmp;

        if (s_buff[s_buff.size()-1]=='>' && s_buff[s_buff.size()-2]=='<')
	    {
		    s_buff.erase(s_buff.size()-2,2);
            return s_buff;
	    }
        memset(buff,0,1024);
        usleep(500000);
    }
    return "";
}


void authentication_user(int Socket, string name, int is_op)
{
    cout<<"authentication_user"<<endl;
    cout<<"Socket:"<<Socket<<endl;
    cout<<"Name:"<<name<<endl;
    cout<<"Is_op"<<is_op<<endl;

    int drop=0;
    int RecvSize=0;
    string comm="";
    string tmp="";
    string s_buff="";
    string param="";
    char buff[1024];

    get_all_file(name,Socket);
    sleep(10);

    for(int tryes=0;tryes<30;tryes++)
    {
        if(stop_server) break;
        comm="";
        
        s_buff=load_str(Socket,5);
        cout<<"Socket "<<Socket<<" send (buff):"<<s_buff<<endl;
        

        for (int i=0;i<s_buff.size();i++) //Парсинг управляющей команды
            {
                if (s_buff[i]!=' ')
                    comm=comm+s_buff[i];
                else
                    break;
            }

            
          if (comm=="NEW")
		{
            if (s_buff.size()>4)
            {
                param=s_buff.substr(5,150);

            }
			
		}

		if (comm=="DELETE")
		{
            if (s_buff.size()>7)
            {
                param=s_buff.substr(8,150);
            }
			del_task(name,param);

		}

		if(comm=="OPEN")
		{
            if (s_buff.size()>5)
            {
                param=s_buff.substr(6,150);

            }
			
		}

		if (comm=="GET_T")
		{
            //if (s_buff.size()<45)
				
		}


        sleep(1);

    }

    send(Socket,"You disconnect!",strlen("You disconnect!"),0);
    mutex_user_list.lock();
    del_user_to_list(name.c_str());
    mutex_user_list.unlock();
    cout<<"Disconnect "<<Socket<<endl;
    close(Socket);

}


void connected(int Socket)
{
    int send_flag=0;
    int is_op=0;
    int second_try=0;
    int drop=0;
    int tryes=0;
    char buff[1024];
    string s_buff="";
    string comm="";
    string name="";
    string tmp="";
    int RecvSize=0;
    for (int j=0;j<1;j++)
    {
		
		comm="";
        
        if(stop_server) break;
        
        //RecvSize = recv(Socket,buff,1024,MSG_DONTWAIT);
        
        //if(tryes>=5)
          //  break;
        /*
            cout<<"Socket "<<Socket<<" send (buff):"<<buff<<endl;
            cout<<RecvSize;
			tmp=buff;
            s_buff=s_buff+tmp;
            if (RecvSize>0 )
            {
                memset(buff,0,1024);
                //RecvSize=0;
                usleep(1000000);
                continue;
            }
            else if(!(drop >= 3))
            {
                drop++;
                usleep(10000);
                continue;
            }
*/
            //tryes=0;

            s_buff=load_str(Socket,5);
            cout<<"Socket "<<Socket<<" send:"<<s_buff<<endl;
            
            for (int i=0;i<s_buff.size();i++) //Парсинг управляющей команды
            {
                if (s_buff[i]!=' ')
                    comm=comm+s_buff[i];
                else
                    break;
            }

            cout<<"COMM: "<<comm<<endl;
            
            if (comm=="LOGIN")
            {
                second_try=0;
				int flag=0;
                
                flag=login_user(s_buff, comm.size(), is_op);   

                if (flag==0)
                {
                    send_flag=send (Socket,"Login ok<>",strlen("Login ok<>"),0);
                    name=s_buff.substr(46,30);
                    break;
                }
                if (flag==-1)
                    send_flag=send (Socket,"Bad hash<>",strlen("Bad hash<>"),0);
                if (flag==-2)
                    send_flag=send (Socket,"Bad name<>",strlen("Bad name<>"),0);
                if (flag==-3)
                    send_flag=send (Socket,"Login not found<>",strlen("Login not found<>"),0);
                if (flag==-4)
                    send_flag=send (Socket,"Bad command<>",strlen("Bad command<>"),0);
                if (flag==-5)
                    send_flag=send (Socket,"Bad password<>",strlen("Bad password<>"),0);     
                if (flag==-6)
					send_flag=send (Socket,"Name online<>",strlen("Name online<>"),0);
            }

			if (comm=="REG")
			{
                second_try=0;
				int flag=0;
				flag=registered(s_buff,comm.size());
				if (flag==0)
                {
					send_flag=send (Socket,"Registration ok<>\0",strlen("Registration ok<>\0"),0);
                    
                    name=s_buff.substr(44,30);
                    break;
                }
				if (flag==-1)
					send_flag=send (Socket,"Bad hash<>",strlen("Bad hash<>"),0);
				if(flag==-2)
					send_flag=send (Socket,"Bad name<>",strlen("Bad name<>"),0);
				if (flag==-3)
					send_flag=send (Socket,"Name alredy taken<>",strlen("Name alredy taken<>"),0);
                if (flag==-4)
					send_flag=send (Socket,"Bad command<>",strlen("Bad command<>"),0);
			}

			s_buff="";
            memset(buff,0,1024);
            tmp="";
        
        sleep(1);
        tryes++;
    }
    /*
    mutex_user_list.lock();
    del_user_to_list(name.c_str());
    mutex_user_list.unlock();
  */  
    if (name=="")
    {
        if (send_flag!=-1)
            send(Socket,"You disconnect!<>",strlen("You disconnect!<>"),0);
        cout<<"Disconnect "<<Socket<<endl;
        close(Socket);
    }
    else
    {
        thread auth_user(authentication_user, Socket, name, is_op);
        auth_user.detach();
    }
}

void make_connect(int MasterSocket)
{
    for(;;){
        if(stop_server) break;

        int SlaveSocket=accept(MasterSocket,0,0);
        //fcntl(SlaveSocket, F_SETFL, O_NONBLOCK);
        cout << "SERVER: No." << SlaveSocket << " connected.\n";
       // send(SlaveSocket,"",strlen("Hello!\n"),0);
        
        thread connection(connected, SlaveSocket);
        connection.detach();
    }
    cout<<"make_connect is down"<<endl;
}


int main(int argc, char *argv[])
{
    int port=18785;
    if(argc==3 && (string)argv[1]=="-p"){
        try
        {
            port=atoi(argv[2]);
            if(port<1024 || port>65000)
            {
                cout << "Wrong value of port, 1024-65000 must be"<<endl;
                cout<<"Use defaul port 18785"<<endl;
            }
            else
            cout<<"Use port "<<port<<endl;
        }
        catch(...)
        {}
    }
    else
        cout<<"Use defaul port 18785"<<endl;
    port=18785;
    cout<<"Server: started..."<<endl;
    user_list=(online_user_list*)malloc(sizeof(online_user_list*)+1);
    
    int listenerSocket=0;
    struct sockaddr_in addr;    
    string command="";
    listenerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listenerSocket < 0)
        cout<<"Bad Socket!"<<endl;
       
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(listenerSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            cout<<"Bind is bad!"<<endl;
            return 0;
        }

    listen(listenerSocket, MAX_CONNECTIONS);
    
    thread connector(make_connect,listenerSocket);
    connector.detach();

    cout<<"Server: connect_maker is up"<<endl;
    cout<<"Server: All up!"<<endl;
    for(;;)
    {
        command="";
        cout<<endl<<">";
        getline(cin,command);

        if (command=="stop") {stop_server=true; break;}

        if (command=="status")
        {
            mutex_user_list.lock();
            print_online_user_list();
            mutex_user_list.unlock();
        }    

    }
    cout<<"Server: stoped..."<<endl;
    close(listenerSocket);
    sleep (5);
    free(user_list);
    return 0;
}
