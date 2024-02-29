#include <../include/allneed.h>
#include<../include/log.h>
int log_init()
{
    struct stat s;
    if(stat("LOG",&s)==-1||!S_ISDIR(s.st_mode))
        mkdir("LOG",0777);
    time_t cur_time;
    time(&cur_time);
    struct tm *curtm = localtime(&cur_time);
    char filname[1024];
    sprintf(filname, "LOG/%.4d%.2d%.2daccess.log", curtm->tm_year + 1900,
            curtm->tm_mon + 1, curtm->tm_mday);
    pass = fopen(filname, "a");
    sprintf(filname, "LOG/%.4d%.2d%.2derror.log", curtm->tm_year + 1900,
            curtm->tm_mon + 1, curtm->tm_mday);
    error = fopen(filname, "a");
    
}

char *zhmon(int mon)
{
    switch (mon)
    {
    case 0:
        return "Jan";
    case 1:
        return "Feb";
    case 2:
        return "Mar";
    case 3:
        return "Apr";
    case 4:
        return "May";
    case 5:
        return "Jun";
    case 6:
        return "Jul";
    case 7:
        return "Aug";
    case 8:
        return "Sep";
    case 9:
        return "Oct";
    case 10:
        return "Nov";
    case 11:
        return "Dec";
    }
}

char *zhxq(int xq)
{
    switch (xq)
    {
    case 0:
        return "Mon";
    case 1:
        return "Tue";
    case 2:
        return "Wes";
    case 3:
        return "Tur";
    case 4:
        return "Far";
    case 5:
        return "Sat";
    case 6:
        return "Sun";
    }
}

void success_halddle(struct sockaddr_in *cli_addr,Request *request,int status,int sendsize)
{
    time_t cur_time;
    time(&cur_time);
    struct tm *curtm = localtime(&cur_time);
    fprintf(pass,"%s - - [%.2d/%s/%.4d:%.2d:%.2d:%.2d +0800] \"%s %s %s\" %d %d\n",
        inet_ntoa(cli_addr->sin_addr), curtm->tm_mday, zhmon(curtm->tm_mon),
        curtm->tm_year+1900,curtm->tm_hour,curtm->tm_min,curtm->tm_sec,
        request->http_method,request->http_uri,request->http_version,status,sendsize);
     fflush(pass);
     
}

void error_haddle(struct sockaddr_in *cli_addr,Request *request)
{
    time_t cur_time;
    time(&cur_time);
    struct tm *curtm = localtime(&cur_time);
    fprintf(error,"[%.2d/%s/%.4d:%.2d:%.2d:%.2d +0800] [ERROR] [client %s] \"%s %s %s\" ",
        curtm->tm_mday, zhmon(curtm->tm_mon),curtm->tm_year+1900,curtm->tm_hour,
        curtm->tm_min,curtm->tm_sec,inet_ntoa(cli_addr->sin_addr),
        request->http_method,request->http_uri,request->http_version
        );
    fflush(error);
}



