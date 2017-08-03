#pragma once

#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "DBCAnalyzer.h"

void analyzer(const std::string & _str);
static void search(const std::string & _str);
static void output(const std::string & _str);
static void open_file(const std::string & _str);

static void message_search(const std::string & _str);
static void message_id_search();
static void message_name_search();
static void message_transmitter_search();
static void signal_receiver_search();

static void comment_search(const std::string & _str);
static void comment_message_id_search();

static void signalvalue_search(const std::string & _str);
static void signalvalue_message_id_search();

static void attribute_search(const std::string & _str);
static void attribute_name_search();
static void attribute_valuetype_search();

static void attributevalue_search(const std::string & _str);
static void attributevalue_objecttype_serarch();


#endif