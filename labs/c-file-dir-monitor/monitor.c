#include "logger.h"
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#define BUF_LEN (10 * (sizeof(struct inotify_event) + 250 + 1))

struct Fd{
    char *path;
    int flag;
    int lv;
};

static struct Fd *paths[100];
int size=0;

static int getFolders(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    /*
    printf("%-3s %2d %7jd   %-40s %d %s\n",
        (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
        (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
        (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
        (tflag == FTW_SLN) ? "sln" : "???",
        ftwbuf->level, (intmax_t) sb->st_size,
        fpath, ftwbuf->base, fpath + ftwbuf->base);
    */
    struct Fd *file;
    file = malloc(sizeof(*file));
    if (ftwbuf->level < 2){
        if (tflag == FTW_D){
            paths[size]=file;
            paths[size]->flag = tflag;
            paths[size]->lv = ftwbuf->level;
            paths[size++]->path = strdup(fpath);
        }
    }
    return 0;           /* To tell nftw() to continue */
}

static void displayInotifyEvent(struct inotify_event *i){/* Display information from inotify_event structure */
/*
    printf("    wd =%2d; ", i->wd);
    if (i->cookie > 0)
        printf("cookie =%4d; ", i->cookie);
        */

    char *action=NULL;
    //printf("mask = ");
    if (i->mask & IN_ACCESS)        action = "[Accesing ";//printf("IN_ACCESS ");
    //if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    //if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    //if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
    if (i->mask & IN_CREATE)        action = "[Create " ;//printf("IN_CREATE ");
    if (i->mask & IN_DELETE)        action = "[Delete ";//("IN_DELETE ");
    //if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    //if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
    //if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
    if (i->mask & IN_MODIFY)        action = "[Modify ";//printf("IN_MODIFY ");
    //if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (i->mask & IN_MOVED_FROM)    action = "[Moved from ";//printf("IN_MOVED_FROM ");
    if (i->mask & IN_MOVED_TO)      action = "[Moved to ";//printf("IN_MOVED_TO ");
    if (i->mask & IN_OPEN)          action = "[Opened ";//printf("IN_OPEN ");
    //if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    //if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    if(i->wd<size+1 && action!=NULL){
        printf("%s",action);
        if (i->len > 0){
            if(strstr(paths[i->wd]->path,i->name)!=NULL){
                if(i->wd==0)
                    printf(" dir] - %s\n", i->name);
                else
                    printf(" subdir] - %s/%s\n",paths[0]->path, i->name);
            }else
                printf(" file] - %s/%s\n",paths[i->wd]->path, i->name);
        }
    }
}

int main(int argc, char *argv[]){
    // Place your magic here
    if(argc<2){
        printf("No directory provide\n");
        return -1;
    }
    int flags = 0;

    if (nftw((argc < 2) ? "." : argv[1], getFolders, 20, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    /*
    for (int i=0;i<size;i++){
        printf("%-3s %2d %2s\n",
        (paths[i]->flag == FTW_D) ?   "d"   : (paths[i]->flag == FTW_DNR) ? "dnr" :
        (paths[i]->flag == FTW_DP) ?  "dp"  : (paths[i]->flag == FTW_F) ?   "f" :
        (paths[i]->flag == FTW_NS) ?  "ns"  : (paths[i]->flag == FTW_SL) ?  "sl" :
        (paths[i]->flag == FTW_SLN) ? "sln" : "???",
        paths[i]->lv, paths[i]->path);
    }
    */
    printf("Starting File/Directory Monitor on %s\n-----------------------------------------------------\n",argv[1]);

    int inotifyFd, wd, j;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        exit(EXIT_FAILURE);

    inotifyFd = inotify_init();                 /* Create inotify instance */
    if (inotifyFd == -1)
        exit(EXIT_FAILURE);

    /* For each command-line argument, add a watch for all events */

    for (j = 0; j < size; j++) {
        wd = inotify_add_watch(inotifyFd, paths[j]->path, IN_ALL_EVENTS);
        if (wd == -1)
            exit(EXIT_FAILURE);

        //printf("Watching %s using wd %d\n", paths[j]->path, wd);
    }

    for (;;) {                                  /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            exit(EXIT_FAILURE);

        if (numRead == -1)
            exit(EXIT_FAILURE);

        /*FIXME: should use %zd here, and remove (long) cast */
        //printf("Read %ld bytes from inotify fd\n", (long) numRead);

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }
    return 0;
}
