
#ifndef _CONSTS_REGEX_H
#define _CONSTS_REGEX_H

// Folder
static const char *__r_mkdir = "mkdir([[:space:]]+)([[:alnum:]]+)$";
static const char *__r_rmdir = "rmdir([[:space:]]+)([[:alnum:]]+)$";
static const char *__r_mv_folder = "mv([[:space:]]+)([[:alnum:]]+)([[:space:]]+)([[:alnum:]]+)$";
static const char *__r_mv_rename_folder = "mv([[:space:]]+)((([[:alnum:]]+)/([[:alpha:]]+))+)([[:space:]]+)((([[:alnum:]]+)/([[:alpha:]]+))+)$";

// File
static const char *__r_rm_unlink = "(rm|unlink)([[:space:]]+)([[:alnum:]]*)$";
static const char *__r_mv_file = "mv([[:space:]]+)([[:alnum:]]+).([[:alpha:]]+)([[:space:]]+)([[:alnum:]]+).([[:alpha:]]+)$";
static const char *__r_touch = "touch([[:space:]]+)([[:alnum:]]+).([[:alpha:]]+)$";
static const char *__r_cat = "cat([[:space:]]+)([[:alnum:]]+).([[:alpha:]]+)$";

//Other - Display
static const char *__r_ls = "(ls|ls -a)$";
static const char *__r_ls_time = "ls -t$";

#endif