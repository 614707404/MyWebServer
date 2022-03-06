//
// Created by 戴威涛 on 2022/3/5.
//

#ifndef MYWEBSERVER_HTTPPARSER_H
#define MYWEBSERVER_HTTPPARSER_H

#define BUFFER_SIZE 4096
enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT };
enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };
enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR,CLOSED_CONNECTION };

LINE_STATUS parse_line( char* buffer, int& checked_index, int& read_index );
HTTP_CODE parse_requestline( char* szTemp, CHECK_STATE& checkstate );
HTTP_CODE parse_headers( char* szTemp );
HTTP_CODE parse_content( char* buffer, int& checked_index, CHECK_STATE& checkstate, int& read_index, int& start_line );

#endif //MYWEBSERVER_HTTPPARSER_H
