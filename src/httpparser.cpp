//
// Created by 戴威涛 on 2022/3/5.
//
#include "httpparser.h"
#include <string>
//解析出一行内容
//http请求报文每行的末尾为\r\n
LINE_STATUS parse_line( char* buffer, int& checked_index, int& read_index )
{
    char temp;
    for ( ; checked_index < read_index; ++checked_index )
    {
        temp = buffer[ checked_index ];
        if ( temp == '\r' )
        {
            //数据不完整，因为\r后面必为\n
            if ( ( checked_index + 1 ) == read_index )
            {
                return LINE_OPEN;
            }
            //出现\r\n，读取到一行
            else if ( buffer[ checked_index + 1 ] == '\n' )
            {
                buffer[ checked_index++ ] = '\0';
                buffer[ checked_index++ ] = '\0';
                return LINE_OK;
            }
            //其他情况，格式错误
            return LINE_BAD;
        }
        else if( temp == '\n' )
        {
            if( ( checked_index > 1 ) &&  buffer[ checked_index - 1 ] == '\r' )
            {
                buffer[ checked_index-1 ] = '\0';
                buffer[ checked_index++ ] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}
//分析请求行
//GET index.html HTTP/1.1
HTTP_CODE parse_requestline( char* szTemp, CHECK_STATE& checkstate )
{
    char* szURL = strpbrk( szTemp, " \t" );
    if ( ! szURL )
    {
        return BAD_REQUEST;
    }
    //截断字符数组
    *szURL++ = '\0';

    char* szMethod = szTemp;
    if ( strcasecmp( szMethod, "GET" ) == 0 ){
        printf( "The request method is GET\n" );
    }else if( strcasecmp( szMethod, "POST" ) == 0 ){
        printf( "The request method is POST\n" );
    }else{
        return BAD_REQUEST;
    }

    //去除空格
    szURL += strspn( szURL, " \t" );
    char* szVersion = strpbrk( szURL, " \t" );
    if ( ! szVersion )
    {
        return BAD_REQUEST;
    }
    *szVersion++ = '\0';
    szVersion += strspn( szVersion, " \t" );
    if ( strcasecmp( szVersion, "HTTP/1.1" ) != 0 )
    {
        return BAD_REQUEST;
    }

//    if ( strncasecmp( szURL, "http://", 7 ) == 0 )
//    {
//        szURL += 7;
//        szURL = strchr( szURL, '/' );
//    }
//
//    if ( ! szURL || szURL[ 0 ] != '/' )
//    {
//        return BAD_REQUEST;
//    }

    //URLDecode( szURL );
    printf( "The request URL is: %s\n", szURL );
    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}
HTTP_CODE parse_headers( char* szTemp )
{
    //遇到一个空行，说明取到了一个正确的请求
    if ( szTemp[ 0 ] == '\0' ){
        return GET_REQUEST;
    }else if ( strncasecmp( szTemp, "Host:", 5 ) == 0 ){
        szTemp += 5;
        szTemp += strspn( szTemp, " \t" );
        printf( "the request host is: %s\n", szTemp );
    }else{//暂时不处理其他部分
        printf( "I can not handle this header\n" );
    }

    return NO_REQUEST;
}
//解析请求
HTTP_CODE parse_content( char* buffer, int& checked_index, CHECK_STATE& checkstate, int& read_index, int& start_line )
{
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE retcode = NO_REQUEST;
    while( ( linestatus = parse_line( buffer, checked_index, read_index ) ) == LINE_OK )
    {
        char* szTemp = buffer + start_line;
        start_line = checked_index;
        switch ( checkstate )
        {
            case CHECK_STATE_REQUESTLINE:
            {
                retcode = parse_requestline( szTemp, checkstate );
                if ( retcode == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:
            {
                retcode = parse_headers( szTemp );
                if ( retcode == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                else if ( retcode == GET_REQUEST )
                {
                    return GET_REQUEST;
                }
                break;
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }
    if( linestatus == LINE_OPEN )
    {
        return NO_REQUEST;
    }
    else
    {
        return BAD_REQUEST;
    }
}
