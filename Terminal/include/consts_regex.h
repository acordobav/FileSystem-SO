#ifndef _CONSTS_REGEX_H
#define _CONSTS_REGEX_H

// Folder
static const char *__r_mv_rename_folder = "mv([[:space:]]+)([[:alnum:]]+)([[:space:]]+)([[:alnum:]]+)$";
static const char *__r_cd = "cd(([[:space:]]+)([[:alnum:]]+)|([[:space:]]+)..)$";
static const char *__r_mkdir = "mkdir([[:space:]]+)([[:alnum:]]+)$";
static const char *__r_rmdir = "rmdir([[:space:]]+)([[:alnum:]]+)$";

// File
static const char *__r_mv_file = "mv([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)$";
static const char *__r_cat_write = "cat([[:space:]]+)>>([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)$";
static const char *__r_unlink = "unlink([[:space:]]+)([[:alnum:]]*)((\\.([[:alpha:]]+))?)$";
static const char *__r_rm = "rm([[:space:]]+)([[:alnum:]]*)((\\.([[:alpha:]]+))?)$";
static const char *__r_touch = "touch([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)$";
static const char *__r_less = "less([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)$";
static const char *__r_cat = "cat([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)$";
static const char *__r_get = "get([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)$";
static const char *__r_close = "([[:space:]]*)close([[:space:]]*)$";
static const char *__r_exit = "([[:space:]]*)exit([[:space:]]*)$"; 

static const char *__r_set_owner = "set([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)([[:space:]]+)-o([[:space:]]+)([[:alnum:]]+)$";
static const char *__r_set_size = "set([[:space:]]+)([[:alnum:]]+)((\\.([[:alpha:]]+))?)([[:space:]]+)-s([[:space:]]+)([[:digit:]]+)$";

//Other - Display
static const char *__r_ls = "(ls|ls -a)$";
static const char *__r_ls_time = "ls -t$";
static const char *__r_vs = "vs -f([[:space:]]*)$";

#endif